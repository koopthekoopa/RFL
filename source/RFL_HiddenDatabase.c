#include <RVLFaceLib.h>
#include <RVLFaceLibInternal.h>

static void initWritableList_() {
    RFLiHiddenDBManager* manager = RFLiGetHDBManager(); // r31
    manager->writeCallback = NULL;
    manager->list.num = 0;
    manager->list.current = 0;
}

void RFLiInitHiddenDatabase() {
    RFLiHiddenDBManager* manager = RFLiGetHDBManager(); // r31
}
