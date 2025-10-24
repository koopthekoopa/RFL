#include <RVLFaceLib.h>
#include <RVLFaceLibInternal.h>

#include <revolution/os.h>

#include <string.h>

u32 RFLGetMiddleDBBufferSize(u16 size) {
    return size * 64;
}

typedef struct {
    u8 sex; // 0x0
    u8 age; // 0x1
    u8 race; // 0x2
    u8 padding; // 0x3
} RFL_MIDDLEDB_MASK;

typedef struct {
    s32 sex : 2; // 0x0
    s32 srcIdx : 15; // 0x0
    s32 dstIdx : 15; // 0x0
} RFL_MIDDLEDB_PARAM;

typedef struct {
    s16 lastSrcIdx; // 0x0
    u16 padding; // 0x2
} RFL_MIDDLEDB_PARAM2;

typedef struct {
    u16 dstIdx; // 0x0
    u8 sex; // 0x2
    u8 padding; // 0x3
} RFL_MIDDLEDB_RAND_PARAM;

void RFLInitMiddleDB(RFLMiddleDB* db, RFLMiddleDBType type, void* buffer, u16 size) {
    RFLiMiddleDB* idb = (RFLiMiddleDB*)db;

    RFLi_ASSERTLINE_NULL(db, 87);
    RFLi_ASSERTLINE_NULL(buffer, 88);

    if (db != NULL && buffer != NULL) {
        idb->type = type;
        idb->callback = NULL;
        idb->size = size;
        idb->data = buffer;
        idb->storedSize = 0;
        idb->userdata1 = 0;
        idb->userdata2 = 0;

        switch (type) {
            case RFLMiddleDBType_Random: {
                RFL_MIDDLEDB_MASK* mask = (RFL_MIDDLEDB_MASK*)&idb->userdata1;

                RFLi_ASSERTLINE(size <= (100) /*RFL_MAX_DATABASE*/, 106);
                if (size > RFL_MAX_DATABASE) {
                    return;
                }

                mask->sex = RFLSex_All;
                mask->age = RFLAge_All;
                mask->race = RFLRace_All;
                break;
            }
            case RFLMiddleDBType_HiddenNewer:
            case RFLMiddleDBType_HiddenOlder: {
                RFL_MIDDLEDB_PARAM* param = (RFL_MIDDLEDB_PARAM*)&idb->userdata1;

                RFLi_ASSERTLINE(size <= (10000) /*RFL_MAX_HIDDEN_DB*/, 120);
                if (size > RFL_MAX_HIDDEN_DB) {
                    return;
                }

                param->srcIdx = -1;
                param->dstIdx = 0;
                break;
            }
            case RFLMiddleDBType_HiddenRandom: {
                RFL_MIDDLEDB_RAND_PARAM* param = (RFL_MIDDLEDB_RAND_PARAM*)&idb->userdata1;

                param->sex = RFLSex_All;
                param->dstIdx = 0;
            }
            default: {
                break;
            }
        }

        memset(idb->data, 0, RFLGetMiddleDBBufferSize(idb->size));
    }
}

