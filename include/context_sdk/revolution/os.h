#ifndef REVOLUTION_OS_H
#define REVOLUTION_OS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>

#include <revolution/os/OSInterrupt.h>
#include <revolution/os/OSAlarm.h>
#include <revolution/os/OSContext.h>
#include <revolution/os/OSCache.h>
#include <revolution/os/OSMutex.h>
#include <revolution/os/OSThread.h>

#include <revolution/os/OSError.h>

void    OSRegisterVersion(const char* version);

#define OSRoundUp32B(x) (((x) + ((32) - 1)) & ~((32) - 1))

#define OS_CACHED_REGION_PREFIX         0x8000
#define OS_UNCACHED_REGION_PREFIX       0xC000
#define OS_PHYSICAL_MASK                0x3FFF

#define OS_BASE_CACHED                  (OS_CACHED_REGION_PREFIX << 16)
#define OS_BASE_UNCACHED                (OS_UNCACHED_REGION_PREFIX << 16)

u32     __OSBusClock                    ADDRESS(OS_BASE_CACHED + 0x00F8);
u32     __OSCoreClock                   ADDRESS(OS_BASE_CACHED + 0x00FC);

#define OS_BUS_CLOCK                    __OSBusClock
#define OS_CORE_CLOCK                   __OSCoreClock
#define OS_TIMER_CLOCK                  (OS_BUS_CLOCK / 4)

#define OSTicksToCycles(ticks)          (((ticks) *        ((OS_CORE_CLOCK * 2) / OS_TIMER_CLOCK)) / 2)
#define OSTicksToSeconds(ticks)         ((ticks)  /          OS_TIMER_CLOCK)
#define OSTicksToMilliseconds(ticks)    ((ticks)  /         (OS_TIMER_CLOCK     / 1000))
#define OSTicksToMicroseconds(ticks)    (((ticks) * 8)    / (OS_TIMER_CLOCK     / 125000))
#define OSTicksToNanoseconds(ticks)     (((ticks) * 8000) / (OS_TIMER_CLOCK     / 125000))

#define OSSecondsToTicks(s)             ((s)   *  OS_TIMER_CLOCK) 
#define OSMillisecondsToTicks(ms)       ((ms)  * (OS_TIMER_CLOCK / 1000))
#define OSMicrosecondsToTicks(us)       (((us) * (OS_TIMER_CLOCK / 125000)) / 8)
#define OSNanosecondsToTicks(ns)        (((ns) * (OS_TIMER_CLOCK / 125000)) / 8000)

#define OSDiffTick(tick1, tick0)        ((s32)(tick1) - (s32)(tick0))

#define OSSleepSeconds(s)               OSSleepTicks(OSSecondsToTicks((OSTime)s))
#define OSSleepMilliseconds(ms)         OSSleepTicks(OSMillisecondsToTicks((OSTime)ms))
#define OSSleepMicroseconds(us)         OSSleepTicks(OSMicrosecondsToTicks((OSTime)us))
#define OSSleepNanoseconds(ns)          OSSleepTicks(OSNanosecondsToTicks((OSTime)ns))

u32 OSGetTick();
s64 OSGetTime();

#ifdef DEBUG
void* OSPhysicalToCached(u32 paddr);
void* OSPhysicalToUncached(u32 paddr);
u32 OSCachedToPhysical(void* caddr);
u32 OSUncachedToPhysical(void* ucaddr);
void* OSCachedToUncached(void* caddr);
void* OSUncachedToCached(void* ucaddr);
#else
#define OSPhysicalToCached(paddr)    ((void*) ((u32)(OS_BASE_CACHED   + (u32)(paddr))))
#define OSPhysicalToUncached(paddr)  ((void*) ((u32)(OS_BASE_UNCACHED + (u32)(paddr))))
#define OSCachedToPhysical(caddr)    ((u32)   ((u32)(caddr)  - OS_BASE_CACHED))
#define OSUncachedToPhysical(ucaddr) ((u32)   ((u32)(ucaddr) - OS_BASE_UNCACHED))
#define OSCachedToUncached(caddr)    ((void*) ((u8*)(caddr)  + (OS_BASE_UNCACHED - OS_BASE_CACHED)))
#define OSUncachedToCached(ucaddr)   ((void*) ((u8*)(ucaddr) - (OS_BASE_UNCACHED - OS_BASE_CACHED)))
#endif

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_OS_H
