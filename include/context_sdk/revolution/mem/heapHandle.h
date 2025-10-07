#ifndef REVOLUTION_MEM_HEAP_HANDLE_H
#define REVOLUTION_MEM_HEAP_HANDLE_H

#include <revolution/types.h>
#include <revolution/os/OSMutex.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MEMiHeapHead {
    unsigned long signature; // 0x0
    struct {
        void * prevObject; // 0x0
        void * nextObject; // 0x4
    } link; // 0x4
    struct {
        void * headObject; // 0x0
        void * tailObject; // 0x4
        unsigned short numObjects; // 0x8
        unsigned short offset; // 0xA
    } childList; // 0xC
    void * heapStart; // 0x18
    void * heapEnd; // 0x1C
    struct OSMutex mutex; // 0x20
    union {
        unsigned long val; // 0x0
        struct {
            unsigned long _reserved : 24; // 0x0
            unsigned long optFlag : 8; // 0x0
        } fields; // 0x0
    } attribute; // 0x38
} MEMiHeapHead;

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_MEM_HEAP_HANDLE_H
