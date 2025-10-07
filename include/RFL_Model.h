#ifndef RFL_MODEL_H
#define RFL_MODEL_H

#include <RFL_Types.h>
#include <RFL_MiddleDatabase.h>

#include <revolution/mtx.h>
#include <revolution/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RFLCoordinate_XP = 0x01000000,
    RFLCoordinate_XM = 0xFF000000,
    RFLCoordinate_YP = 0x010000,
    RFLCoordinate_YM = 0xFF0000,
    RFLCoordinate_ZP = 0x0100,
    RFLCoordinate_ZM = 0xFF00,
} RFLCoordinate;

typedef struct {
    u32 dummy[34];
} RFLCharModel;

typedef struct {
    u8 lightEnable; // offset 0x0, size 0x1
    u32 lightMask; // offset 0x4, size 0x4
    GXDiffuseFn diffFn; // offset 0x8, size 0x4
    GXAttnFn attnFn; // offset 0xC, size 0x4
    GXColor ambientColor; // offset 0x10, size 0x4
    u8 zCompLoc; // offset 0x14, size 0x1
} RFLDrawSetting;

typedef struct {
    u8 txcGenNum; // offset 0x0, size 0x1
    GXTexCoordID txcID; // offset 0x4, size 0x4
    GXTexMapID texMapID; // offset 0x8, size 0x4
    u8 tevStageNum; // offset 0xC, size 0x1
    GXTevSwapSel tevSwapTable; // offset 0x10, size 0x4
    GXTevKColorID tevKColorID; // offset 0x14, size 0x4
    GXTevRegID tevOutRegID; // offset 0x18, size 0x4
    GXPosNrmMtx posNrmMtxID; // offset 0x1C, size 0x4
    u8 reverseCulling; // offset 0x20, size 0x1
} RFLDrawCoreSetting;

void            RFLSetCoordinate(RFLCoordinate up, RFLCoordinate front);

u32             RFLGetModelBufferSize(RFLResolution resolution, u32 expressionFlag);

RFLErrcode      RFLInitCharModel(RFLCharModel* charModel, RFLDataSource source, RFLMiddleDB* middleDB, u16 index, void* bufferPtr, RFLResolution resolution, u32 expressionFlag);

void            RFLSetMtx(RFLCharModel* charModel, const Mtx mvMtx);
void            RFLSetMtxPosNrm(RFLCharModel* charModel, const Mtx posMtx,const Mtx nrmMtx);

void            RFLSetExpression(RFLCharModel* charModel, RFLExpression expression);
RFLExpression   RFLGetExpression(const RFLCharModel* charModel);
BOOL            RFLIsAvailableExpression(const RFLCharModel* charModel, RFLExpression expression);

GXColor         RFLGetFavoriteColor(RFLFavoriteColor favoriteColor);

void            RFLDrawOpa(const RFLCharModel* charModel);
void            RFLDrawXlu(const RFLCharModel* charModel);

void            RFLLoadVertexSetting(const RFLDrawCoreSetting* setting);
void            RFLLoadMaterialSetting(const RFLDrawCoreSetting* setting);

void            RFLDrawOpaCore(const RFLCharModel* charModel, const RFLDrawCoreSetting* setting);
void            RFLDrawXluCore(const RFLCharModel* charModel, const RFLDrawCoreSetting* setting);

void            RFLDrawShape(const RFLCharModel* charModel);
void            RFLCopyCharModel(RFLCharModel* dstCharModel, void* dstBufferPtr, RFLCharModel* srcCharModel);

void            RFLSetModelDrawDoneCallback(RFLSimpleCB cb);

#ifdef __cplusplus
}
#endif

#endif // RFL_MODEL_H
