#ifndef PRIVATE_NAND_H
#define PRIVATE_NAND_H

#include <revolution/nand.h>

#ifdef __cplusplus
extern "C" {
#endif

s32     NANDPrivateCreateAsync(const char* path, u8 perm, u8 attr, NANDCallback callback, NANDCommandBlock* block);

s32     NANDPrivateOpen(const char* path, NANDFileInfo* info, u8 accType);
s32     NANDPrivateSafeOpenAsync(const char* path, NANDFileInfo* info, u8 accType, void* buffer, u32 length, NANDCallback callback, NANDCommandBlock* block);

s32     NANDPrivateDeleteAsync(const char* path, NANDCallback callback, NANDCommandBlock* block);

s32     NANDPrivateCreateDirAsync(const char* path, u8 perm, u8 attr, NANDCallback callback, NANDCommandBlock* block);

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_NAND_H
