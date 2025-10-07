#ifndef RFLi_MODEL_H
#define RFLi_MODEL_H

#include <RFL_Types.h>
#include <internal/RFLi_Types.h>
#include <internal/RFLi_Debug.h>

#include <RFL_Model.h>

#include <revolution/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u8 uOff; // 0x0
    u8 fOff; // 0x1
    u8 rOff; // 0x2
    int uRev; // 0x4
    int fRev; // 0x8
    int rRev; // 0xC
} RFLiCoordinateData;

typedef struct {
    u8 dlNose[192]; // offset 0x0, size 0xC0
    u8 dlCap[1376]; // offset 0xC0, size 0x560
    u8 dlFaceline[736]; // offset 0x620, size 0x2E0
    u8 dlBeard[352]; // offset 0x900, size 0x160
    u8 dlNoseline[96]; // offset 0xA60, size 0x60
    u8 dlMask[896]; // offset 0xAC0, size 0x380
    u8 dlGlass[64]; // offset 0xE40, size 0x40
    u8 texFaceline[16384]; // offset 0xE80, size 0x4000
    u8 texCap[1024]; // offset 0x4E80, size 0x400
    u8 texNoseline[1024]; // offset 0x5280, size 0x400
    u8 texGlass[4096]; // offset 0x5680, size 0x1000
    s16 vtxPosNose[69]; // offset 0x6680, size 0x8A
    s16 vtxNrmNose[69]; // offset 0x670A, size 0x8A
    s16 vtxPosCap[519]; // offset 0x6794, size 0x40E
    s16 vtxNrmCap[738]; // offset 0x6BA2, size 0x5C4
    s16 vtxTxcCap[190]; // offset 0x7166, size 0x17C
    s16 vtxPosFaceline[198]; // offset 0x72E2, size 0x18C
    s16 vtxNrmFaceline[198]; // offset 0x746E, size 0x18C
    s16 vtxTxcFaceline[230]; // offset 0x75FA, size 0x1CC
    s16 vtxPosBeard[120]; // offset 0x77C6, size 0xF0
    s16 vtxNrmBeard[204]; // offset 0x78B6, size 0x198
    s16 vtxPosNoseline[18]; // offset 0x7A4E, size 0x24
    s16 vtxNrmNoseline[6]; // offset 0x7A72, size 0xC
    s16 vtxTxcNoseline[14]; // offset 0x7A7E, size 0x1C
    s16 vtxPosMask[264]; // offset 0x7A9A, size 0x210
    s16 vtxNrmMask[258]; // offset 0x7CAA, size 0x204
    s16 vtxTxcMask[352]; // offset 0x7EAE, size 0x2C0
    s16 vtxPosGlass[12]; // offset 0x816E, size 0x18
    s16 vtxNrmGlass[3]; // offset 0x8186, size 0x6
    s16 vtxTxcGlass[8]; // offset 0x818C, size 0x10
    GXTexObj texObjFaceline; // offset 0x819C, size 0x20
    GXTexObj texObjCap; // offset 0x81BC, size 0x20
    GXTexObj texObjNoseline; // offset 0x81DC, size 0x20
    GXTexObj texObjGlass; // offset 0x81FC, size 0x20
    s16* vtxPosHair; // offset 0x821C, size 0x4
    s16* vtxNrmHair; // offset 0x8220, size 0x4
    u8* dlHair; // offset 0x8224, size 0x4
    s16* vtxPosForehead; // offset 0x8228, size 0x4
    s16* vtxNrmForehead; // offset 0x822C, size 0x4
    u8* dlForehead; // offset 0x8230, size 0x4
    u16 dlSizeNose; // offset 0x8234, size 0x2
    u16 dlSizeFaceline; // offset 0x8236, size 0x2
    u16 dlSizeHair; // offset 0x8238, size 0x2
    u16 dlSizeCap; // offset 0x823A, size 0x2
    u16 dlSizeForehead; // offset 0x823C, size 0x2
    u16 dlSizeBeard; // offset 0x823E, size 0x2
    u16 dlSizeNoseline; // offset 0x8240, size 0x2
    u16 dlSizeMask; // offset 0x8242, size 0x2
    u16 dlSizeGlass; // offset 0x8244, size 0x2
    u8 colorIdxFaceline; // offset 0x8246, size 0x1
    u8 colorIdxHair; // offset 0x8247, size 0x1
    u8 colorIdxBeard; // offset 0x8248, size 0x1
    u8 colorIdxGlass; // offset 0x8249, size 0x1
    u8 colorIdxFavorite; // offset 0x824A, size 0x1
    BOOL flipHair; // offset 0x824C, size 0x4
} RFLiCharModelRes;

