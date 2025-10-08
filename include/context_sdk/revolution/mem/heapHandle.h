#ifndef REVOLUTION_MEM_HEAP_HANDLE_H
#define REVOLUTION_MEM_HEAP_HANDLE_H

#include <revolution/types.h>
#include <revolution/os/OSMutex.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MEMiHeapHead {
    u32 signature; // 0x0
    struct {
        void * prevObject; // 0x0
        void * nextObject; // 0x4
    } link; // 0x4
    struct {
        void * headObject; // 0x0
        void * tailObject; // 0x4
        u16 numObjects; // 0x8
        u16 offset; // 0xA
    } childList; // 0xC
    void * heapStart; // 0x18
    void * heapEnd; // 0x1C
    struct OSMutex mutex; // 0x20
    union {
        u32 val; // 0x0
        struct {
            u32 _reserved : 24; // 0x0
            u32 optFlag : 8; // 0x0
        } fields; // 0x0
    } attribute; // 0x38
} MEMiHeapHead;

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_MEM_HEAP_HANDLE_H
