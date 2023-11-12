#include <sys/sysctl.h>
#include <sys/syslog.h>
#include <Foundation/Foundation.h>

#define PROC_PIDPATHINFO_MAXSIZE  (1024)
int proc_pidpath(pid_t pid, void *buffer, uint32_t buffersize);
int memorystatus_control(uint32_t command, int32_t pid, uint32_t flags, void *buffer, size_t buffersize);

#if __has_include(<System/sys/kern_memorystatus.h>)
#include <System/sys/kern_memorystatus.h>
#else
typedef struct memorystatus_memlimit_properties {
    int32_t memlimit_active;                /* jetsam memory limit (in MB) when process is active */
    uint32_t memlimit_active_attr;
    int32_t memlimit_inactive;              /* jetsam memory limit (in MB) when process is inactive */
    uint32_t memlimit_inactive_attr;
} memorystatus_memlimit_properties_t;

typedef struct memorystatus_priority_properties {
    int32_t  priority;
    uint64_t user_data;
} memorystatus_priority_properties_t;

#define JETSAM_PRIORITY_FOREGROUND_SUPPORT 9

#define MEMORYSTATUS_CMD_SET_PRIORITY_PROPERTIES 2
#define MEMORYSTATUS_CMD_SET_MEMLIMIT_PROPERTIES 7
#define MEMORYSTATUS_CMD_GET_MEMLIMIT_PROPERTIES 8
#define MEMORYSTATUS_CMD_SET_PROCESS_IS_FREEZABLE 18
#define MEMORYSTATUS_CMD_GET_PROCESS_IS_FREEZABLE 19
#endif

// This is the same as bmalloc::jetsamConfiguration.
// We reimplement it here so we don't need to import the other one.
memorystatus_memlimit_properties_t jetsamConfigurationXPC(pid_t pid) {
    memorystatus_memlimit_properties_t properties;
    memorystatus_priority_properties_t priority;
    
    // Use our privilege to change jetsam limit at the last second
    memset(&properties, 0, sizeof(memorystatus_memlimit_properties_t));
    memset(&priority, 0, sizeof(memorystatus_priority_properties_t));
    properties.memlimit_active = 840;
    properties.memlimit_inactive = 840;
    priority.priority = JETSAM_PRIORITY_FOREGROUND_SUPPORT;
    
    if (pid == 0) {
        syslog(LOG_WARNING, "CyberKit XPC cannot configure for pid 0");
        return properties;
    }

    int err;
    if ((err = memorystatus_control(MEMORYSTATUS_CMD_SET_MEMLIMIT_PROPERTIES, pid, 0, &properties, sizeof(properties)))) {
        syslog(LOG_ERR, "CyberKit XPC crash: Failed to set memlimit properties for pid %d with error %d", pid, err);
        return properties;
    }

    if ((err = memorystatus_control(MEMORYSTATUS_CMD_SET_PRIORITY_PROPERTIES, pid, 0, &priority, sizeof(priority)))) {
        syslog(LOG_ERR, "CyberKit XPC crash: Failed to set priority properties for pid %d with error %d", pid, err);
        return properties;
    }
    
    // Get jetsam limit from system
    memset(&properties, 0, sizeof(memorystatus_memlimit_properties_t));
    if ((err = memorystatus_control(MEMORYSTATUS_CMD_GET_MEMLIMIT_PROPERTIES, pid, 0, &properties, sizeof(properties)))) {
        syslog(LOG_ERR, "CyberKit XPC crash: Failed to get memlimit properties for pid %d with error %d", pid, err);
        return properties;
    }
    
    syslog(LOG_WARNING, "CyberKit XPC found active limit %d and inactive limit %d for pid %d", properties.memlimit_active, properties.memlimit_inactive, pid);
    return properties;
}

__attribute__ ((constructor))
static void cyberkitjetsam(void) {
    @autoreleasepool {
        if (NSBundle.mainBundle.bundleIdentifier == nil || ![NSBundle.mainBundle.bundleIdentifier isEqualToString:@"org.coolstar.SafeMode"]) {
            char pathbuf[PROC_PIDPATHINFO_MAXSIZE] = {0};
            pid_t pid = getpid();
            int ret = proc_pidpath(pid, pathbuf, sizeof(pathbuf));
            if (ret > 0) {
                NSString *pathStr = [[NSString stringWithUTF8String:pathbuf] stringByResolvingSymlinksInPath];
                syslog(LOG_WARNING, "CyberKit XPC: Loading for binary %s with pid %d", [pathStr UTF8String], pid);
                jetsamConfigurationXPC(pid);
            }
        }
    }
}
