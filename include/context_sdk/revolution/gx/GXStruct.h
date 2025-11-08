/* from wii-ipl */

#ifndef REVOLUTION_GX_STRUCTS_H
#define REVOLUTION_GX_STRUCTS_H

#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXColor {
    u8  r, g, b, a; // 0x00
} GXColor;

typedef struct _GXColorS10 {
    s16 r, g, b, a; // 0x00
} GXColorS10;

typedef struct _GXTexObj {
    u32 dummy[8];   // 0x00
} GXTexObj;

typedef struct _GXLightObj {
    u32 dummy[16];  // 0x00
} GXLightObj;

typedef struct _GXTexRegion {
    u32 dummy[4];   // 0x00
} GXTexRegion;

typedef struct _GXTlutObj {
    u32 dummy[3];   // 0x00
} GXTlutObj;

typedef struct _GXTlutRegion {
    u32 dummy[4];   // 0x00
} GXTlutRegion;

typedef struct GXFifoObj {
    u8 pad[128];    // 0x00
} GXFifoObj;

typedef struct _GXFogAdjTable {
    u16 r[10];  // 0x00
} GXFogAdjTable;

typedef struct _GXVtxDescList {
    GXAttr      attr;   // 0x00
    GXAttrType  type;   // 0x04
} GXVtxDescList;

typedef struct _GXVtxAttrFmtList {
    GXAttr      attr;   // 0x00

    GXCompCnt   cnt;    // 0x04
    GXCompType  type;   // 0x08

    u8          frac;   // 0x0C
} GXVtxAttrFmtList;

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_GX_STRUCTS_H
