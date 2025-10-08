#ifndef REVOLUTION_OS_ERROR_H
#define REVOLUTION_OS_ERROR_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void OSReport(const char* format, ...);
extern void OSPanic(const char* fileName, int line, const char* format, ...);

#define OSHalt(msg, line)   OSPanic(__FILE__, line, msg)

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_OS_ERROR_H
