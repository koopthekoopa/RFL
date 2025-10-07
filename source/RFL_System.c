#include <RVLFaceLib.h>
#include <RVLFaceLibInternal.h>

#include <revolution/os.h>
#include <revolution/mem.h>

#include <string.h>

#ifdef RFL_DEBUG
const char* __RFLVersion = "<< RVL_SDK - RFL \tdebug build: Mar  6 2008 17:39:52 (0x4199_60831) >>";
#else
const char* __RFLVersion = "<< RVL_SDK - RFL \trelease build: Mar  6 2008 17:40:04 (0x4199_60831) >>";
#endif

#define RFLi_WORK_SIZE              0x4CF28
#define RFLi_DELUX_WORK_SIZE        0x65F28

#define RFLi_WORK_BUFFER_SIZE       0x4B000
#define RFLi_DELUX_WORK_BUFFER_SIZE 0x64000

static RFLiSysManager*          sRFLManager = NULL;

static RFLErrcode               sRFLLastErrCode = RFLErrcode_NotAvailable;
static s32                      sRFLLastReason = 0;
static u8                       sRFLBrokenType = 0;

static const RFLiCoordinateData scCoordinate = {1, 2, 0, 0, 0, 0};

u32 RFLGetWorkSize(BOOL useDeluxTex) {
    if (useDeluxTex) {
        return RFLi_DELUX_WORK_SIZE;
    }
    else {
        return RFLi_WORK_SIZE;
    }
}

static void* allocSys_(u32 size, int alignment) {
    return MEMAllocFromExpHeapEx(RFLManager->mSystemHeap, size, alignment);
}

RFLErrcode RFLInitResAsync(void* workBuffer, void* resBuffer, u32 resSize, BOOL useDeluxTex) {
    RFLErrcode errcode;

    // Assertz
    RFLi_ASSERTLINE_NULL(resBuffer, 96);
    RFLi_ASSERTLINE_NULL(workBuffer, 97);

    // Must need the RFL resource data
    if (resBuffer == NULL) {
        return RFLErrcode_Fatal;
    }
    else {
        RFLi_REPORT("---------- RVLFaceLib::RFLInit() --------------\n");

        if (RFLManager == NULL) {
            // Start to initialize!!!

            OSRegisterVersion(__RFLVersion);

            // Initialize work buffer
            {
                u32 wholesize = useDeluxTex ? RFLi_DELUX_WORK_BUFFER_SIZE : RFLi_WORK_BUFFER_SIZE;

                RFLi_ASSERTLINE_NULL(workBuffer, 111);
                memset(workBuffer, 0, wholesize);

                sRFLManager = (RFLiSysManager*)workBuffer;
                sRFLLastErrCode = RFLErrcode_NotAvailable;
                sRFLLastReason = 0;
                sRFLBrokenType = 0;

                RFLi_REPORT(" manager   : 0x%08x - 0x%08x (%6dByte)\n", (u8*)sRFLManager, ((u8*)sRFLManager + sizeof(RFLiSysManager)), sizeof(RFLiSysManager));
                RFLManager->mWorkBuffer = ((u8*)workBuffer + sizeof(RFLiSysManager));
            }

            // Initialize root heap
            {
                u32 size;
                if (useDeluxTex) {
                    size = (RFLi_DELUX_WORK_BUFFER_SIZE - sizeof(RFLiSysManager));
                }
                else {
                    size = (RFLi_WORK_BUFFER_SIZE - sizeof(RFLiSysManager));
                }
                RFLManager->mRootHeap = MEMCreateExpHeapEx(RFLManager->mWorkBuffer, size, 1);
                RFLi_REPORT(" rootHeap  : 0x%08x - 0x%08x (%6dByte)\n", (u8*)RFLManager->mWorkBuffer, ((u8*)RFLManager->mWorkBuffer + size), size);
            }

            // Initialize system heap
            {
                u32 size = 0x24800;
                void* buffer = MEMAllocFromExpHeapEx(RFLManager->mRootHeap, size, 32);
                RFLManager->mSystemHeap = MEMCreateExpHeapEx(buffer, size, 1);
                RFLi_REPORT(" systemHeap: 0x%08x - 0x%08x (%6dByte)\n", (u8*)buffer, ((u8*)buffer + size), size);
            }

            // Initialize temporary heap
            {
                u32 size = MEMGetAllocatableSizeForExpHeap(RFLManager->mRootHeap);
                void* buffer = MEMAllocFromExpHeapEx(RFLManager->mRootHeap, size, 8);
                RFLManager->mTmpHeap = MEMCreateExpHeapEx(buffer, size, 1);
                RFLi_REPORT(" tmpHeap   : 0x%08x - 0x%08x (%6dByte)\n", (u8*)buffer, ((u8*)buffer + size), size);
            }

            // Prepare settings
            RFLManager->mLastErrcode = RFLErrcode_Success;
            RFLManager->mBeforeCloseErr = RFLErrcode_Success;
            RFLManager->mLastReason = 0;
            RFLManager->mBeforeCloseReason = 0;
            RFLManager->mUseDeluxTex = useDeluxTex;
            RFLManager->mBrokenTypeList = FALSE;

            // Icon & model
            RFLSetIconDrawDoneCallback(FALSE);
            RFLSetModelDrawDoneCallback(FALSE);
            RFLiSetWorking(FALSE);

            // Prepare database
            RFLiInitDatabase(RFLManager->mSystemHeap);
            RFLiInitLoader();
            RFLiInitCtrlBuf(RFLManager->mSystemHeap);
            RFLiInitHiddenDatabase();
            RFLiInitAccessInfo(RFLManager->mSystemHeap);

            // Prepare coordinates
            RFLiSetCoordinateData(&scCoordinate);

            // Completed!!!!
            RFLi_REPORT(" remain    : sys=%8dByte, tmp=%8dByte\n", MEMGetTotalFreeSizeForExpHeap(RFLManager->mSystemHeap), MEMGetTotalFreeSizeForExpHeap(RFLManager->mTmpHeap));
            RFLi_REPORT(" initialize finished.\n");

            // Setup cache
            if (resBuffer != NULL) {
                RFLiNANDLoader* loader = RFLiGetLoader();
                loader->mCached = TRUE;
                loader->mCacheSize = resSize;
                loader->mCacheBuffer = resBuffer;
            }

            errcode = RFLiBootLoadAsync();
            if (errcode != RFLErrcode_Busy && errcode != RFLErrcode_Success) {
                RFLExit();
            }
        }
        else {
            // we already done that!!
            RFLi_REPORT(" already initialized.\n");
            errcode = RFLErrcode_Success;
        }
    }

    return errcode;
}

