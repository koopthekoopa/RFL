#ifndef REVOLUTION_OS_THREAD_H
#define REVOLUTION_OS_THREAD_H

#include <revolution/types.h>

#include <revolution/os/OSContext.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSThread OSThread;
typedef struct OSMutex  OSMutex;

typedef struct OSThreadLink {
    OSThread * next; // 0x00
    OSThread * prev; // 0x04
} OSThreadLink;

typedef struct OSMutexQueue {
    OSMutex * head; // 0x00
    OSMutex * tail; // 0x04
} OSMutexQueue;

typedef struct OSMutexLink {
    OSMutex * next; // 0x00
    OSMutex * prev; // 0x04
} OSMutexLink;

typedef struct OSThreadQueue {
    OSThread * head; // 0x00
    OSThread * tail; // 0x04
} OSThreadQueue;

struct OSThread {
    OSContext context; // 0x00
    u16 state; // 0x2C8
    u16 attr; // 0x2CA
    long suspend; // 0x2CC
    long priority; // 0x2D0
    long base; // 0x2D4
    void * val; // 0x2D8
    OSThreadQueue * queue; // 0x2DC
    OSThreadLink link; // 0x2E0
    OSThreadQueue queueJoin; // 0x2E8
    OSMutex * mutex; // 0x2F0
    OSMutexQueue queueMutex; // 0x2F4
    OSThreadLink linkActive; // 0x2FC
    u8 * stackBase; // 0x304
    u32 * stackEnd; // 0x308
    long error; // 0x30C
    void * specific[2]; // 0x310
};

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_OS_THREAD_H
