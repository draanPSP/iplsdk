#ifndef IPLSDK_KIRK
#define IPLSDK_KIRK

#include <psptypes.h>

#ifdef __cplusplus
extern "C" {
#endif

	enum KirkCmd {
		CMD1 = 1
	};

    s32 sdkKirkCmd(void *dst, u32 dsize, void *src, u32 ssize, KirkCmd cmd);

#ifdef __cplusplus
}
#endif



#endif //IPLSDK_KIRK