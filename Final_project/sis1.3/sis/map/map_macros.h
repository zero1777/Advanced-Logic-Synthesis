/*
 * Revision Control Information
 *
 * $Source: /vol/opua/opua2/sis/sis-1.1/common/src/sis/map/RCS/map_macros.h,v $
 * $Author: sis $
 * $Revision: 1.2 $
 * $Date: 1992/05/06 18:55:51 $
 *
 */
/* file @(#)map_macros.h	1.2 */
/* last modified on 5/1/91 at 15:51:27 */
#ifndef MAP_MACROS_H
#define MAP_MACROS_H

#include "map_defs.h"
#define DISCRETIZATION_FACTOR 100

#define SETSUB(a,b,c) ((a).rise=(b).rise-(c).rise, (a).fall=(b).fall-(c).fall)
#define SETADD(a,b,c) ((a).rise=(b).rise+(c).rise, (a).fall=(b).fall+(c).fall)
#define GETMIN(a) MIN((a).rise, (a).fall)
#define GETMAX(a) MAX((a).rise, (a).fall)
#define SETMIN(a,b,c) ((a).rise=MIN((b).rise, (c).rise), (a).fall=MIN((b).fall,(c).fall))
#define SETMAX(a,b,c) ((a).rise=MAX((b).rise, (c).rise), (a).fall=MAX((b).fall,(c).fall))
#define SETMAX2(a,b)  ((a)=((a)>(b))?(a):(b))
#define SETMIN2(a,b)  ((a)=((a)<(b))?(a):(b))
#define IS_EQUAL(a,b) (FP_EQUAL((a).rise,(b).rise) && FP_EQUAL((a).fall,(b).fall))
#define IS_BETTER(a,b) ((a).rise>=(b).rise && (a).fall>=(b).fall)

#endif
