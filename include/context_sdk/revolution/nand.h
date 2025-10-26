#ifndef REVOLUTION_NAND_H
#define REVOLUTION_NAND_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NAND_MAX_PATH   64

typedef struct NANDFileInfo {
    s32 fileDescriptor; // 0x0
    s32 origFd; // 0x4
    s8 origPath[NAND_MAX_PATH]; // 0x8
    s8 tmpPath[NAND_MAX_PATH]; // 0x48
    u8 accType; // 0x88
    u8 stage; // 0x89
    u8 mark; // 0x8A
} NANDFileInfo;

typedef struct NANDCommandBlock {
    void* userData; // 0x0
    void* callback; // 0x4
    void* fileInfo; // 0x8
    void* bytes; // 0xC
    void* inodes; // 0x10
    void* status; // 0x14
    u32 ownerId; // 0x18
    u16 groupId; // 0x1C
    u8 nextStage; // 0x1E
    u32 attr; // 0x20
    u32 ownerAcc; // 0x24
    u32 groupAcc; // 0x28
    u32 othersAcc; // 0x2C
    u32 num; // 0x30
    s8 absPath[NAND_MAX_PATH]; // 0x34
    u32* length; // 0x74
    u32* pos; // 0x78
    int state; // 0x7C
    void* copyBuf; // 0x80
    u32 bufLength; // 0x84
    u8* type; // 0x88
    u32 uniqNo; // 0x8C
    u32 reqBlocks; // 0x90
    u32 reqInodes; // 0x94
    u32* answer; // 0x98
    u32 homeBlocks; // 0x9C
    u32 homeInodes; // 0xA0
    u32 userBlocks; // 0xA4
    u32 userInodes; // 0xA8
    u32 workBlocks; // 0xAC
    u32 workInodes; // 0xB0
    const s8** dir; // 0xB4
    int simpleFlag; // 0xB8
} NANDCommandBlock;

typedef void (*NANDCallback)(s32 result, NANDCommandBlock* block);

enum {
    NAND_RESULT_FATAL_ERROR = -128,
    NAND_RESULT_UNKNOWN = -NAND_MAX_PATH,

    NAND_RESULT_MAXDEPTH = -16,
    NAND_RESULT_AUTHENTICATION,
    NAND_RESULT_OPENFD,
    NAND_RESULT_NOTEMPTY,
    NAND_RESULT_NOEXISTS,
    NAND_RESULT_MAXFILES,
    NAND_RESULT_MAXFD,
    NAND_RESULT_MAXBLOCKS,
    NAND_RESULT_INVALID,
    NAND_RESULT_INIT_FAIL,
    NAND_RESULT_EXISTS,
    NAND_RESULT_ECC_CRIT,
    NAND_RESULT_CORRUPT,
    NAND_RESULT_BUSY,
    NAND_RESULT_ALLOC_FAILED,
    NAND_RESULT_ACCESS,

    NAND_RESULT_OK,
};

enum {
    NAND_SEEK_BEG = 0,
    NAND_SEEK_CUR,
    NAND_SEEK_END,
};

enum {
    NAND_ACCESS_NONE = 0,
    NAND_ACCESS_READ = (1 << 0),
    NAND_ACCESS_WRITE = (1 << 1),
    NAND_ACCESS_RW = (NAND_ACCESS_READ | NAND_ACCESS_WRITE)
};

enum {
    NAND_PERM_MASK_BOTH  = 0,
    NAND_PERM_MASK_GROUP = 2,
    NAND_PERM_MASK_USER  = 4,
};

enum {
    NAND_PERM_USER_READ     = (NAND_ACCESS_READ    << NAND_PERM_MASK_USER),
    NAND_PERM_USER_WRITE    = (NAND_ACCESS_WRITE   << NAND_PERM_MASK_USER),
    NAND_PERM_USER          =  NAND_PERM_USER_READ |  NAND_PERM_USER_WRITE,

    NAND_PERM_GROUP_READ    = (NAND_ACCESS_READ     << NAND_PERM_MASK_GROUP),
    NAND_PERM_GROUP_WRITE   = (NAND_ACCESS_WRITE    << NAND_PERM_MASK_GROUP),
    NAND_PERM_GROUP         =  NAND_PERM_GROUP_READ |  NAND_PERM_GROUP_WRITE,

    NAND_PERM_BOTH_READ     = (NAND_ACCESS_READ   << NAND_PERM_MASK_BOTH),
    NAND_PERM_BOTH_WRITE    = (NAND_ACCESS_WRITE  << NAND_PERM_MASK_BOTH),
    NAND_PERM_BOTH          =  NAND_PERM_BOTH_READ | NAND_PERM_BOTH_WRITE,

    NAND_PERM_ALL_READ      = NAND_PERM_USER_READ  | NAND_PERM_GROUP_READ  | NAND_PERM_BOTH_READ,
    NAND_PERM_ALL_WRITE     = NAND_PERM_USER_WRITE | NAND_PERM_GROUP_WRITE | NAND_PERM_BOTH_WRITE,
    NAND_PERM_ALL_RW        = NAND_PERM_ALL_READ   | NAND_PERM_ALL_WRITE
};

/* Main */

s32     NANDRead(NANDFileInfo* info, void* buffer, u32 length);
s32     NANDReadAsync(NANDFileInfo* info, void* buffer, u32 length, NANDCallback callback, NANDCommandBlock* block);

s32     NANDWriteAsync(NANDFileInfo* info, const void* buffer, u32 length, NANDCallback callback, NANDCommandBlock* block);

s32     NANDSeek(NANDFileInfo* info, s32 offset, s32 whence);
s32     NANDSeekAsync(NANDFileInfo* info, s32 offset, s32 whence, NANDCallback callback, NANDCommandBlock* block);

s32     NANDGetLengthAsync(NANDFileInfo* info, u32* length, NANDCallback callback, NANDCommandBlock* block);

s32     NANDClose(NANDFileInfo* info);
s32     NANDSafeCloseAsync(NANDFileInfo* info, NANDCallback callback, NANDCommandBlock* block);

void    NANDSetUserData(NANDCommandBlock* block, void* data);
void*   NANDGetUserData(NANDCommandBlock* block);

/* Private */

s32     NANDPrivateCreateAsync(const char* path, u8 perm, u8 attr, NANDCallback callback, NANDCommandBlock* block);

s32     NANDPrivateOpen(const char* path, NANDFileInfo* info, u8 accType);
s32     NANDPrivateSafeOpenAsync(const char* path, NANDFileInfo* info, u8 accType, void* buffer, u32 length, NANDCallback callback, NANDCommandBlock* block);

s32     NANDPrivateDeleteAsync(const char* path, NANDCallback callback, NANDCommandBlock* block);

s32     NANDPrivateCreateDirAsync(const char* path, u8 perm, u8 attr, NANDCallback callback, NANDCommandBlock* block);

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_NAND_H
