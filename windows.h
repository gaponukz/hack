#ifndef _WINDOWS_H
#pragma GCC system_header
#define _WINDOWS_H

#ifdef RC_INVOKED
/* winresrc.h includes the necessary headers */
#include <winresrc.h>
#else

#include <stdarg.h>
#include <windef.h>
#include <wincon.h>
#include <winbase.h>
#if !(defined NOGDI || defined  _WINGDI_H)
#include <wingdi.h>
#endif
#include <winuser.h>
#include <winnls.h>
#include <winver.h>
#include <winnetwk.h>
#include <winreg.h>
#include <winsvc.h>

#ifndef WIN32_LEAN_AND_MEAN
#include <cderr.h>
#include <dde.h>
#include <ddeml.h>
#include <dlgs.h>
#include <imm.h>
#include <lzexpand.h>
#include <mmsystem.h>
#include <nb30.h>
#include <rpc.h>
#include <shellapi.h>
#include <winperf.h>
#ifndef NOGDI
#include <commdlg.h>
#include <winspool.h>
#endif
#if defined Win32_Winsock
#warning "The Win32_Winsock macro is deprecated; use __USE_W32_SOCKETS instead."
# ifndef __USE_W32_SOCKETS
#  define __USE_W32_SOCKETS
# endif
#endif
#if defined __USE_W32_SOCKETS \
 || ! (defined __CYGWIN__ || defined __MSYS__ || defined _UWIN)

# include "_winsock.h"
# 
#endif
#ifndef NOGDI

#if !defined (_OBJC_NO_COM)
#if (__GNUC__ >= 3) || defined (__WATCOMC__)
#include <ole2.h>
#endif
#endif 
#endif

#endif 

#endif 

#ifdef __OBJC__

#undef BOOL
#endif

#endif
