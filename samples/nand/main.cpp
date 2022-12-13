#include <psptypes.h>

#include <nand.h>
#include <lowio.h>
#include <syscon.h>
#include <serial.h>

#define PAGE_SIZE 0x200
#define IPL_BLOCK_LIST_BLOCK 0x4
#define IPL_BLOCK_LIST_DUP_COUNT 0x8
#define IPL_BLOCK_ID 0x6DC64A38

u16 iplBlockListBuf[PAGE_SIZE/sizeof(u16)];
u32 spareBuf[4];

void readIplBlockList() {
	u32 pagesPerBlock = iplNandGetPagesPerBlock();

	for (int i = 0; i < IPL_BLOCK_LIST_DUP_COUNT; i++) {
		u32 block = IPL_BLOCK_LIST_BLOCK + i;
		u32 ppn = block * pagesPerBlock;
		int res = iplNandReadPage(ppn, iplBlockListBuf, spareBuf);

		if (res >= 0 && spareBuf[1] == IPL_BLOCK_ID) {
			printf("Found IPL block list at block 0x%x\n", block);
			printf("IPL Blocks: ");

			for (int j = 0; j < sizeof(iplBlockListBuf) / sizeof(u16); j++) {
				u16 iplBlock = iplBlockListBuf[j];

				if (!iplBlock) {
					break;
				}

				if (j != 0) {
					printf(", ");
				}

				printf("0x%02X", iplBlock);
			}
		}
	}
}

int main()
{
	// Init communication with syscon
	iplSysconInit();

	// Tell syscon to apply power to HP Remote
	iplSysconCtrlHRPower(true);

	// Initialize HP Remote Serial
	sdkUartHpRemoteInit();

	// Initialize Nand
	int ret = iplNandInit();

	if (ret < 0) {
		printf("Error initializing nand: 0x%x\n", ret);
		goto exit;
	}

	printf("Your PSP's nand page size 0x%x\n", iplNandGetPageSize());
	printf("Your PSP's nand pages per block is 0x%x\n", iplNandGetPagesPerBlock());
	printf("Your PSP's nand block count is 0x%x\n", iplNandGetTotalBlocks());

	readIplBlockList();

	for (u32 i = 0; i < 5; ++i) {
		sdkWait(1 * 1000 * 1000);
	}

exit:

	// PSP will not shut down properly if we don't stop UART first
	sdkKernelUartHpRemoteSuspend();
	sdkKernelUartHpRemoteEnd();

	// Shut down the PSP
	iplSysconPowerStandby();

	return 0;
}