#ifndef IPLSDK_SERIAL
#define IPLSDK_SERIAL

#include <psptypes.h>
#include "printf.h"

void iplKernelUart4Init();
u32 iplKernelUart4Suspend();
void iplKernelUart4End();

void sdkUartHpRemoteInit();
u32 sdkKernelUartHpRemoteSuspend();
void sdkKernelUartHpRemoteEnd();

void sdkUartIrdaInit();
u32 sdkKernelUartIrdaSuspend();
void sdkKernelUartIrdaEnd();

// Define putchar function used by printf
#ifdef __cplusplus
extern "C" {
#endif

	void _putchar(char c);

#ifdef __cplusplus
}
#endif

#endif //IPLSDK_SERIAL