RFLErrcode RFLInitRes(void* workBuffer, void* resBuffer, u32 resSize, BOOL useDeluxTex) {
    RFLInitResAsync(workBuffer, resBuffer, resSize, useDeluxTex);
    return RFLWaitAsync();
}

void RFLExit() {
    if (RFLManager) {
        RFLWaitAsync();

        sRFLLastErrCode = RFLGetAsyncStatus();
        sRFLLastReason = RFLGetLastReason();
        sRFLBrokenType = RFLManager->mBrokenTypeList;

        if (RFLIsResourceCached()) {
            RFLFreeCachedResource();
        }

        RFLiExitAccessInfo();

        MEMDestroyExpHeap(RFLManager->mTmpHeap);
        MEMDestroyExpHeap(RFLManager->mSystemHeap);
        MEMDestroyExpHeap(RFLManager->mRootHeap);

        sRFLManager = NULL;

        RFLi_REPORT("---------- NigaoeLibrary::RFLExit() --------------\n");
    }
}

RFLErrcode RFLiBootLoad() {
    RFLiBootLoadAsync();
    return RFLWaitAsync();
}

static void bootloadDB2Res_() {
    RFLiLoadResourceHeaderAsync();

    switch (RFLWaitAsync()) {
        case RFLErrcode_Success:
        case RFLErrcode_Busy:
        case RFLErrcode_Loadfail: {
            break;
        }
        default: {
            RFLExit();
            break;
        }
    }
}

RFLErrcode RFLiBootLoadAsync() {
    return RFLiBootLoadDatabaseAsync(bootloadDB2Res_);
}

BOOL RFLAvailable() {
    return sRFLManager != NULL ? TRUE : FALSE;
}

static void* allocal_(u32 size, int alignment) {
    void* dst = MEMAllocFromExpHeapEx(RFLManager->mTmpHeap, size, alignment);
    RFLi_ASSERTLINE_NULL(dst, 353);
    return dst;
}

void* RFLiAlloc(u32 size) {
    return allocal_(size, 8);
}

void* RFLiAlloc32(u32 size) {
    return allocal_(size, 32);
}

void RFLiFree(void *block)  {
    MEMFreeToExpHeap(RFLManager->mTmpHeap, block);
}

