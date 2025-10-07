#ifndef REVOLUTION_OS_H
#define REVOLUTION_OS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/os/OSInterrupt.h>
#include <revolution/os/OSAlarm.h>
#include <revolution/os/OSContext.h>
#include <revolution/os/OSMutex.h>
#include <revolution/os/OSThread.h>

#include <revolution/os/OSError.h>

void    OSRegisterVersion(const char* version);

#define OSRoundUp32B(x) (((x) + ((32) - 1)) & ~((32) - 1))

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_OS_H
