#ifndef REVOLUTION_WPAD_H
#define REVOLUTION_WPAD_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WPAD_ERR_OK             0
#define WPAD_ERR_NO_CONTROLLER  -1
#define WPAD_ERR_TRANSFER       -3

#define WPAD_MAX_CONTROLLERS    4

typedef void (*WPADCallback)(s32 chan, s32 result);

s32 WPADProbe(s32 chan, u32* pDevType);

s32 WPADReadFaceData(s32 chan, void* dst, u16 size, u16 src, WPADCallback cb);
s32 WPADWriteFaceData(s32 chan, void *dst, u16 size, u16 src, WPADCallback cb);

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_WPAD_H
