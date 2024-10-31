/*
 * Copyright (C) 2019-2024 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include <WebKit/WKMain.h>
#include <sys/syslog.h>
#include <sys/param.h>

#define PROC_PIDPATHINFO_MAXSIZE  (1024)
extern "C" {
int proc_pidpath(pid_t pid, void *buffer, uint32_t buffersize);

// NOTE: Begin copied header from MemoryStatusSPI
int memorystatus_control(uint32_t command, int32_t pid, uint32_t flags, void *buffer, size_t buffersize);
}

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
// NOTE: End copied header from MemoryStatusSPI

static void jetsamConfiguration(pid_t pid) {
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
    char pathbuf[PROC_PIDPATHINFO_MAXSIZE] = {0};
    pid_t pid = getpid();
    if (proc_pidpath(pid, pathbuf, sizeof(pathbuf)) > 0) {
        char* resolved = NULL;
        realpath(pathbuf, resolved);
        syslog(LOG_WARNING, "Jetsam configuration: Loading for binary %s with pid %d", resolved, pid);
        free(resolved);
        jetsamConfiguration(pid);
    } else {
        syslog(LOG_WARNING, "Jetsam configuration might run. PID obtained is %d", getpid());
    }
}

typedef int (*Main)(int, const char**, const char**, const char**);
int main(int argc, const char** argv, const char**, const char** darwinEnvp)
{
    // Setup
    CFURLRef bundle = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFURLRef xpcServices = CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault, bundle);
    CFURLRef url = CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault, xpcServices);
    CFBundleRef webKit = CFBundleCreate(kCFAllocatorDefault, url);
    CFRelease(bundle);
    CFRelease(xpcServices);
    CFRelease(url);
    
    // Dynamically link and load main function
    Main WKXPCServiceMain = (Main)CFBundleGetFunctionPointerForName(webKit, CFSTR("WKXPCServiceMain"));
    if (WKXPCServiceMain == NULL)
        abort();
    int ret = WKXPCServiceMain(argc, argv, nullptr, darwinEnvp);
    CFRelease(webKit);
    return ret;
}
