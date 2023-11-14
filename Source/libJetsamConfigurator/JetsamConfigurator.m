#include "JetsamConfigurator.h"

void jetsamConfiguration(pid_t pid) {
    memorystatus_memlimit_properties_t properties;
    memorystatus_priority_properties_t priority;
    
    // Use our privilege to change jetsam limit at the last second
    memset(&properties, 0, sizeof(memorystatus_memlimit_properties_t));
    memset(&priority, 0, sizeof(memorystatus_priority_properties_t));
    properties.memlimit_active = 840;
    properties.memlimit_inactive = 840;
    priority.priority = JETSAM_PRIORITY_FOREGROUND_SUPPORT;
    
    if (pid == 0) {
        syslog(LOG_ERR, "Jetsam configuration crash: Can't configure pid 0");
        return;
    }

    int err;
    if ((err = memorystatus_control(MEMORYSTATUS_CMD_SET_MEMLIMIT_PROPERTIES, pid, 0, &properties, sizeof(properties)))) {
        syslog(LOG_ERR, "Jetsam configuration crash: Failed to set memlimit properties for pid %d with error %d", pid, err);
        return;
    }

    if ((err = memorystatus_control(MEMORYSTATUS_CMD_SET_PRIORITY_PROPERTIES, pid, 0, &priority, sizeof(priority)))) {
        syslog(LOG_ERR, "Jetsam configuration crash: Failed to set priority properties for pid %d with error %d", pid, err);
        return;
    }
    
    // Get jetsam limit from system
    memset(&properties, 0, sizeof(memorystatus_memlimit_properties_t));
    if ((err = memorystatus_control(MEMORYSTATUS_CMD_GET_MEMLIMIT_PROPERTIES, pid, 0, &properties, sizeof(properties)))) {
        syslog(LOG_ERR, "Jetsam configuration crash: Failed to get memlimit properties for pid %d with error %d", pid, err);
        return;
    }
    
    syslog(LOG_WARNING, "Jetsam configuration: Got active limit %d and inactive limit %d for pid %d", properties.memlimit_active, properties.memlimit_inactive, pid);
}

__attribute__ ((constructor))
static void jetsamConfigurator(void) {
    @autoreleasepool {
        if (NSBundle.mainBundle.bundleIdentifier == nil || ![NSBundle.mainBundle.bundleIdentifier isEqualToString:@"org.coolstar.SafeMode"]) {
            char pathbuf[PROC_PIDPATHINFO_MAXSIZE] = {0};
            pid_t pid = getpid();
            int ret = proc_pidpath(pid, pathbuf, sizeof(pathbuf));
            if (ret > 0) {
                NSString *pathStr = [[NSString stringWithUTF8String:pathbuf] stringByResolvingSymlinksInPath];
                syslog(LOG_WARNING, "Jetsam configuration: Loading for binary %s with pid %d", [pathStr UTF8String], pid);
                jetsamConfiguration(pid);
            }
        }
    }
}
