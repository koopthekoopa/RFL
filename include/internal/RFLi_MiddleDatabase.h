#ifndef RFLi_MIDDLE_DATABASE_H
#define RFLi_MIDDLE_DATABASE_H

#include <internal/RFLi_Types.h>
#include <RFL_Types.h>

#include <RFL_MiddleDatabase.h>

#ifdef __cplusplus
extern "C" {
#endif

RFLErrcode  RFLiUpdateMiddleDB(RFLMiddleDB* db);
RFLErrcode  RFLiUpdateMiddleDBAsync(RFLMiddleDB* db, RFLSimpleCB cb, BOOL use_cache);

BOOL        RFLiGetCharInfoMiddleDB(RFLiCharInfo* info, const RFLMiddleDB* db, u16 index);

RFLErrcode  RFLiDeleteMiddleDB(RFLMiddleDB* db, u16 index);
RFLErrcode  RFLiPassMiddleDB(RFLMiddleDB* db, u16 passNum);

RFLErrcode  RFLiAddMiddleDBUserData(RFLMiddleDB* db, const RFLCharData* data);

#ifdef __cplusplus
}
#endif

#endif // RFLi_MIDDLE_DATABASE_H
