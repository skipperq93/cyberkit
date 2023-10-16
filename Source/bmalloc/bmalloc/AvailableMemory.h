/*
 * Copyright (C) 2012-2017 Apple Inc. All rights reserved.
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

#pragma once

#include "BPlatform.h"
#include "Sizes.h"
#if BPLATFORM(IOS_FAMILY)
#include "MemoryStatusSPI.h"
#endif

namespace bmalloc {

BEXPORT size_t availableMemory();

#if BPLATFORM(IOS_FAMILY) || BOS(LINUX) || BOS(FREEBSD)
struct MemoryStatus {
    MemoryStatus(size_t memoryFootprint, double percentAvailableMemoryInUse)
        : memoryFootprint(memoryFootprint)
        , percentAvailableMemoryInUse(percentAvailableMemoryInUse)
    {
    }

    size_t memoryFootprint;
    double percentAvailableMemoryInUse;
};

inline static memorystatus_memlimit_properties_t jetsamConfiguration(pid_t pid)
{
    memorystatus_memlimit_properties_t properties;
    memorystatus_priority_properties_t priority;
    
    // Use our privilege to change jetsam limit at the last second
    memset(&properties, 0, sizeof(memorystatus_memlimit_properties_t));
    memset(&priority, 0, sizeof(memorystatus_priority_properties_t));
    properties.memlimit_active = 840;
    properties.memlimit_inactive = 840;
    priority.priority = JETSAM_PRIORITY_FOREGROUND_SUPPORT;

    if (memorystatus_control(MEMORYSTATUS_CMD_SET_MEMLIMIT_PROPERTIES, pid, 0, &properties, sizeof(properties))) {
        fprintf(stderr, "CyberKit crash: Failed to set memlimit properties for pid %d", pid);
        abort();
    }

    if (memorystatus_control(MEMORYSTATUS_CMD_SET_PRIORITY_PROPERTIES, pid, 0, &priority, sizeof(priority))) {
        fprintf(stderr, "CyberKit crash: Failed to set priority properties for pid %d", pid);
        abort();
    }
    
    // Get jetsam limit from system
    memset(&properties, 0, sizeof(memorystatus_memlimit_properties));
    if (memorystatus_control(MEMORYSTATUS_CMD_GET_MEMLIMIT_PROPERTIES, pid, 0, &properties, sizeof(properties))) {
        fprintf(stderr, "CyberKit crash: Failed to get memlimit properties for pid %d", pid);
        abort();
    }
    return properties;
}

BEXPORT MemoryStatus memoryStatus();

inline size_t memoryFootprint()
{
    auto memoryUse = memoryStatus();
    return memoryUse.memoryFootprint;
}

inline double percentAvailableMemoryInUse()
{
    auto memoryUse = memoryStatus();
    return memoryUse.percentAvailableMemoryInUse;
}
#endif

inline bool isUnderMemoryPressure()
{
#if BPLATFORM(IOS_FAMILY) || BOS(LINUX) || BOS(FREEBSD)
    return percentAvailableMemoryInUse() > memoryPressureThreshold;
#else
    return false;
#endif
}
    
} // namespace bmalloc
