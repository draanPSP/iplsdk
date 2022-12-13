#ifndef IPLSDK_NAND
#define IPLSDK_NAND

#include <psptypes.h>

#ifdef __cplusplus
extern "C" {
#endif

	s32 iplNandInit(void);
	s32 iplNandGetPageSize(void);
	s32 iplNandGetPagesPerBlock(void);
	s32 iplNandGetTotalBlocks(void);
	s32 iplNandReadPage(u32 ppn, void *user, void *spare);

#ifdef __cplusplus
}
#endif

#endif // IPLSDK_NAND