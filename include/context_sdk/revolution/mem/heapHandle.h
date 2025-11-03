#ifndef REVOLUTION_MEM_HEAP_HANDLE_H
#define REVOLUTION_MEM_HEAP_HANDLE_H

#include <revolution/types.h>
#include <revolution/os/OSMutex.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MEMiHeapHead {
    u32 signature; // 0x00
    struct {
        void * prevObject; // 0x00
        void * nextObject; // 0x04
    } link; // 0x04
    struct {
        void * headObject; // 0x00
        void * tailObject; // 0x04
        u16 numObjects; // 0x08
        u16 offset; // 0x0A
    } childList; // 0x0C
    void * heapStart; // 0x18
    void * heapEnd; // 0x1C
    struct OSMutex mutex; // 0x20
    union {
        u32 val; // 0x00
        struct {
            u32 _reserved : 24; // 0x00
            u32 optFlag : 8; // 0x00
        } fields; // 0x00
    } attribute; // 0x38
} MEMiHeapHead;

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_MEM_HEAP_HANDLE_H
