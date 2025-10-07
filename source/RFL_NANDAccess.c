#include <RVLFaceLib.h>
#include <RVLFaceLibInternal.h>

#include <revolution/os.h>
#include <revolution/nand.h>

#include <string.h>

static const char* scFileNames[2] = { "/shared2/menu/FaceLib/RFL_DB.dat", "/shared2/menu/FaceLib/RFL_Res.dat" };
static const u8 scFilePermissions[2] = {NAND_PERM_ALL_RW, NAND_PERM_ALL_READ };
static const u8 scFileAttributes[2];

static const char* scFirstDirectory = "/shared2/menu";
static const char* scSecondDirectory = "/shared2/menu/FaceLib";

void RFLiInitAccessInfo(MEMiHeapHead* heap) {
    u16 i;

    for (i = 0; i < RFLiFileType_Max; i++) {
        RFLiNANDAccessInfo* info = RFLiGetAccInfo(i);
        memset(info, 0, sizeof(RFLiNANDAccessInfo));

        info->safeBuffer = MEMAllocFromExpHeapEx(heap, 0x2000, 32);
        RFLi_ASSERTLINE_NULL(info->safeBuffer, 123);

        OSCreateAlarm(&info->alarm);
    }
}

void RFLiExitAccessInfo() {
    u16 i;

    for (i = 0; i < RFLiFileType_Max; i++) {
        RFLiNANDAccessInfo* info = RFLiGetAccInfo(i);

        OSCancelAlarm(&info->alarm);
    }
}

BOOL RFLiIsWorking() {
    RFLi_ASSERTLINE(RFLAvailable(), 151);
    return RFLiGetWorking();
}

void RFLiStartWorking() {
    BOOL b;
    RFLi_ASSERTLINE(RFLAvailable(), 170);
    
    b = OSDisableInterrupts();

    RFLiSetWorking(TRUE);
    RFLiGetManager()->mLastErrcode = RFLErrcode_Busy;

    OSRestoreInterrupts(b);
}

static void startWorkingClose_() {
    BOOL b;
    RFLi_ASSERTLINE(RFLAvailable(), 183);

    b = OSDisableInterrupts();

    RFLiSetWorking(TRUE);
    RFLiGetManager()->mBeforeCloseErr = RFLiGetManager()->mLastErrcode;
    RFLiGetManager()->mBeforeCloseReason = RFLiGetManager()->mLastReason;
    RFLiGetManager()->mLastErrcode = RFLErrcode_Busy;

    OSRestoreInterrupts(b);
}

static void endWorkingCloseReason_(RFLErrcode errcode, s32 reason) {
    BOOL b;
    RFLi_ASSERTLINE(RFLAvailable(), 201);

    b = OSDisableInterrupts();

    RFLiSetWorking(FALSE);
    if (errcode == RFLErrcode_Success) {
        RFLiGetManager()->mLastErrcode = RFLiGetManager()->mBeforeCloseErr;
        RFLiGetManager()->mLastReason = RFLiGetManager()->mBeforeCloseErr; // @BUG Should be mBeforeCloseReason
    }
    else {
        RFLiGetManager()->mLastErrcode = errcode;
        RFLiGetManager()->mLastReason = reason;
    }

    OSRestoreInterrupts(b);
}

static void endWorkingClose_(RFLErrcode errcode) {
    endWorkingCloseReason_(errcode, 0);
}

void RFLiEndWorkingReason(RFLErrcode errcode, s32 reason) {
    BOOL b;
    RFLi_ASSERTLINE(RFLAvailable(), 238);

    switch (RFLiGetManager()->mLastErrcode) {
        case RFLErrcode_Busy:
        case RFLErrcode_Success: {
            b = OSDisableInterrupts();

            RFLiSetWorking(FALSE);
            RFLiGetManager()->mLastErrcode = errcode;
            RFLiGetManager()->mLastReason = reason;

            OSRestoreInterrupts(b);
            break;
        }
        default: {
            break;
        }
    }
}

void RFLiEndWorking(RFLErrcode errcode) {
    RFLiEndWorkingReason(errcode, 0);
}