typedef struct {
    RFLiPartsShp parts; // offset 0x0, size 0x4
    u16 index; // offset 0x4, size 0x2
    int transform; // offset 0x8, size 0x4
    int flipX; // offset 0xC, size 0x4
    s16 * vtxPosBuf; // offset 0x10, size 0x4
    s16 * vtxNrmBuf; // offset 0x14, size 0x4
    s16 * vtxTxcBuf; // offset 0x18, size 0x4
    u8 * dlBuf; // offset 0x1C, size 0x4
    u16 vtxPosBufSize; // offset 0x20, size 0x2
    u16 vtxNrmBufSize; // offset 0x22, size 0x2
    u16 vtxTxcBufSize; // offset 0x24, size 0x2
    u16 dlBufSize; // offset 0x26, size 0x2
    u16 vtxPosSize; // offset 0x28, size 0x2
    u16 vtxNrmSize; // offset 0x2A, size 0x2
    u16 vtxTxcSize; // offset 0x2C, size 0x2
    u16 dlSize; // offset 0x2E, size 0x2
    f32 posScale; // offset 0x30, size 0x4
    struct {
        f32 x; // offset 0x0, size 0x4
        f32 y; // offset 0x4, size 0x4
        f32 z; // offset 0x8, size 0x4
    } * posTrans; // offset 0x34, size 0x4
    struct {
        f32 x; // offset 0x0, size 0x4
        f32 y; // offset 0x4, size 0x4
        f32 z; // offset 0x8, size 0x4
    } * noseTrans; // offset 0x38, size 0x4
    struct {
        f32 x; // offset 0x0, size 0x4
        f32 y; // offset 0x4, size 0x4
        f32 z; // offset 0x8, size 0x4
    } * beardTrans; // offset 0x3C, size 0x4
    struct {
        f32 x; // offset 0x0, size 0x4
        f32 y; // offset 0x4, size 0x4
        f32 z; // offset 0x8, size 0x4
    } * hairTrans; // offset 0x40, size 0x4
} RFLiCharShapeRes;

extern RFLiCoordinateData coordinateData;

u32     RFLiGetExpressionNum(u32 expressionFlag);

void    RFLiInitCharModel(RFLCharModel* charModel, const RFLiCharInfo* info, void* bufferPtr, RFLResolution resolution, u32 expressionFlag);

GXColor RFLiGetFavoriteColor(const RFLiCharInfo* info);
GXColor RFLiGetFacelineColor(const RFLiCharInfo* info);

void    RFLiInitCharModelRes(RFLiCharModelRes* charModelRes, const RFLiCharInfo* info);
void    RFLiInitShapeRes(RFLiCharShapeRes* arg);
void    RFLiInitTexRes(GXTexObj* texObj, RFLiPartsShpTex parts, u16 index, void* buf);

void    RFLiTransformCoordinate(s16* to, const s16* from);

inline void RFLiSetCoordinateData(const RFLiCoordinateData* data) {
    RFLi_ASSERTLINE_NULL(data, 281);
    coordinateData = *data;
}

static inline RFLiCoordinateData* RFLiGetCoordinateData() {
    return &coordinateData;
}

#ifdef __cplusplus
}
#endif

#endif // RFLi_MODEL_H
