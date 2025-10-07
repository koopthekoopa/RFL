#include <RVLFaceLib.h>
#include <RVLFaceLibInternal.h>

#include <string.h>

static const u8 RFLi_EYE_ROT_OFFSET[50] = {
    0x1D, 0x1C, 0x1C, 0x1C, 0x1D, 0x1C, 0x1C, 0x1C,
    0x1D, 0x1C, 0x1C, 0x1C, 0x1C, 0x1D, 0x1D, 0x1C,
    0x1C, 0x1C, 0x1D, 0x1D, 0x1C, 0x1D, 0x1C, 0x1D,
	0x1D, 0x1C, 0x1D, 0x1C, 0x1C, 0x1D, 0x1C, 0x1C,
    0x1C, 0x1D, 0x1D, 0x1D, 0x1C, 0x1C, 0x1D, 0x1D,
    0x1D, 0x1C, 0x1C, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D,
    0x1C, 0x1C
};
static const u8 RFLi_EYEBROW_ROT_OFFSET[24] = {
    0x1A, 0x1A, 0x1B, 0x19, 0x1A, 0x19, 0x1A, 0x19,
    0x1C, 0x19, 0x1A, 0x18, 0x1B, 0x1B, 0x1A, 0x1A,
    0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1A, 0x19, 0x1B
};

#define CHECK_BOOL(name, var)       if (!(var))          { RFLi_REPORT("Invalid "name" data."); return FALSE; }
BOOL RFLiCheckValidInfo(const RFLiCharInfo* info) {
    CHECK_BOOL("FACELINE",      info->faceline.type <= 7);
    CHECK_BOOL("FACELINE",      info->faceline.color <= 5);
    CHECK_BOOL("FACELINE",      info->faceline.texture <= 11);

    CHECK_BOOL("HAIR",          info->hair.type <= 71);
    CHECK_BOOL("HAIR",          info->hair.color <= 7);
    CHECK_BOOL("HAIR",          info->hair.flip <= 1);

    CHECK_BOOL("EYE",           info->eye.type <= 47);
    CHECK_BOOL("EYE",           info->eye.color <= 5);
    CHECK_BOOL("EYE",           info->eye.scale <= 7);
    CHECK_BOOL("EYE",           info->eye.rotate <= 7);
    CHECK_BOOL("EYE",           info->eye.x <= 12);
    CHECK_BOOL("EYE",           info->eye.y <= 18);

    CHECK_BOOL("EYEBROW",       info->eyebrow.type <= 23);
    CHECK_BOOL("EYEBROW",       info->eyebrow.color <= 7);
    CHECK_BOOL("EYEBROW",       info->eyebrow.scale <= 8);
    CHECK_BOOL("EYEBROW",       info->eyebrow.rotate <= 11);
    CHECK_BOOL("EYEBROW",       info->eyebrow.x <= 12);
    CHECK_BOOL("EYEBROW",       info->eyebrow.y >= 3);
    CHECK_BOOL("EYEBROW",       info->eyebrow.y <= 18);

    CHECK_BOOL("NOSE",          info->nose.type <= 11);
    CHECK_BOOL("NOSE",          info->nose.scale <= 8);
    CHECK_BOOL("NOSE",          info->nose.y <= 18);

    CHECK_BOOL("MOUTH",         info->mouth.type <= 23);
    CHECK_BOOL("MOUTH",         info->mouth.color <= 2);
    CHECK_BOOL("MOUTH",         info->mouth.scale <= 8);
    CHECK_BOOL("MOUTH",         info->mouth.y <= 18);

    CHECK_BOOL("BEARD",         info->beard.mustache <= 3);
    CHECK_BOOL("BEARD",         info->beard.type <= 3);
    CHECK_BOOL("BEARD",         info->beard.color <= 7);
    CHECK_BOOL("BEARD",         info->beard.scale <= 8);
    CHECK_BOOL("BEARD",         info->beard.y <= 16);

    CHECK_BOOL("GLASS",         info->glass.type <= 8);
    CHECK_BOOL("GLASS",         info->glass.color <= 5);
    CHECK_BOOL("GLASS",         info->glass.scale <= 7);
    CHECK_BOOL("GLASS",         info->glass.y <= 20);

    CHECK_BOOL("MOLE",          info->mole.type <= 1);
    CHECK_BOOL("MOLE",          info->mole.scale <= 8);
    CHECK_BOOL("MOLE",          info->mole.x <= 16);
    CHECK_BOOL("MOLE",          info->mole.y <= 30);

    CHECK_BOOL("NAME",          RFLiIsValidName2(info));

    CHECK_BOOL("HEIGHT",        info->body.height <= 128);
    CHECK_BOOL("BUILD",         info->body.build <= 128);

    CHECK_BOOL("SEX",           info->personal.sex <= RFLSex_All-1);
    CHECK_BOOL("BIRTHDAY",      RFLiCheckBirthday(info->personal.birth_month, info->personal.birth_day));
    CHECK_BOOL("FAVORITECOLOR", info->personal.favoriteColor < RFLFavoriteColor_Max);
    CHECK_BOOL("CREATEID",      !RFLiIsSpecialID(&info->createID) || info->personal.localonly != 0);

    return TRUE;
}
#undef CHECK_BOOL

