#ifndef RFLi_SYSTEM_H
#define RFLi_SYSTEM_H

#include <RFL_Types.h>
#include <internal/RFLi_Types.h>

#include <internal/RFLi_Database.h>
#include <internal/RFLi_HiddenDatabase.h>

#include <internal/RFLi_NANDLoader.h>
#include <internal/RFLi_NANDAccess.h>

#include <internal/RFLi_Controller.h>

#include <revolution/mem/heapHandle.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* mWorkBuffer; // 0x0
    MEMiHeapHead* mRootHeap; // 0x4
    MEMiHeapHead* mSystemHeap; // 0x8
    MEMiHeapHead* mTmpHeap; // 0xC
    RFLiDatabaseManager mDBMan; // 0x10
    RFLiNANDLoader mLoader; // 0xC0
    RFLiHiddenDBManager mHDBMan; // 0x16C
    RFLiCtrlManager mCtrlMan; // 0x1AAC
    BOOL mIsWorking; // 0x1B34
    BOOL mUseDeluxTex; // 0x1B38
    u8 mBrokenTypeList; // 0x1B3C
    RFLErrcode mLastErrcode; // 0x1B40
    RFLErrcode mBeforeCloseErr; // 0x1B44
    s32 mLastReason; // 0x1B48
    s32 mBeforeCloseReason; // 0x1B4C
    RFLiNANDAccessInfo mAccInfo[2]; // 0x1B50
    RFLSimpleCB mSysCB; // 0x1F10
    RFLSimpleCB mDrawIconCB; // 0x1F14
    RFLSimpleCB mCreateModelCB; // 0x1F18
    u8 macaddr[6]; // 0x1F1C
} RFLiSysManager;

RFLErrcode              RFLiBootLoad();
RFLErrcode              RFLiBootLoadAsync();

RFLiSysManager*         RFLiGetManager();

void*                   RFLiAlloc(u32 size);
void*                   RFLiAlloc32(u32 size);
void                    RFLiFree(void* block);

u32                     RFLiGetUsedWorkSize();

RFLiDatabaseManager*    RFLiGetDBManager();
RFLiHiddenDBManager*    RFLiGetHDBManager();
RFLiNANDLoader*         RFLiGetLoader();

BOOL                    RFLiGetWorking();
void                    RFLiSetWorking(BOOL b);

RFLiSysManager*         RFLiGetManager();

RFLiNANDAccessInfo*     RFLiGetAccInfo(RFLiFileType type);
RFLiCtrlManager*        RFLiGetCtrlBufManager();

BOOL                    RFLiGetUseDeluxTex();
s32                     RFLiGetLastReason();

BOOL                    RFLiSetMacAddr(const u8* addr);
u8*                     RFLiGetMacAddr();

void                    RFLiSetFileBroken(RFLiFileBrokenType type);

BOOL                    RFLiNotFoundError();
BOOL                    RFLiNeedRepairError();
BOOL                    RFLiCriticalError();

#ifdef __cplusplus
}
#endif

#endif // RFLi_SYSTEM_H
