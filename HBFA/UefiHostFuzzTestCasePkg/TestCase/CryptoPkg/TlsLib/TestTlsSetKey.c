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
  UINT8 *Password;
  UINTN PasswordSize;
  VOID *SslCtx;
  VOID *SslConn;
  UINT8 *TestPointBuffer;
  UINT8 Index;

  TestPointBuffer = TestBuffer;
  SslCtx = NULL;
  SslConn = NULL;
  Password = NULL;

  //STEP 1:
  //Input parse:
  //Focus on suitable length
  if (TestBufferSize > 0) {
    // TestPointBuffer := [| Password Length: 1| | Password: Password Length| | Key Data: Remainder|]
    PasswordSize = TestPointBuffer[0];
    if (TestBufferSize > PasswordSize + 1) {
      // Convert password to string
      Password = (UINT8 *) AllocatePool (PasswordSize + 1);
      for (Index = 0; Index < PasswordSize; Index++) {
        Password[Index] = TestPointBuffer[1 + Index];
      }
      Password[PasswordSize] = '\0';

      TestPointBuffer = TestPointBuffer + 1 + PasswordSize;

      TlsInitialize ();
      /* Use minimal version of TLS 1.2 */
      SslCtx = TlsCtxNew (0x3, 0x03);
      if (SslCtx != NULL) {
        SslConn = TlsNew (SslCtx);
        if (SslConn != NULL) {
          TlsSetHostPrivateKeyEx (SslConn, TestPointBuffer, TestBufferSize - 1 - PasswordSize, Password);
        }
      }

      TlsFree (SslConn);
      TlsCtxFree (SslCtx);
      FreePool (Password);
    }
  }
}
