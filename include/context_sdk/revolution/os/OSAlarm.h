#ifndef REVOLUTION_OS_ALARM_H
#define REVOLUTION_OS_ALARM_H

#include <revolution/types.h>

#include <revolution/os/OSContext.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSAlarm OSAlarm;

typedef void (*OSAlarmHandler)(OSAlarm* alarm, OSContext* context);

struct OSAlarm {
    OSAlarmHandler handler; // 0x0
    u32 tag; // 0x4
    long long fire; // 0x8
    struct OSAlarm * prev; // 0x10
    struct OSAlarm * next; // 0x14
    long long period; // 0x18
    long long start; // 0x20
    void * userData; // 0x28
};

void    OSCreateAlarm(OSAlarm* alarm);
void    OSSetAlarm(OSAlarm* alarm, s64 start, OSAlarmHandler handler);
void    OSCancelAlarm(OSAlarm* alarm);

void    OSSetPeriodicAlarm(OSAlarm* alarm, s64 start, s64 period, OSAlarmHandler handler);

void    OSSetAlarmTag(OSAlarm* alarm, u32 tag);

void    OSSetAlarmUserData(OSAlarm* alarm, void* userData);
void*   OSGetAlarmUserData(OSAlarm* alarm);

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_OS_ALARM_H