static BOOL checkHiddenData_(RFLiHiddenCharData* data) {
    RFLiCharInfo info;

    if (!RFLiIsValidID((RFLCreateID*)&data->createID)) {
        return FALSE;
    }

    RFLiConvertHRaw2Info(data, &info);

    if (RFLiCheckValidInfo(&info) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

static void updateHDBcallback_(u32 arg) {
    RFLiMiddleDB* db = (RFLiMiddleDB*)arg;
    RFL_MIDDLEDB_PARAM* param = (RFL_MIDDLEDB_PARAM*)&db->userdata1;
    
    if (RFLGetAsyncStatus() == RFLErrcode_Success || RFLGetAsyncStatus() == RFLErrcode_Broken) {
        s16 src = -1;
        BOOL bFind;
        RFLiDatabaseManager* manager = RFLiGetDBManager();

        do {
            bFind = FALSE;

            if (db->type == RFLMiddleDBType_HiddenOlder) {
                src = RFLiGetHiddenNext(param->srcIdx);
            }
            else {
                src = RFLiGetHiddenPrev(param->srcIdx);
            }

            if (src != -1 && (param->sex == RFLSex_All || param->sex == manager->mDatabase->hidden.data[src].sex)) {
                bFind = TRUE;
            }
        } while (src != -1 && !bFind);

        if (RFLGetAsyncStatus() != RFLErrcode_Broken && checkHiddenData_(&db->data[db->storedSize])) {
            db->storedSize++;
        }

        param->dstIdx++;

        if (src >= 0 && param->dstIdx < db->size) {
            RFLErrcode err;
            param->srcIdx = (u16)src;

            err = RFLiLoadHiddenDataAsync(&db->data[db->storedSize], param->srcIdx, updateHDBcallback_, (u32)db);
            if (err != RFLErrcode_Busy) {
                RFLiEndWorking(err);
            }
        }
        else {
            RFLiGetManager()->mLastErrcode = db->storedSize < db->size ? RFLErrcode_DBNodata : RFLErrcode_Success;
        }
    }

    if (!RFLiIsWorking()) {
        if (RFLGetAsyncStatus() == RFLErrcode_NANDCommandfail && RFLGetLastReason() == NAND_RESULT_BUSY) {
            RFL_MIDDLEDB_PARAM2* param2 = (RFL_MIDDLEDB_PARAM2*)&db->userdata2;
            param->srcIdx = param2->lastSrcIdx;
            db->storedSize = 0;
            param->dstIdx = 0;
        }

        if (db->callback != NULL) {
            db->callback();
        }
    }
}

static s16 stepOne_(s16 srcIdx, BOOL oldIsHead) {
    RFLiHiddenDBManager* manager = RFLiGetHDBManager();
    s16 ret = -1;

    if (oldIsHead) {
        if (srcIdx < 0) {
            ret = RFLiGetHiddenHeader()->head;
        }
        else {
            ret = RFLiGetHiddenNext(srcIdx);
        }
    }
    else {
        if (srcIdx < 0) {
            ret = RFLiGetHiddenHeader()->tail;
        }
        else {
            ret = RFLiGetHiddenPrev(srcIdx);
        }
    }

    return ret;
}

static void loadHiddenDataSync_(RFLiMiddleDB* db) {
    s16 src = -1;
    BOOL running = TRUE;
    RFL_MIDDLEDB_PARAM* param = (RFL_MIDDLEDB_PARAM*)&db->userdata1;

    while (running) {
        RFLiLoadCachedHiddenData(&db->data[db->storedSize], param->srcIdx);
        if (db->type == RFLMiddleDBType_HiddenOlder) {
            src = RFLiGetHiddenNext(param->srcIdx);
        }
        else {
            src = RFLiGetHiddenPrev(param->srcIdx);
        }

        if (checkHiddenData_(&db->data[db->storedSize])) {
            db->storedSize++;
        }

        param->dstIdx++;

        if (src >= 0 && param->dstIdx < db->size) {
            param->srcIdx = (u16)src;
        }
        else {
            running = FALSE;
        }
    }

    RFLiEndWorking(param->srcIdx == -1 ? RFLErrcode_DBNodata : RFLErrcode_Success);
}

static void updateHiddenOld_(RFLiMiddleDB* db, BOOL oldIsHead, BOOL use_cache) {
    RFL_MIDDLEDB_PARAM* param = (RFL_MIDDLEDB_PARAM*)&db->userdata1;

    if (!RFLiDBIsLoaded()) {
        RFLiEndWorking(RFLErrcode_DBNodata);
        return;
    }

    if (db->data == NULL || db->size == 0 || !RFLiDBIsLoaded()) {
        RFLiEndWorking(RFLErrcode_NotAvailable);
        return;
    }

    {
        s16 src = -1;
        BOOL bFind;
        RFLiDatabaseManager* manager = RFLiGetDBManager();

        do {
            bFind = FALSE;
            src = stepOne_(src, oldIsHead);
            if (src != -1 && (param->sex == RFLSex_All || param->sex == manager->mDatabase->hidden.data[src].sex)) {
                bFind = TRUE;
            }
        } while (src != -1 && !bFind);

        if (src >= 0) {
            param->srcIdx = src;

            if (use_cache) {
                loadHiddenDataSync_(db);
                return;
            }
            else {
                RFLErrcode err = RFLiLoadHiddenDataAsync(&db->data[param->dstIdx], param->srcIdx, updateHDBcallback_, (u32)db); // r25
                if (err != RFLErrcode_Busy) {
                    RFLiEndWorking(err);
                }
                return;
            }
        }

        RFLiEndWorking(RFLErrcode_DBNodata);
    }
}

static void loadHiddenRandomSync_(RFLiMiddleDB* db /* r30 */) {
    BOOL running = TRUE; // r27
    RFL_MIDDLEDB_RAND_PARAM* param = (RFL_MIDDLEDB_RAND_PARAM*)&db->userdata1; // r29

    while (running) {
        u32 src = *(u32*)&db->data[param->dstIdx]; // r28
        if (src != 0) {
            u16 srcIdx = src-1; // r26
            RFLiLoadCachedHiddenData(&db->data[db->storedSize], srcIdx);

            if (checkHiddenData_((RFLiHiddenCharData*)&db->data[db->storedSize])) {
                db->storedSize++;
            }

            param->dstIdx++;

            if (src > 0 && param->dstIdx < db->size) {
                RFLi_ASSERTLINE_RANGE(src, 0, RFL_MAX_HIDDEN_DB, 397);
                if (src >= RFL_MAX_HIDDEN_DB) {
                    running = FALSE;
                }
            }
            else {
                running = FALSE;
            }
        }
        else {
            running = FALSE;
        }
    }

    RFLiGetManager()->mLastErrcode = db->storedSize < db->size ? RFLErrcode_DBNodata : RFLErrcode_Success;
}

static void updateHDBRandcallback_(u32 arg /* r31+0x8 */) {
    RFLiMiddleDB* db = (RFLiMiddleDB*)arg; // r30
    RFL_MIDDLEDB_RAND_PARAM* param = (RFL_MIDDLEDB_RAND_PARAM*)&db->userdata1; // r28

    if (RFLGetAsyncStatus() == RFLErrcode_Success || RFLGetAsyncStatus() == RFLErrcode_Broken) {
        u32* src; // r29

        if (RFLGetAsyncStatus() != RFLErrcode_Broken && checkHiddenData_(&db->data[db->storedSize])) {
            db->storedSize++;
        }

        param->dstIdx++;
        src = (u32*)&db->data[param->dstIdx];

        if (*src > 0 && param->dstIdx < db->size) {
            RFLi_ASSERTLINE_RANGE(*src, 0, RFL_MAX_HIDDEN_DB, 443);
            
            if (*src < RFL_MAX_HIDDEN_DB) {
                u16 srcIdx = *src-1; // r31+0xC
                RFLErrcode err = RFLiLoadHiddenDataAsync(&db->data[db->storedSize], srcIdx, updateHDBRandcallback_, (u32)db); // r31+0x1C

                if (err != RFLErrcode_Busy) {
                    RFLiEndWorking(err);
                }
            }
            else {
                RFLiGetManager()->mLastErrcode = RFLErrcode_Broken;
            }
        }
        else {
            RFLiGetManager()->mLastErrcode = db->storedSize < db->size ? RFLErrcode_DBNodata : RFLErrcode_Success;
        }
    }

    if (!RFLiIsWorking() && db->callback != NULL) {
        if (RFLGetAsyncStatus() == RFLErrcode_NANDCommandfail && RFLGetLastReason() == NAND_RESULT_BUSY) {
            db->storedSize = 0;
            param->dstIdx = 0;
        }

        db->callback();
    }
}

static void updateHiddenRandom_(RFLiMiddleDB* db /* r28 */, BOOL use_cache /* r31+0x8 */) {
    RFLSex sex = (RFLSex)((RFL_MIDDLEDB_RAND_PARAM*)&db->userdata1)->sex; // r31+0x34
    u16 count; // r31+0x16
    u16 max = db->size; // r31+0x14
    u16* array; // r29
    u16 aidx; // r31+0x12
    u16 i; // r30
    u32 rand = OSGetTick(); // r31+0x30

    RFLiStartWorking();

    if (!RFLiDBIsLoaded()) {
        RFLiEndWorking(RFLErrcode_DBNodata);
        return;
    }

    count = RFLiCountupHiddenDataNum(sex);

    if (count == 0) {
        RFLiEndWorking(RFLErrcode_DBNodata);
        return;
    }

    if (count < db->size) {
        max = count;
    }

    array = RFLiAlloc(count * sizeof(*array));
    if (array == NULL) {
        RFLiEndWorking(RFLErrcode_Fatal);
        return;
    }

    aidx = 0;
    for (i = 0; i < RFL_MAX_HIDDEN_DB; i++) {
        if (RFLiIsValidHiddenData(i, sex)) {
            array[aidx] = i;
            aidx++;
        }
    }

    RFLi_ASSERTLINE(aidx == count, 514);

    for (i = 0; i < (count-1); i++) {
        u16 tmp; // r31+0x10
        u16 target; // r31+0xE

        target = (((rand >> 0x10) + rand) & 0xFFFF) % (count-1);
        if (target >= i) {
            target++;
        }

        tmp = array[target];
        array[target] = array[i];
        array[i] = tmp;

        rand = 0x04F8BB63 * (rand + 0x046AC055);
    }

    for (i = 0; i < max; i++) {
        u32* src = (u32*)&db->data[i];
        *src = array[i] + 1;
    }

    RFLiFree(array);

    {
        RFL_MIDDLEDB_RAND_PARAM* param = (RFL_MIDDLEDB_RAND_PARAM*)&db->userdata1; // r31+0x28
        u16 srcIdx = 0; // r31+0xC
        u32* src = (u32*)&db->data[param->dstIdx]; // r31+0x24
        RFLErrcode err; // r31+0x20;

        if (*src == 0) {
            RFLiEndWorking(RFLErrcode_DBNodata);
            return;
        }

        srcIdx = *src-1;
        if (use_cache) {
            loadHiddenRandomSync_(db);
            return;
        }

        err = RFLiLoadHiddenDataAsync(&db->data[param->dstIdx], srcIdx, updateHDBRandcallback_, (u32)db); 
        if (err != RFLErrcode_Busy) {
            RFLiEndWorking(err);
        }
    }
}


// Range: 0xFE0 -> 0x10DC
static void updateRandom_(RFLiMiddleDB* db /* r30 */) {
    int count = 0; // r26
    RFL_MIDDLEDB_MASK* mask = (RFL_MIDDLEDB_MASK*)&db->userdata1; // r29

    RFLiStartWorking();

    while (db->storedSize < db->size) {
        int j; // r28
        BOOL isSame = FALSE; // r27
        RFLiCharInfo info; // r31+0x58

        RFLi_MakeRandomFace(&info, (RFLSex)mask->sex, (RFLAge)mask->age, (RFLRace)mask->race);
        RFLiSetTemporaryID(&info);

        for (j = 0; j < db->storedSize; j++) {
            RFLiCharInfo info2; // r31+0x8
            RFLiConvertHRaw2Info(&db->data[j], &info2);
            if (RFLiIsSameFaceCore(&info, &info2)) {
                isSame = TRUE;
                break;
            }
        }

        if (!isSame) {
            RFLiConvertInfo2HRaw(&info, &db->data[db->storedSize]);
            db->storedSize++;
        }
    }

    RFLiEndWorking(RFLErrcode_Success);
}


static void startUpdateDB_(RFLiMiddleDB* db /* r30 */) {
    db->storedSize = 0;
    memset(db->data, 0, db->size * 64);

    switch (db->type) {
        case RFLMiddleDBType_HiddenNewer:
        case RFLMiddleDBType_HiddenOlder: {
            RFL_MIDDLEDB_PARAM* param = (RFL_MIDDLEDB_PARAM*)&db->userdata1;
            RFL_MIDDLEDB_PARAM2* param2 = (RFL_MIDDLEDB_PARAM2*)&db->userdata2;

            param->dstIdx = 0;
            param2->lastSrcIdx = param->srcIdx;
            break;
        }
        case RFLMiddleDBType_HiddenRandom: {
            RFL_MIDDLEDB_RAND_PARAM* param = (RFL_MIDDLEDB_RAND_PARAM*)&db->userdata1;
            RFL_MIDDLEDB_PARAM2* param2 = (RFL_MIDDLEDB_PARAM2*)&db->userdata2;

            param->dstIdx = 0;
            param2->lastSrcIdx = 0;
        }
        default: {
            break;
        }
    }
}

RFLErrcode RFLiUpdateMiddleDB(RFLMiddleDB* db) {
    RFLErrcode errcode = RFLErrcode_Unknown;
    BOOL use_cache = FALSE;

    if (RFLiIsCachedHDB()) {
        switch (((RFLiMiddleDB*)db)->type) {
            case RFLMiddleDBType_HiddenRandom:
            case RFLMiddleDBType_HiddenNewer:
            case RFLMiddleDBType_HiddenOlder: {
                use_cache = TRUE;
                break;
            }
            default: {
                break;
            }
        }
    }

    if (use_cache) {
        return RFLiUpdateMiddleDBAsync(db, NULL, TRUE);
    }

    errcode = RFLiUpdateMiddleDBAsync(db, NULL, FALSE);
    if (errcode != RFLErrcode_Busy) {
        return errcode;
    }

    return RFLWaitAsync();
}

RFLErrcode RFLUpdateMiddleDBAsync(RFLMiddleDB* db) {
    RFLi_ASSERTLINE_NULL(db, 686);

    if (db == NULL) {
        return RFLErrcode_WrongParam;
    }

    if (!RFLAvailable()) {
        return RFLErrcode_NotAvailable;
    }

    return RFLiUpdateMiddleDBAsync(db, NULL, FALSE);
}

RFLErrcode RFLiUpdateMiddleDBAsync(RFLMiddleDB* db /* r29 */, RFLSimpleCB cb /* r28 */, BOOL use_cache /* r30 */) {
    RFLiMiddleDB* idb = (RFLiMiddleDB*)db; // r31

    RFLi_ASSERTLINE_NULL(db, 711);
    RFLi_ASSERTLINE(!use_cache || (use_cache && RFLiIsCachedHDB()), 712);
    RFLi_ASSERTLINE(!use_cache || (use_cache && cb == 0L), 713);

    if (db == NULL) {
        return RFLErrcode_WrongParam;
    }

    idb->callback = cb;
    startUpdateDB_(idb);

    switch (idb->type) {
        case RFLMiddleDBType_HiddenRandom: {
            updateHiddenRandom_(idb, use_cache);
            break;
        }
        case RFLMiddleDBType_HiddenNewer: {
            updateHiddenOld_(idb, FALSE, use_cache);
            break;
        }
        case RFLMiddleDBType_HiddenOlder: {
            updateHiddenOld_(idb, TRUE, use_cache);
            break;
        }
        case RFLMiddleDBType_Random: {
            updateRandom_(idb);
            break;
        }
    }

    if (!RFLiIsWorking() && idb->callback != NULL) {
        idb->callback();
    }

    return RFLGetAsyncStatus();
}

RFLMiddleDBType RFLGetMiddleDBType(const RFLMiddleDB* db /* r29 */) {
    RFLi_ASSERTLINE_NULL(db, 755);
    return (RFLMiddleDBType)((RFLiMiddleDB*)db)->type;
}

u16 RFLGetMiddleDBSize(const RFLMiddleDB* db /* r29 */) {
    RFLi_ASSERTLINE_NULL(db, 772);
    return (u16)((RFLiMiddleDB*)db)->size;
}

u16 RFLGetMiddleDBStoredSize(const RFLMiddleDB* db /* r29 */) {
    RFLi_ASSERTLINE_NULL(db, 789);
    return (u16)((RFLiMiddleDB*)db)->storedSize;
}

BOOL RFLiGetCharInfoMiddleDB(RFLiCharInfo* info /* r1+0x8 */, const RFLMiddleDB* db /* r28 */, u16 index /* r26 */) {
    RFLiHiddenCharData* data; // r30
    RFLiMiddleDB* idb; // r31

    RFLi_ASSERTLINE_NULL(db, 811);

    if (db == NULL) {
        return FALSE;
    }
    idb = (RFLiMiddleDB*)db;

    if (index >= idb->size) {
        return FALSE;
    }

    if (idb->storedSize == 0) {
        return FALSE;
    }

    data = &idb->data[index];
    if (!RFLiIsValidID((RFLCreateID*)&data->createID)) {
        return FALSE;
    }

    RFLiConvertHRaw2Info(data, info);
    return TRUE;
}

void RFLSetMiddleDBRandomMask(RFLMiddleDB* db /* r28 */, RFLSex sex /* r1+0x8 */, RFLAge age /* r1+0xC */, RFLRace race /* r1+0x10 */) {
    // Local variables
    RFL_MIDDLEDB_MASK* ptr; // r31
    RFLiMiddleDB* idb = (RFLiMiddleDB*)db; // r30

    RFLi_ASSERTLINE(RFLGetMiddleDBType(db) == RFLMiddleDBType_Random, 844);

    if (RFLGetMiddleDBType(db) == RFLMiddleDBType_Random) {
        ptr = (RFL_MIDDLEDB_MASK*)&idb->userdata1;
        ptr->sex = sex;
        ptr->age = age;
        ptr->race = race;
    }
}

void RFLSetMiddleDBHiddenMask(RFLMiddleDB* db /* r21 */, RFLSex sex /* r22 */) {
    RFLiMiddleDB* idb = (RFLiMiddleDB*)db; // r29
    RFLMiddleDBType type = RFLGetMiddleDBType(db); // r30

    RFLi_ASSERTLINE(type == RFLMiddleDBType_HiddenOlder || type == RFLMiddleDBType_HiddenNewer || type == RFLMiddleDBType_HiddenRandom, 873);

    switch (type) {
        case RFLMiddleDBType_HiddenRandom: {
            RFL_MIDDLEDB_RAND_PARAM* ptr = (RFL_MIDDLEDB_RAND_PARAM*)&idb->userdata1;
            ptr->sex = sex;
            break;
        }
        case RFLMiddleDBType_HiddenNewer:
        case RFLMiddleDBType_HiddenOlder: {
            RFL_MIDDLEDB_PARAM* ptr = (RFL_MIDDLEDB_PARAM*)&idb->userdata1;
            ptr->sex = (u8)sex;
        }
        default: {
            break;
        }
    }
}


// Range: 0x1948 -> 0x1A18
void RFLiDeleteMiddleDB(RFLMiddleDB* db, u16 index) {
    RFLiHiddenCharData* data;
    RFLiMiddleDB* idb;

    RFLi_ASSERTLINE_NULL(db, 908);

    if (db != NULL) {
        idb = (RFLiMiddleDB*)db;
        data = &idb->data[index];

        if (index < idb->size && idb->storedSize != 0 && RFLiIsValidID((RFLCreateID*)data->createID)) {
            memset(data, 0, sizeof(*data));
            idb->storedSize--;
        }
    }
}


// Range: 0x1A18 -> 0x1B58
BOOL RFLiPassMiddleDB(RFLMiddleDB* db /* r24 */, u16 passNum /* r31+0x8 */) {
    // Local variables
    BOOL completed = FALSE; // r26
    int i; // r29
    RFLiMiddleDB* idb; // r28
    RFL_MIDDLEDB_PARAM* param; // r27
    RFLiHiddenDBManager* manager = RFLiGetHDBManager(); // r31+0x10

    RFLi_ASSERTLINE_NULL(db, 942);

    if (db == NULL) {
        return FALSE;
    }

    idb = (RFLiMiddleDB*)db;
    param = (RFL_MIDDLEDB_PARAM*)&idb->userdata1;
    for (i = 0; i < passNum; i++) {
        s16 target = -1; // r30

        switch (idb->type) {
            case RFLMiddleDBType_HiddenOlder: {
                target = stepOne_(param->srcIdx, TRUE);
                break;
            }
            case RFLMiddleDBType_HiddenNewer: {
                target = stepOne_(param->srcIdx, FALSE);
                break;
            }
            default: {
                break;
            }
        }

        if (target == -1) {
            completed = TRUE;
            break;
        }

        param->srcIdx = target;
    }

    return completed;
}

RFLErrcode RFLiAddMiddleDBUserData(RFLMiddleDB* db /* r26 */, const RFLCharData* data /* r30 */) {
    RFLiHiddenCharData hdata; // r1+0x8
    RFLiMiddleDB* idb; // r31
    RFLiCharInfo info; // r1+0x48

    RFLi_ASSERTLINE_NULL(db, 989);
    RFLi_ASSERTLINE_NULL(data, 990);

    if (db == NULL) {
        return RFLErrcode_WrongParam;
    }
    if (data == NULL) {
        return RFLErrcode_WrongParam;
    }

    if (!RFLAvailable()) {
        return RFLErrcode_NotAvailable;
    }

    RFLiConvertRaw2HRaw((RFLiCharData*)data, &hdata);

    idb = (RFLiMiddleDB*)db;

    if (idb->type != RFLMiddleDBType_UserSet) {
        return RFLErrcode_NotAvailable;
    }

    if (!RFLiIsValidID((RFLCreateID*)hdata.createID)) {
        return RFLErrcode_Broken;
    }

    RFLiConvertRaw2Info((RFLiCharData*)data, &info);
    if (!RFLiCheckValidInfo(&info)) {
        return RFLErrcode_Broken;
    }

    if (idb->storedSize >= idb->size) {
        return RFLErrcode_DBFull;
    }

    memcpy(&idb->data[idb->storedSize], &hdata, sizeof(RFLiHiddenCharData));
    idb->storedSize++;

    return RFLErrcode_Success;
}

RFLErrcode RFLAddMiddleDBStoreData(RFLMiddleDB* db /* r25 */, const RFLStoreData* data /* r31 */) {
    u16 crc;
    const RFLiStoreData* rawdata;

    RFLi_ASSERTLINE_NULL(db, 1032);
    RFLi_ASSERTLINE_NULL(data, 1033);

    if (db == NULL) {
        return RFLErrcode_WrongParam;
    }
    if (data == NULL) {
        return RFLErrcode_WrongParam;
    }

    if (!RFLAvailable()) {
        return RFLErrcode_NotAvailable;
    }

    crc = RFLiCalculateCRC((void*)data, sizeof(RFLiStoreData));
    if (crc != 0) {
        return RFLErrcode_Broken;
    }

    rawdata = (const RFLiStoreData*)data; // r28
    return RFLiAddMiddleDBUserData(db, (RFLCharData*)rawdata);
}
