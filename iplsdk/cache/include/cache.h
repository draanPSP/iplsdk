#ifndef IPLSDK_CACHE
#define IPLSDK_CACHE

#include <psptypes.h>

inline void sdkSync() {
	asm("sync\n");
}

void iplKernelDcacheWritebackInvalidateAll();

void iplKernelIcacheInvalidateAll();

#endif //IPLSDK_CACHE