#define CHECK_BOOL(name, var)       if (!(var))          { return FALSE; }
BOOL RFLiCheckValidInfoNoName(const RFLiCharInfo* info) {
    CHECK_BOOL("FACELINE",      info->faceline.type <= 7);
    CHECK_BOOL("FACELINE",      info->faceline.color <= 5);
    CHECK_BOOL("FACELINE",      info->faceline.texture <= 11);

    CHECK_BOOL("HAIR",          info->hair.type <= 71);
    CHECK_BOOL("HAIR",          info->hair.color <= 7);
    CHECK_BOOL("HAIR",          info->hair.flip <= 1);

    CHECK_BOOL("EYE",           info->eye.type <= 47);
    CHECK_BOOL("EYE",           info->eye.color <= 5);
    CHECK_BOOL("EYE",           info->eye.scale <= 7);
    CHECK_BOOL("EYE",           info->eye.rotate <= 7);
    CHECK_BOOL("EYE",           info->eye.x <= 12);
    CHECK_BOOL("EYE",           info->eye.y <= 18);

    CHECK_BOOL("EYEBROW",       info->eyebrow.type <= 23);
    CHECK_BOOL("EYEBROW",       info->eyebrow.color <= 7);
    CHECK_BOOL("EYEBROW",       info->eyebrow.scale <= 8);
    CHECK_BOOL("EYEBROW",       info->eyebrow.rotate <= 11);
    CHECK_BOOL("EYEBROW",       info->eyebrow.x <= 12);
    CHECK_BOOL("EYEBROW",       info->eyebrow.y >= 3);
    CHECK_BOOL("EYEBROW",       info->eyebrow.y <= 18);

    CHECK_BOOL("NOSE",          info->nose.type <= 11);
    CHECK_BOOL("NOSE",          info->nose.scale <= 8);
    CHECK_BOOL("NOSE",          info->nose.y <= 18);

    CHECK_BOOL("MOUTH",         info->mouth.type <= 23);
    CHECK_BOOL("MOUTH",         info->mouth.color <= 2);
    CHECK_BOOL("MOUTH",         info->mouth.scale <= 8);
    CHECK_BOOL("MOUTH",         info->mouth.y <= 18);

    CHECK_BOOL("BEARD",         info->beard.mustache <= 3);
    CHECK_BOOL("BEARD",         info->beard.type <= 3);
    CHECK_BOOL("BEARD",         info->beard.color <= 7);
    CHECK_BOOL("BEARD",         info->beard.scale <= 8);
    CHECK_BOOL("BEARD",         info->beard.y <= 16);

    CHECK_BOOL("GLASS",         info->glass.type <= 8);
    CHECK_BOOL("GLASS",         info->glass.color <= 5);
    CHECK_BOOL("GLASS",         info->glass.scale <= 7);
    CHECK_BOOL("GLASS",         info->glass.y <= 20);

    CHECK_BOOL("MOLE",          info->mole.type <= 1);
    CHECK_BOOL("MOLE",          info->mole.scale <= 8);
    CHECK_BOOL("MOLE",          info->mole.x <= 16);
    CHECK_BOOL("MOLE",          info->mole.y <= 30);

    CHECK_BOOL("HEIGHT",        info->body.height <= 128);
    CHECK_BOOL("BUILD",         info->body.build <= 128);

    CHECK_BOOL("SEX",           info->personal.sex <= RFLSex_All-1);
    CHECK_BOOL("BIRTHDAY",      RFLiCheckBirthday(info->personal.birth_month, info->personal.birth_day));
    CHECK_BOOL("FAVORITECOLOR", info->personal.favoriteColor < RFLFavoriteColor_Max);
    CHECK_BOOL("CREATEID",      !RFLiIsSpecialID(&info->createID) || info->personal.localonly != 0);

    return TRUE;
}
#undef CHECK_BOOL

