#ifndef RFL_MIDDLE_DATABASE_H
#define RFL_MIDDLE_DATABASE_H

#include <RFL_Types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RFLMiddleDBType_HiddenRandom = 0,
    RFLMiddleDBType_HiddenNewer,
    RFLMiddleDBType_HiddenOlder,
    RFLMiddleDBType_Random,
    RFLMiddleDBType_UserSet,
    RFLMiddleDBType_Reserved1,
} RFLMiddleDBType;

typedef struct {
    u8 data[24]; // 0x0
} RFLMiddleDB;

u32             RFLGetMiddleDBBufferSize(u16 size);

void            RFLInitMiddleDB(RFLMiddleDB* db, RFLMiddleDBType type, void* buffer, u16 size);
RFLErrcode      RFLUpdateMiddleDBAsync(RFLMiddleDB* db);

RFLMiddleDBType RFLGetMiddleDBType(const RFLMiddleDB* db);
u16             RFLGetMiddleDBSize(const RFLMiddleDB* db);
u16             RFLGetMiddleDBStoredSize(const RFLMiddleDB* db);

void            RFLSetMiddleDBRandomMask(RFLMiddleDB* db, RFLSex sex, RFLAge age, RFLRace race);
void            RFLSetMiddleDBHiddenMask(RFLMiddleDB* db, RFLSex sex);

RFLErrcode      RFLAddMiddleDBStoreData(RFLMiddleDB* db, const RFLCharData* data);

#ifdef __cplusplus
}
#endif

#endif // RFL_MIDDLE_DATABASE_H
