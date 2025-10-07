#ifndef REVOLUTION_OS_CONTEXT_H
#define REVOLUTION_OS_CONTEXT_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSContext {
    unsigned long gpr[32]; // 0x0
    unsigned long cr; // 0x80
    unsigned long lr; // 0x84
    unsigned long ctr; // 0x88
    unsigned long xer; // 0x8C
    double fpr[32]; // 0x90
    unsigned long fpscr_pad; // 0x190
    unsigned long fpscr; // 0x194
    unsigned long srr0; // 0x198
    unsigned long srr1; // 0x19C
    unsigned short mode; // 0x1A0
    unsigned short state; // 0x1A2
    unsigned long gqr[8]; // 0x1A4
    unsigned long psf_pad; // 0x1C4
    double psf[32]; // 0x1C8
} OSContext;

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_OS_MUTEX_H
