#ifndef RFLi_MIDDLE_DATABASE_H
#define RFLi_MIDDLE_DATABASE_H

#include <internal/RFLi_Types.h>
#include <RFL_Types.h>

#include <RFL_MiddleDatabase.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    RFLMiddleDBType type; // offset 0x0, size 0x4
    RFLSimpleCB callback; // offset 0x4, size 0x4
    RFLiHiddenCharData* data; // offset 0x8, size 0x4
    u16 size; // offset 0xC, size 0x2
    u16 storedSize; // offset 0xE, size 0x2
    u32 userdata1; // offset 0x10, size 0x4
    u32 userdata2; // offset 0x14, size 0x4
} RFLiMiddleDB;

RFLErrcode  RFLiUpdateMiddleDB(RFLMiddleDB* db);
RFLErrcode  RFLiUpdateMiddleDBAsync(RFLMiddleDB* db, RFLSimpleCB cb, BOOL use_cache);

BOOL        RFLiGetCharInfoMiddleDB(RFLiCharInfo* info, const RFLMiddleDB* db, u16 index);

void        RFLiDeleteMiddleDB(RFLMiddleDB* db, u16 index);
BOOL        RFLiPassMiddleDB(RFLMiddleDB* db, u16 passNum);

RFLErrcode  RFLiAddMiddleDBUserData(RFLMiddleDB* db, const RFLCharData* data);

#ifdef __cplusplus
}
#endif

#endif // RFLi_MIDDLE_DATABASE_H