BOOL RFLiIsValidOnNAND(const RFLiCharInfo* info) {
    if (RFLiIsTemporaryID(&info->createID)) {
        RFLi_REPORT("Invalid TEMPORARY data.");
        return FALSE;
    }

    return TRUE;
}


BOOL RFLiIsSameFaceCore(const RFLiCharInfo* lhv, const RFLiCharInfo* rhv) {
    RFLi_ASSERTLINE_NULL(lhv, 211);
    RFLi_ASSERTLINE_NULL(rhv, 212);

    #define PAD_MASK(t, size)           ((t)(~((1 << size) - 1)))
    #define CHECK_PADMASK(p, ms, mt)    if ((lhv->p.rawdata & PAD_MASK(mt, ms)) != (rhv->p.rawdata & PAD_MASK(mt, ms))) { return FALSE; }

    CHECK_PADMASK(eye, 5, u32);
    CHECK_PADMASK(eyebrow, 6, u32);
    CHECK_PADMASK(mouth, 0, u16);
    CHECK_PADMASK(beard, 0, u16);
    CHECK_PADMASK(glass, 1, u16); // @BUG There is no padding for glass (old version left over?)
    CHECK_PADMASK(mole, 1, u16);
    CHECK_PADMASK(nose, 3, u16);
    CHECK_PADMASK(hair, 5, u16);
    CHECK_PADMASK(faceline, 6, u16);

    return TRUE;

    #undef CHECK_PADMASK
    #undef PAD_MASK
}

RFLCheckError RFLIsSameFace(RFLDataSource lhvs, RFLMiddleDB* lhvd, u16 lhvi, RFLDataSource rhvs, RFLMiddleDB* rhvd, u16 rhvi) {
    RFLiCharInfo linfo;
    RFLiCharInfo rinfo;

    if (RFLiPickupCharInfo(&linfo, lhvs, lhvd, lhvi) != RFLErrcode_Success) {
        return RFLCheckError_ErrorLeft;
    }

    if (RFLiPickupCharInfo(&rinfo, rhvs, rhvd, rhvi) != RFLErrcode_Success) {
        return RFLCheckError_ErrorRight;
    }

    if (RFLiIsSameFaceCore(&linfo, &rinfo)) {
        return RFLCheckError_Same;
    }
    else {
        return RFLCheckError_Difference;
    }
}

