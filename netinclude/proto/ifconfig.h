#ifndef PROTO_IFCONFIG_H
#define PROTO_IFCONFIG_H \
       "$Id: ifconfig.h,v 1.1.1.1 1999/08/08 14:20:32 zapek Exp $"
/*
 *	SAS C prototypes for ifconfig.library
 *
 *      Copyright ? 1995 AmiTCP/IP Group,
 *                       Network Solutions Development, Inc.
 *                       All rights reserved.
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef __NOLIBBASE__       
extern struct Library *IfConfigBase;
#endif

#include <libraries/ifconfig.h>
#include <clib/ifconfig_protos.h>
#if __SASC
#include <pragmas/ifconfig_pragmas.h>
#elif __GNUC__
#include <inline/ifconfig.h>
#endif

#endif /* PROTO_IFCONFIG_H */
