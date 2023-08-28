#ifndef IPLSDK_MS
#define IPLSDK_MS

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    void sdkMsInit();
    int32_t sdkMsReadSector(uint32_t const sector, void *buf);
    int32_t sdkMsWriteSector(uint32_t const sector, void const *buf);

#ifdef __cplusplus
}
#endif



#endif //IPLSDK_MON