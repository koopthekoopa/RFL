#ifndef RFL_ICON_H
#define RFL_ICON_H

#include <RFL_Types.h>
#include <internal/RFLi_Types.h>

#include <RFL_MiddleDatabase.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RFLIconBG_Favorite = 0,
    RFLIconBG_Direct,
} RFLIconBG;

typedef struct {
    u16 width; // offset 0x0, size 0x2
    u16 height; // offset 0x2, size 0x2
    RFLIconBG bgType; // offset 0x4, size 0x4
    GXColor bgColor; // offset 0x8, size 0x4
    int drawXluOnly; // offset 0xC, size 0x4
} RFLIconSetting;

RFLErrcode  RFLMakeIcon(void* buf, RFLDataSource source, RFLMiddleDB* middleDB, u16 index, RFLExpression expression, const RFLIconSetting* setting);
void        RFLSetIconDrawDoneCallback(RFLSimpleCB cb);

#ifdef __cplusplus
}
#endif

#endif // RFL_ICON_H
