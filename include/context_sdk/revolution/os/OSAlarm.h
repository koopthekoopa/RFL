#ifndef REVOLUTION_OS_ALARM_H
#define REVOLUTION_OS_ALARM_H

#include <revolution/types.h>

#include <revolution/os/OSContext.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSAlarm OSAlarm;

struct OSAlarm {
    void (*handler)(OSAlarm*, OSContext*); // 0x0
    unsigned long tag; // 0x4
    long long fire; // 0x8
    struct OSAlarm * prev; // 0x10
    struct OSAlarm * next; // 0x14
    long long period; // 0x18
    long long start; // 0x20
    void * userData; // 0x28
};

void    OSCreateAlarm(OSAlarm* alarm);
void    OSCancelAlarm(OSAlarm* alarm);

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_OS_ALARM_H
