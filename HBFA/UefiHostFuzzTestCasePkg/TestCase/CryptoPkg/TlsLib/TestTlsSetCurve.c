/** @file

Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/TlsLib.h>

#define TOTAL_SIZE (512 * 1024)

VOID
FixBuffer (
  UINT8                   *TestBuffer
  )
{
}

UINTN
EFIAPI
GetMaxBufferSize (
  VOID
  )
{
  return TOTAL_SIZE;
}

VOID
EFIAPI
RunTestHarness(
  IN VOID  *TestBuffer,
  IN UINTN TestBufferSize
  )
{
  VOID *SslCtx;
  VOID *SslConn;

  SslCtx = NULL;
  SslConn = NULL;

  //STEP 1:
  //Input parse:
  //Focus on suitable length
  TlsInitialize ();
  /* Use minimal version of TLS 1.2 */
  SslCtx = TlsCtxNew (0x3, 0x03);
	if (SslCtx != NULL) {
		SslConn = TlsNew (SslCtx);
    if (SslConn != NULL) {
      TlsSetEcCurve (SslConn, TestBuffer, TestBufferSize);
      TlsShutdown (SslConn);
    }
	}

  TlsFree (SslConn);
  TlsCtxFree (SslCtx);
}
