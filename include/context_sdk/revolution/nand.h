#ifndef REVOLUTION_NAND_H
#define REVOLUTION_NAND_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NAND_MAX_PATH 64

typedef struct NANDFileInfo {
    s32 fileDescriptor;          // 0x00
    s32 origFd;                  // 0x04
    s8 origPath[NAND_MAX_PATH];  // 0x08
    s8 tmpPath[NAND_MAX_PATH];   // 0x48
    u8 accType;                  // 0x88
    u8 stage;                    // 0x89
    u8 mark;                     // 0x8A
} NANDFileInfo;

typedef struct NANDCommandBlock {
    void* userData;             // 0x00
    void* callback;             // 0x04
    void* fileInfo;             // 0x08
    void* bytes;                // 0x0C
    void* inodes;               // 0x10
    void* status;               // 0x14
    u32 ownerId;                // 0x18
    u16 groupId;                // 0x1C
    u8 nextStage;               // 0x1E
    u32 attr;                   // 0x20
    u32 ownerAcc;               // 0x24
    u32 groupAcc;               // 0x28
    u32 othersAcc;              // 0x2C
    u32 num;                    // 0x30
    s8 absPath[NAND_MAX_PATH];  // 0x34
    u32* length;                // 0x74
    u32* pos;                   // 0x78
    int state;                  // 0x7C
    void* copyBuf;              // 0x80
    u32 bufLength;              // 0x84
    u8* type;                   // 0x88
    u32 uniqNo;                 // 0x8C
    u32 reqBlocks;              // 0x90
    u32 reqInodes;              // 0x94
    u32* answer;                // 0x98
    u32 homeBlocks;             // 0x9C
    u32 homeInodes;             // 0xA0
    u32 userBlocks;             // 0xA4
    u32 userInodes;             // 0xA8
    u32 workBlocks;             // 0xAC
    u32 workInodes;             // 0xB0
    const s8** dir;             // 0xB4
    int simpleFlag;             // 0xB8
} NANDCommandBlock;

typedef void (*NANDCallback)(s32 result, NANDCommandBlock* block);

// Error codes
#define NAND_RESULT_OK 0
#define NAND_RESULT_ACCESS -1
#define NAND_RESULT_ALLOC_FAILED -2
#define NAND_RESULT_BUSY -3
#define NAND_RESULT_CORRUPT -4
#define NAND_RESULT_ECC_CRIT -5
#define NAND_RESULT_EXISTS -6
#define NAND_RESULT_UNUSED_7 -7 /* Unused by the library, but used by some apps such as the Wii Menu */
#define NAND_RESULT_INVALID -8
#define NAND_RESULT_MAXBLOCKS -9
#define NAND_RESULT_MAXFD -10
#define NAND_RESULT_MAXFILES -11
#define NAND_RESULT_NOEXISTS -12
#define NAND_RESULT_NOTEMPTY -13
#define NAND_RESULT_OPENFD -14
#define NAND_RESULT_AUTHENTICATION -15
#define NAND_RESULT_MAXDEPTH -16
#define NAND_RESULT_UNKNOWN -64
#define NAND_RESULT_FATAL_ERROR -128

// Whence types
#define NAND_SEEK_BEG 0
#define NAND_SEEK_CUR 1
#define NAND_SEEK_END 2

// Access types
#define NAND_ACCESS_NONE 0
#define NAND_ACCESS_READ (1 << 0)
#define NAND_ACCESS_WRITE (1 << 1)
#define NAND_ACCESS_RW (NAND_ACCESS_READ | NAND_ACCESS_WRITE)

// Read/write by owner
#define NAND_PERM_USER_READ (NAND_ACCESS_READ << 4)
#define NAND_PERM_USER_WRITE (NAND_ACCESS_WRITE << 4)
#define NAND_PERM_USER_RW (NAND_PERM_USER_READ | NAND_PERM_USER_WRITE)

// Read/write by group
#define NAND_PERM_GROUP_READ (NAND_ACCESS_READ << 2)
#define NAND_PERM_GROUP_WRITE (NAND_ACCESS_WRITE << 2)
#define NAND_PERM_GROUP_RW (NAND_PERM_GROUP_READ | NAND_PERM_GROUP_WRITE)

// Read/write by other
#define NAND_PERM_BOTH_READ (NAND_ACCESS_READ << 0)
#define NAND_PERM_BOTH_WRITE (NAND_ACCESS_WRITE << 0)
#define NAND_PERM_BOTH_RW (NAND_PERM_BOTH_READ | NAND_PERM_BOTH_WRITE)

// Read/write by all
#define NAND_PERM_ALL_READ (NAND_PERM_USER_READ | NAND_PERM_GROUP_READ | NAND_PERM_BOTH_READ)
#define NAND_PERM_ALL_WRITE (NAND_PERM_USER_WRITE | NAND_PERM_GROUP_WRITE | NAND_PERM_BOTH_WRITE)
#define NAND_PERM_ALL_RW (NAND_PERM_ALL_READ | NAND_PERM_ALL_WRITE)

/* Main */

s32 NANDRead(NANDFileInfo* info, void* buffer, u32 length);
s32 NANDReadAsync(NANDFileInfo* info, void* buffer, u32 length, NANDCallback callback, NANDCommandBlock* block);

s32 NANDWriteAsync(NANDFileInfo* info, const void* buffer, u32 length, NANDCallback callback, NANDCommandBlock* block);

s32 NANDSeek(NANDFileInfo* info, s32 offset, s32 whence);
s32 NANDSeekAsync(NANDFileInfo* info, s32 offset, s32 whence, NANDCallback callback, NANDCommandBlock* block);

s32 NANDGetLengthAsync(NANDFileInfo* info, u32* length, NANDCallback callback, NANDCommandBlock* block);

s32 NANDClose(NANDFileInfo* info);
s32 NANDSafeCloseAsync(NANDFileInfo* info, NANDCallback callback, NANDCommandBlock* block);

void NANDSetUserData(NANDCommandBlock* block, void* data);
void* NANDGetUserData(NANDCommandBlock* block);

s32 NANDPrivateCreateAsync(const char* path, u8 perm, u8 attr, NANDCallback callback, NANDCommandBlock* block);

s32 NANDPrivateOpen(const char* path, NANDFileInfo* info, u8 accType);
s32 NANDPrivateSafeOpenAsync(const char* path, NANDFileInfo* info, u8 accType, void* buffer, u32 length, NANDCallback callback, NANDCommandBlock* block);

s32 NANDPrivateDeleteAsync(const char* path, NANDCallback callback, NANDCommandBlock* block);

s32 NANDPrivateCreateDirAsync(const char* path, u8 perm, u8 attr, NANDCallback callback, NANDCommandBlock* block);

#ifdef __cplusplus
}
#endif

#endif  // REVOLUTION_NAND_H
