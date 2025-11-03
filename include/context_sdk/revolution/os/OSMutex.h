#ifndef REVOLUTION_OS_MUTEX_H
#define REVOLUTION_OS_MUTEX_H

#include <revolution/os/OSThread.h>

#ifdef __cplusplus
extern "C" {
#endif

struct OSMutex {
    OSThreadQueue queue; // 0x00
    OSThread* thread; // 0x08
    s32 count; // 0x0C
    OSMutexLink link; // 0x10
};

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_OS_MUTEX_H
