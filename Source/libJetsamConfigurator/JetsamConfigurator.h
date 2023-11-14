//
//  JetsamConfigurator.h
//  libJetsamConfigurator
//
//  Created by Matthew Benedict on 2023/11/13.
//

#ifndef JetsamConfigurator_h
#define JetsamConfigurator_h

#include <sys/sysctl.h>
#include <sys/syslog.h>
#include <Foundation/Foundation.h>

#define PROC_PIDPATHINFO_MAXSIZE  (1024)
int proc_pidpath(pid_t pid, void *buffer, uint32_t buffersize);

// NOTE: Begin copied header from MemoryStatusSPI
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
// NOTE: End copied header from MemoryStatusSPI

#endif /* JetsamConfigurator_h */
