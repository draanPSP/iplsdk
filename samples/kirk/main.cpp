#include <psptypes.h>

#include <kirk.h>
#include <lowio.h>
#include <nand.h>
#include <syscon.h>
#include <serial.h>

#define PAGE_SIZE 0x200
#define IPL_BLOCK_LIST_BLOCK 0x4
#define IPL_BLOCK_LIST_DUP_COUNT 0x8
#define IPL_BLOCK_ID 0x6DC64A38
// TODO - Determine if bootrom still mapped, if not use 0xBFC00000.
#define IPL_BUF 0xBFD00000
#define IPL_BLOCK_SIZE 0x1000

typedef struct {
	u32 dest;
	u32 size;
	u32 entry;
	u32 checksum;
	u32 data;
} IplHeader;

u16 iplBlockListBuf[PAGE_SIZE/sizeof(u16)];
u32 spareBuf[4];
void *iplBuf = reinterpret_cast<void *>(IPL_BUF);
IplHeader *iplHeader = reinterpret_cast<IplHeader *>(IPL_BUF);

s32 readIplBlockList() {
	u32 pagesPerBlock = iplNandGetPagesPerBlock();

	for (int i = 0; i < IPL_BLOCK_LIST_DUP_COUNT; i++) {
		u32 block = IPL_BLOCK_LIST_BLOCK + i;
		u32 ppn = block * pagesPerBlock;
		int ret = iplNandReadPage(ppn, iplBlockListBuf, spareBuf);

		if (ret >= 0 && spareBuf[1] == IPL_BLOCK_ID) {
			printf("Found IPL block list at block 0x%x\n", block);
			printf("IPL Blocks: ");

			for (int j = 0; j < sizeof(iplBlockListBuf) / sizeof(u16); j++) {
				u16 iplBlock = iplBlockListBuf[j];

				if (!iplBlock) {
					break;
				}

				return 0;
			}
		}
	}

	return -1;
}

u32 readIplBlock(u32 iplBlockNum, void *dst) {
	u32 block = iplBlockListBuf[iplBlockNum / 4];
	u16 page = 0;
	u32 ppn;
	u32 ret;

	printf("Reading IPL block: 0x%X\n", iplBlockNum);
	printf("NAND Block: 0x%X\n", block);

	for (int i = 0; i < 8; i++)
	{
		ppn = (block << 2 | iplBlockNum & 0x3) * 8 + i;
		ret = iplNandReadPage(ppn, (void*)((u32)dst + i*PAGE_SIZE), spareBuf);
		if (spareBuf[1] != IPL_BLOCK_ID)
			return -1;
	}

	return 0;
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
	if (iplNandInit() < 0) {
		printf("Error initializing nand\n");
		goto exit;
	}

	// Read IPL Nand Block list
	if (readIplBlockList() < 0) {
		printf("Error reading IPL FAT\n");
		goto exit;
	}

	// Read first block of IPL from Nand
	if (readIplBlock(0, iplBuf) < 0) {
		printf("Error reading IPL Block\n");
		goto exit;
	}

	printf("Decryting Block\n");

	// Decrypt first block of IPL which is a 0x1000 byte Kirk 1 block
	if (sdkKirkCmd(iplBuf, IPL_BLOCK_SIZE, iplBuf, IPL_BLOCK_SIZE, KirkCmd::CMD1) < 0) {
		printf("Error decryting IPL block\n");
		goto exit;
	}

	printf("IPL addr: 0x%X\n", iplHeader->dest);
	printf("IPL size: 0x%X\n", iplHeader->size);
	printf("IPL entry: 0x%X\n", iplHeader->entry);

	if (iplHeader->entry == 0xBFD00100) {
		printf("Pandora cIPL detected\n");
	}
	else if (iplHeader->dest == 0xBC10004C) {
		printf("Reset exploit cIPL detected\n");
	}
	else {
		printf("OFW or signed cIPL detected\n");
	}

exit:

	// PSP will not shut down properly if we don't stop UART first
	sdkKernelUartHpRemoteSuspend();
	sdkKernelUartHpRemoteEnd();

	// Shut down the PSP
	iplSysconPowerStandby();

	return 0;
}