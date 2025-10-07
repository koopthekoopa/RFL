#ifndef REVOLUTION_CARD_H
#define REVOLUTION_CARD_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CARDFileInfo {
    s32 chan; // 0x0
    s32 fileNo; // 0x4
    s32 offset; // 0x8
    s32 length; // 0xC
    u16 iBlock; // 0x10
    u16 __padding; // 0x12
} CARDFileInfo;

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_CARD_H
