#ifndef REVOLUTION_GX_STRUCTS_H
#define REVOLUTION_GX_STRUCTS_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXTexObj {
    u32 dummy[8]; // 0x00
} GXTexObj;

typedef struct _GXColor {
    u8 r;   // 0x00
    u8 g;   // 0x01
    u8 b;   // 0x02
    u8 a;   // 0x03
} GXColor;

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_GX_STRUCTS_H
