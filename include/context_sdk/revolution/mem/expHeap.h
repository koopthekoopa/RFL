#ifndef REVOLUTION_MEM_EXP_HEAP_H
#define REVOLUTION_MEM_EXP_HEAP_H

#include <revolution/mem/heapHandle.h>

#ifdef __cplusplus
extern "C" {
#endif

MEMiHeapHead*   MEMCreateExpHeapEx(void* startAddress, u32 size, u16 opt);
void*           MEMDestroyExpHeap(MEMiHeapHead* heap);

void*           MEMAllocFromExpHeapEx(MEMiHeapHead* heap, u32 size, s32 align);
inline void* MEMAllocFromExpHeap(MEMiHeapHead* heap, u32 size) {
    return MEMAllocFromExpHeapEx(heap, size, 4);
}

u32             MEMGetAllocatableSizeForExpHeapEx(MEMiHeapHead* heap, int align);
inline u32 MEMGetAllocatableSizeForExpHeap(MEMiHeapHead* heap) {
    return MEMGetAllocatableSizeForExpHeapEx(heap, 4);
}

u32             MEMGetTotalFreeSizeForExpHeap(MEMiHeapHead* heap);

void            MEMFreeToExpHeap(MEMiHeapHead* heap, void* block);

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_MEM_EXP_HEAP_H
