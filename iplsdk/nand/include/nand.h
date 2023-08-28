#ifndef IPLSDK_NAND
#define IPLSDK_NAND

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	int32_t iplNandInit(void);
	int32_t iplNandGetPageSize(void);
	int32_t iplNandGetPagesPerBlock(void);
	int32_t iplNandGetTotalBlocks(void);
	int32_t iplNandReadPage(uint32_t ppn, void *user, void *spare);

#ifdef __cplusplus
}
#endif

#endif // IPLSDK_NAND