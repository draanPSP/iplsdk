#include <string.h>

#include <nand.h>
#include <lowio.h>

namespace
{
	constexpr inline u32 SUPPORTED_PAGE_SIZE = 0x200;
	constexpr inline u32 SUPPORTED_PAGES_PER_BLOCK = 0x20;

	constexpr inline u32 CMD_READ_STATUS = 0x70;
	constexpr inline u32 CMD_READ_ID = 0x90;
	constexpr inline u32 CMD_RESET = 0xFF;

	typedef enum {
		USER_ECC_IN_SPARE = 0x01,
		NO_AUTO_USER_ECC = 0x10,
		NO_AUTO_SPARE_ECC = 0x20
	} IplNandEccMode;

	typedef struct {
		u32 page_size;
		u32 pages_per_block;
		s32 total_blocks;
	} __attribute__((packed)) IplNandProperties;

	IplNandProperties iplNandProperties;

	typedef struct {
		u8 manufacturer_id;
		u8 chip_id;
		u16 page_size;
		u16 pages_per_block;
		u32 total_blocks;
	} IplNandChipProperties;

	const IplNandChipProperties iplNandChips[] = {
		{0x98, 0xE6, 0x0200, 0x0010, 0x00000400},
		{0x98, 0x73, 0x0200, 0x0020, 0x00000400},
		{0x98, 0x75, 0x0200, 0x0020, 0x00000800},
		{0x98, 0x76, 0x0200, 0x0020, 0x00001000},
		{0x98, 0x79, 0x0200, 0x0020, 0x00002000},
		{0xEC, 0xE6, 0x0200, 0x0010, 0x00000400},
		{0xEC, 0x73, 0x0200, 0x0020, 0x00000400},
		{0xEC, 0x75, 0x0200, 0x0020, 0x00000800},
		{0xEC, 0x76, 0x0200, 0x0020, 0x00001000},
		{0xEC, 0x79, 0x0200, 0x0020, 0x00002000},
		{0xEC, 0x71, 0x0200, 0x0020, 0x00004000},
		{0xEC, 0xDC, 0x0200, 0x0020, 0x00008000},
		{0xEC, 0x39, 0x0200, 0x0010, 0x00000400},
		{0xEC, 0x33, 0x0200, 0x0020, 0x00000400},
		{0xEC, 0x35, 0x0200, 0x0020, 0x00000800},
		{0xEC, 0x36, 0x0200, 0x0020, 0x00001000},
		{0xEC, 0x78, 0x0200, 0x0020, 0x00002000},
		{0x20, 0x35, 0x0200, 0x0020, 0x00000800},
		{0x20, 0x36, 0x0200, 0x0020, 0x00001000},
		{0x20, 0x39, 0x0200, 0x0020, 0x00002000},
		{0xAD, 0x35, 0x0200, 0x0020, 0x00000800},
		{0xAD, 0x36, 0x0200, 0x0020, 0x00001000},
		{0xAD, 0x39, 0x0200, 0x0020, 0x00002000},
	};

	s32 iplNandReadId(u8 *id, s32 len) {
		memoryK1(REG_EMC_CMD) = CMD_READ_ID;
		memoryK1(REG_EMC_ADDR) = 0x0;

		if (id)
		{
			for (s32 i = 0; i < len; i++)
				id[i] = memoryK1(REG_EMC_DATA);
		}

		memoryK1(REG_EMC_RST) = 0x1;

		return 0;
	}

	s32 iplNandDetectChip(void) {
		u8 id[2];
		s32 ret;

		if ((ret = iplNandReadId(id, 2)) < 0)
			return ret;

		iplNandProperties.page_size = 0;
		iplNandProperties.pages_per_block = 0;
		iplNandProperties.total_blocks = 0;

		for (s32 i = 0; i < sizeof(iplNandChips) / sizeof(IplNandChipProperties); i++)
		{
			if (id[0] == iplNandChips[i].manufacturer_id && id[1] == iplNandChips[i].chip_id)
			{
				iplNandProperties.page_size = iplNandChips[i].page_size;
				iplNandProperties.pages_per_block = iplNandChips[i].pages_per_block;
				iplNandProperties.total_blocks = iplNandChips[i].total_blocks;

				if (iplNandProperties.page_size != SUPPORTED_PAGE_SIZE ||
					iplNandProperties.pages_per_block != SUPPORTED_PAGES_PER_BLOCK)
					return -1;

				return 0;
			}
		}

		return -1;
	}

	s32 iplNandIsReady(void) {
		return memoryK1(REG_EMC_STATUS) & 0x1;
	}

	s32 iplNandReset(void) {
		memoryK1(REG_EMC_CMD) = CMD_RESET;
		while (!iplNandIsReady());
		memoryK1(REG_EMC_RST) = 0x1;

		return 0;
	}

	s32 iplNandReadAccess(u32 ppn, void *user, void *spare, u32 mode) {
		while (!iplNandIsReady());

		if (mode & NO_AUTO_USER_ECC)
			memoryK1(REG_EMC_CTRL) &= ~0x00010000;
		else
			memoryK1(REG_EMC_CTRL) |= 0x00010000;

		memoryK1(REG_EMC_DMA_ADDR) = ppn << 10;
		memoryK1(REG_EMC_DMA_CTRL) = 0x301;

		u32 dma_ctrl;
		do {
			dma_ctrl = memoryK1(REG_EMC_DMA_CTRL);
		} while ((dma_ctrl & 1) != 0);

		if (mode & NO_AUTO_USER_ECC == 0 && memoryK1(REG_EMC_DMA_STATUS) != 0)
			return -1;

		if (spare) {
			u32 *spareBuf = reinterpret_cast<u32 *>(spare);
			for (s32 i = 0; i < 0xC / 4; i++) {
				spareBuf[i] = memoryK1(EMC_SPARE_DATA_BUF + i * 4);
			}
		}

		if (user) {
			u32 *userBuf = reinterpret_cast<u32 *>(user);
			for (s32 i = 0; i < SUPPORTED_PAGE_SIZE / 4; i++) {
				userBuf[i] = memoryK1(EMC_USER_DATA_BUF + i * 4);
			}
		}

		return 0;
	}
}

s32 iplNandInit(void) {
	iplSysregEmcsmBusClockEnable();
	iplSysregEmcsmIoEnable();

	if (iplNandIsReady()) {
		iplNandReset();
		return iplNandDetectChip();
	}

	return -1;
}

s32 iplNandGetPageSize(void) {
	return iplNandProperties.page_size;
}

s32 iplNandGetPagesPerBlock(void) {
	return iplNandProperties.pages_per_block;
}

s32 iplNandGetTotalBlocks(void) {
	return iplNandProperties.total_blocks;
}

s32 iplNandReadPage(u32 ppn, void *user, void *spare) {
	return iplNandReadAccess(ppn, user, spare, 0);
}