RFLErrcode RFLiPickupCharInfo(RFLiCharInfo* info, RFLDataSource source, const RFLMiddleDB* middleDB, u16 index) {
    RFLErrcode err;

    RFLi_ASSERTLINE_NULL(info, 290);

    if (!RFLAvailable()) {
        return RFLErrcode_NotAvailable;
    }

    switch (source) {
        case RFLDataSource_Official: {
            err = RFLiGetCharInfo(info, index);
            break;
        }
        case RFLDataSource_Hidden: {
            RFLi_ASSERTLINE_MSG(FALSE, 304, "裏データベースを直接参照することは出来ません"); // "The specified source cannot reference the database."
            break;
        }
        case RFLDataSource_Controller1:
        case RFLDataSource_Controller2:
        case RFLDataSource_Controller3:
        case RFLDataSource_Controller4: {
            BOOL r;

            s32 chan = source - RFLDataSource_Controller1;
            r = RFLiGetControllerData(info, chan, (u8)index, FALSE);
            if (r) {
                err = RFLErrcode_Success;
            }
            else {
                err = RFLErrcode_Broken;
            }
            break;
        }
        case RFLDataSource_Middle: {
            BOOL r;
            RFLi_ASSERTLINE_NULL(middleDB, 324);

            r = RFLiGetCharInfoMiddleDB(info, middleDB, index);
            if (r) {
                err = RFLErrcode_Success;
            }
            else {
                err = RFLErrcode_Broken;
            }
            break;
        }
        case RFLDataSource_Default: {
            RFLiGetDefaultData(info, index);
            err = RFLErrcode_Success;
            break;
        }
        default: {
            RFLi_ASSERTLINE_MSG(FALSE, 340, "指定されたデータソースが不正です"); // "The specified data source is invalid."
            break;
        }
    }

    if (err == RFLErrcode_Success) {
        if (!RFLiIsValidID(&info->createID)) {
            err = RFLErrcode_Broken;
        }
        else if (!RFLiCheckValidInfo(info)) {
            err = RFLErrcode_Broken;
        }
    }

    return err;
}

static void copyChar2Additional_(RFLAdditionalInfo* dst, const RFLiCharInfo* src) {
    u8 height;
    u8 build;

    memcpy(dst->name, src->personal.name, RFL_NAME_LENGTH * sizeof(u16));
    dst->name[RFL_NAME_LENGTH] = 0;

    memcpy(dst->creator, src->personal.creator, RFL_NAME_LENGTH * sizeof(u16));
    dst->name[RFL_NAME_LENGTH] = 0; // @BUG Copy and paste error
    
    dst->gender = src->personal.sex;

    if (RFLiCheckBirthday(src->personal.birth_month, src->personal.birth_day)) {
        dst->birth_month = src->personal.birth_month;
        dst->birth_day = src->personal.birth_day;
    }
    else {
        dst->birth_month = 0;
        dst->birth_day = 0;
    }

    dst->favoriteColor = src->personal.favoriteColor;
    dst->favorite = src->personal.favorite;

    height = src->body.height;
    build = src->body.build;

    // @BUG The max is not 127
    if (height > 128 - 1) {
        height = 128 - 1;
    }
    if (build > 128 - 1) {
        build = 128 - 1;
    }

    dst->height = height;
    dst->build = build;

    memcpy(&dst->createID, &src->createID, sizeof(RFLCreateID));

    dst->reserved = 0;
    dst->skinColor = RFLiGetFacelineColor(src);
}

RFLErrcode RFLGetAdditionalInfo(RFLAdditionalInfo* info, RFLDataSource source, RFLMiddleDB* db, u16 index) {
    RFLiCharInfo charInfo;
    RFLErrcode err;

    RFLi_ASSERTLINE_NULL(info, 432);

    err = RFLiPickupCharInfo(&charInfo, source, db, index);
    if (err == RFLErrcode_Success) {
        copyChar2Additional_(info, &charInfo);
    }

    return err;
}

BOOL RFLiCheckBirthday(u8 month, u8 day) {
    static const u8 scDayMax[12+1] = {0,  31, 29, 31, 30, 31, 30,
                                      31, 31, 30, 31, 30, 31};
    if (month == 0 && day != 0) {
        return FALSE;
    }
    if (month != 0 && day == 0) {
        return FALSE;
    }
    if (month > 12 || day > 31) {
        return FALSE;
    }
    if (day > scDayMax[month]) {
        return FALSE;
    }

    return TRUE;
}

#ifdef RFL_DEBUG
void RFLiCreateStoreData(RFLiStoreData* store, const RFLiCharInfo* info) {
    RFLiConvertInfo2Raw(info, &store->rawdata);
    store->crc = 0;
    store->crc = RFLiCalculateCRC(store, sizeof(RFLiStoreData));
}
#endif // RFL_DEBUG
