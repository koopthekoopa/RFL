#ifndef RFLi_MAKE_TEXTURE_H
#define RFLi_MAKE_TEXTURE_H

#include <revolution/types.h>

#include <RFL_Types.h>
#include <internal/RFLi_Types.h>
#include <internal/RFLi_Texture.h>

#include <revolution/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    f32 x; // offset 0x0, size 0x4
    f32 y; // offset 0x4, size 0x4
    f32 width; // offset 0x8, size 0x4
    f32 height; // offset 0xC, size 0x4
    f32 angle; // offset 0x10, size 0x4
    RFLi_ORIGIN origin; // offset 0x14, size 0x4
    RFLTexObj ngtobj; // offset 0x18, size 0x28
} RFLiFacePart;

typedef struct {
    RFLiFacePart eyeR; // offset 0x0, size 0x40
    RFLiFacePart eyeL; // offset 0x40, size 0x40
    RFLiFacePart eyebrowR; // offset 0x80, size 0x40
    RFLiFacePart eyebrowL; // offset 0xC0, size 0x40
    RFLiFacePart mouth; // offset 0x100, size 0x40
    RFLiFacePart mustacheR; // offset 0x140, size 0x40
    RFLiFacePart mustacheL; // offset 0x180, size 0x40
    RFLiFacePart mole; // offset 0x1C0, size 0x40
} RFLiFaceData;

#ifdef __cplusplus
#define rf &
#else
#define rf
#endif
void            RFLiSetupCopyTex(GXTexFmt texFmt, u16 width, u16 height, void* buf, GXColor rf clearColor);
#undef rf

void            RFLiMakeTexture(const RFLiCharInfo* pCharInfo, u8** texBuffer, RFLResolution resolution);

void            RFLiInitFaceParts(const RFLiCharInfo* pCharInfo, RFLiFaceData* pFace);
void            RFLiInitRFLTexture(RFLTexObj* pRFLTexObj);

void            RFLiSetup2DCameraAndParam();

void            RFLiSetTev4Mole();
void            RFLiSetTev4Mouth(u32 mouthColor);
void            RFLiSetTev4Eye(u32 eyeColor, u32 eyeType);
void            RFLiSetTev4Eyebrow(u32 eyebrowColor);
void            RFLiSetTev4Mustache(u32 mustacheColor);

void            RFLiSetFaceParts(const RFLiCharInfo* pCharInfo, RFLiFaceData* pFace, RFLi_MASKRSL resolution);
void            RFLiCapture(u8* texBuffer, const RFLiCharInfo* pCharInfo, RFLiFaceData* pFace, RFLi_MASKRSL resolution);
void            RFLiDrawFaceParts(RFLiFacePart* pParts);

void            RFLiDrawQuad(f32 x, f32 y, f32 width, f32 height, f32 rotZ, RFLi_ORIGIN origin);

RFLi_MASKRSL    RFLiGetMaxMaskRsl(RFLResolution resolution);
u32             RFLiGetMaskSize(RFLi_MASKRSL maskResolution);
u32             RFLiGetMaskBufSize(RFLResolution resolution);

#ifdef __cplusplus
}
#endif

#endif // RFLi_MAKE_TEXTURE_H
