#include <RVLFaceLib.h>
#include <RVLFaceLibInternal.h>

#include <string.h>

#define RFL_CTRL_SIGNATURE  'RNCD'

static void clearDatabase_(RFLiCtrlBuffer* buffer) {
    int i;

    buffer->mIdentifier = RFL_CTRL_SIGNATURE;
    buffer->mSecretFlag = FALSE;
    buffer->mDeleted = FALSE;
    buffer->mCheckSum = 0;

    for (i = 0; i < RFL_MAX_CTRL_BUFFER; i++) {
        memset(&buffer->mData[i], 0, sizeof(RFLiCharData));
    }
}

void RFLiInitCtrlBuf(MEMiHeapHead* sysHeap) {
    int i;
    RFLiCtrlManager* manager = RFLiGetCtrlBufManager();
    

    for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        manager->mBuffer[i] = MEMAllocFromExpHeapEx(sysHeap, sizeof(RFLiCtrlBuffer), RFL_BUFFER_ALIGN);
        clearDatabase_(manager->mBuffer[i]);

        manager->mIsLoaded[i] = FALSE;
    }

    manager->mReplaceBuf[0] = MEMAllocFromExpHeapEx(sysHeap, sizeof(RFLiCtrlBuffer), RFL_BUFFER_ALIGN);
    clearDatabase_(manager->mReplaceBuf[0]);

    manager->mReplaceBuf[1] = MEMAllocFromExpHeapEx(sysHeap, sizeof(RFLiCtrlBuffer), RFL_BUFFER_ALIGN);
    clearDatabase_(manager->mReplaceBuf[1]);

    manager->mReadBuffer = NULL;
    manager->mWriteBuffer = NULL;
    manager->mWriteChannel = -1;
    manager->mIsRead = TRUE;
    manager->mVerifyBuffer = NULL;

    {
        void* buffer = MEMAllocFromExpHeapEx(sysHeap, RFLGetMiddleDBBufferSize(RFL_MAX_CTRL_BUFFER), RFL_BUFFER_ALIGN);
        RFLInitMiddleDB(&manager->mHiddenMDB, RFLMiddleDBType_HiddenRandom, buffer, RFL_MAX_CTRL_BUFFER);
    }
}
