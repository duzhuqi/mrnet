/****************************************************************************
 * Copyright � 2003-2011 Dorian C. Arnold, Philip C. Roth, Barton P. Miller *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

#if !defined(test_nativefilters_lightweight_h )
#define test_nativefilters_lightweight_h 1

#include "mrnet_lightweight/Types.h"

typedef enum { PROT_EXIT=FirstApplicationTag, PROT_SUM, PROT_MAX } Protocol;

const char_t CHARVAL=7;
const uchar_t UCHARVAL=7;
const int16_t INT16VAL=-17;
const uint16_t UINT16VAL=17;
const int32_t INT32VAL=-17;
const uint32_t UINT32VAL=17;
const int64_t INT64VAL=-17;
const uint64_t UINT64VAL=17;
const float FLOATVAL=(const float)123.456;
const double DOUBLEVAL=123.456;

#endif /* test_nativefilters_lightweight_h */