u32 RFLiGetUsedWorkSize() {
    if (RFLAvailable()) {
        MEMiHeapHead* heap = RFLManager->mTmpHeap;
        return MEMGetAllocatableSizeForExpHeap(heap) - MEMGetTotalFreeSizeForExpHeap(heap);
    }
    else {
        return 0;
    }
}

RFLiDatabaseManager* RFLiGetDBManager() {
    if (!RFLAvailable()) {
        return NULL;
    }
    return &RFLManager->mDBMan;
}

RFLiHiddenDBManager* RFLiGetHDBManager() {
    if (!RFLAvailable()) {
        return NULL;
    }
    return &RFLManager->mHDBMan;
}

RFLiNANDLoader* RFLiGetLoader() {
    if (!RFLAvailable()) {
        return NULL;
    }
    return &RFLManager->mLoader;
}

BOOL RFLiGetWorking() {
    if (!RFLAvailable()) {
        return FALSE;
    }
    return RFLManager->mIsWorking;
}

void RFLiSetWorking(BOOL b) {
    RFLManager->mIsWorking = b;
}

RFLiSysManager* RFLiGetManager() {
    return sRFLManager;
}

RFLErrcode RFLGetAsyncStatus() {
    if (!RFLAvailable()) {
        return sRFLLastErrCode;
    }

    if (RFLiIsWorking()) {
        return RFLErrcode_Busy;
    }

    if (RFLiCriticalError()) {
        return RFLErrcode_Fatal;
    }

    return RFLManager->mLastErrcode;
}

s32 RFLGetLastReason() {
    if (!RFLAvailable()) {
        return sRFLLastReason;
    }
    return RFLiGetLastReason();
}

RFLErrcode RFLWaitAsync() {
    volatile RFLErrcode errcode;
    do {
        errcode = RFLGetAsyncStatus();
    } while (errcode == RFLErrcode_Busy);
    return errcode;
}

RFLiNANDAccessInfo* RFLiGetAccInfo(RFLiFileType type) {
    RFLi_ASSERTLINE_RANGE(type, RFLiFileType_Database, RFLiFileType_Max, 596);
    if (!RFLAvailable()) {
        return NULL;
    }
    return &RFLManager->mAccInfo[type];
}

RFLiCtrlManager* RFLiGetCtrlBufManager() {
    if (!RFLAvailable()) {
        return NULL;
    }
    return &RFLManager->mCtrlMan;
}

BOOL RFLiGetUseDeluxTex() {
    if (!RFLAvailable()) {
        return FALSE;
    }
    return RFLManager->mUseDeluxTex;
}

s32 RFLiGetLastReason() {
    if (!RFLAvailable()) {
        return FALSE;
    }
    return RFLManager->mLastReason;
}

BOOL RFLiSetMacAddr(const u8* addr) {
    if (!RFLAvailable()) {
        return FALSE;
    }
    memcpy(RFLManager->macaddr, addr, 6);
    return TRUE;
}

u8* RFLiGetMacAddr() {
    if (!RFLAvailable()) {
        return FALSE;
    }
    return RFLManager->macaddr;
}

void RFLiSetFileBroken(RFLiFileBrokenType type) {
    RFLi_ASSERTLINE_RANGE(type, RFLiFileBrokenType_DBNotFound, RFLiFileBrokenType_Max, 701);
    if (!RFLAvailable()) {
        return;
    }
    RFLManager->mBrokenTypeList |= (u8)(1 << type);
}

BOOL RFLiNotFoundError() {
    u8* list = &sRFLBrokenType;
    if (RFLAvailable()) {
        list = &RFLManager->mBrokenTypeList;
    }
    return (*list & (1 << RFLiFileBrokenType_DBNotFound)) ? TRUE : FALSE;
}

BOOL RFLiNeedRepairError() {
    u8* list = &sRFLBrokenType;
    if (RFLAvailable()) {
        list = &RFLManager->mBrokenTypeList;
    }
    return (*list & (1 << RFLiFileBrokenType_DBBroken)) ? TRUE : FALSE;
}

BOOL RFLiCriticalError() {
    u8* list = &sRFLBrokenType;
    BOOL ret;

    if (RFLAvailable()) {
        list = &RFLManager->mBrokenTypeList;
    }

    ret = (*list & (1 << RFLiFileBrokenType_ResBroken)) || (*list & (1 << RFLiFileBrokenType_Corrupt));
    return ret ? TRUE : FALSE;
}

char* RFLGetArcFilePath() {
    return (char*)"/RFL_Res.dat";
}
