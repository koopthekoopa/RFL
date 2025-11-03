#ifndef REVOLUTION_CARD_H
#define REVOLUTION_CARD_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CARDFileInfo {
    s32 chan; // 0x00
    s32 fileNo; // 0x04
    s32 offset; // 0x08
    s32 length; // 0x0C
    u16 iBlock; // 0x10
    u16 __padding; // 0x12
} CARDFileInfo;

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_CARD_H
