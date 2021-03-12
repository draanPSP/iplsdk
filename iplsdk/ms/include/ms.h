#ifndef IPLSDK_MS
#define IPLSDK_MS

#include <psptypes.h>

#ifdef __cplusplus
extern "C" {
#endif

    void sdkMsInit();
    s32 sdkMsReadSector(u32 const sector, void *buf);
    s32 sdkMsWriteSector(u32 const sector, void const *buf);

#ifdef __cplusplus
}
#endif



#endif //IPLSDK_MON