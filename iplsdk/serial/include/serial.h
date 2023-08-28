#ifndef IPLSDK_SERIAL
#define IPLSDK_SERIAL

#include <cstdint>
#include "printf.h"

void iplKernelUart4Init();
std::uint32_t iplKernelUart4Suspend();
void iplKernelUart4End();

void sdkUartHpRemoteInit();
std::uint32_t sdkKernelUartHpRemoteSuspend();
void sdkKernelUartHpRemoteEnd();

void sdkUartIrdaInit();
std::uint32_t sdkKernelUartIrdaSuspend();
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