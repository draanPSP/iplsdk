#include <cstdint>

#include <string.h>
#include <nand.h>
#include <lowio.h>

namespace
{
	constexpr inline std::uint32_t SUPPORTED_PAGE_SIZE = 0x200;
	constexpr inline std::uint32_t SUPPORTED_PAGES_PER_BLOCK = 0x20;

	constexpr inline std::uint32_t CMD_READ_STATUS = 0x70;
	constexpr inline std::uint32_t CMD_READ_ID = 0x90;
	constexpr inline std::uint32_t CMD_RESET = 0xFF;

	typedef enum {
		USER_ECC_IN_SPARE = 0x01,
		NO_AUTO_USER_ECC = 0x10,
		NO_AUTO_SPARE_ECC = 0x20
	} IplNandEccMode;

	typedef struct {
		std::uint32_t page_size;
		std::uint32_t pages_per_block;
		std::int32_t total_blocks;
	} __attribute__((packed)) IplNandProperties;

	IplNandProperties iplNandProperties;

	typedef struct {
		std::uint8_t manufacturer_id;
		std::uint8_t chip_id;
		std::uint16_t page_size;
		std::uint16_t pages_per_block;
		std::uint32_t total_blocks;
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

	std::int32_t iplNandReadId(std::uint8_t *id, std::int32_t len) {
		memoryK1(REG_EMC_CMD) = CMD_READ_ID;
		memoryK1(REG_EMC_ADDR) = 0x0;

		if (id)
		{
			for (std::int32_t i = 0; i < len; i++)
				id[i] = memoryK1(REG_EMC_DATA);
		}

		memoryK1(REG_EMC_RST) = 0x1;

		return 0;
	}

	std::int32_t iplNandDetectChip(void) {
		std::uint8_t id[2];
		std::int32_t ret;

		if ((ret = iplNandReadId(id, 2)) < 0)
			return ret;

		iplNandProperties.page_size = 0;
		iplNandProperties.pages_per_block = 0;
		iplNandProperties.total_blocks = 0;

		for (std::int32_t i = 0; i < sizeof(iplNandChips) / sizeof(IplNandChipProperties); i++)
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

	std::int32_t iplNandIsReady(void) {
		return memoryK1(REG_EMC_STATUS) & 0x1;
	}

	std::int32_t iplNandReset(void) {
		memoryK1(REG_EMC_CMD) = CMD_RESET;
		while (!iplNandIsReady());
		memoryK1(REG_EMC_RST) = 0x1;

		return 0;
	}

	std::int32_t iplNandReadAccess(std::uint32_t ppn, void *user, void *spare, std::uint32_t mode) {
		while (!iplNandIsReady());

		if (mode & NO_AUTO_USER_ECC)
			memoryK1(REG_EMC_CTRL) &= ~0x00010000;
		else
			memoryK1(REG_EMC_CTRL) |= 0x00010000;

		memoryK1(REG_EMC_DMA_ADDR) = ppn << 10;
		memoryK1(REG_EMC_DMA_CTRL) = 0x301;

		std::uint32_t dma_ctrl;
		do {
			dma_ctrl = memoryK1(REG_EMC_DMA_CTRL);
		} while ((dma_ctrl & 1) != 0);

		if (mode & NO_AUTO_USER_ECC == 0 && memoryK1(REG_EMC_DMA_STATUS) != 0)
			return -1;

		if (spare) {
			std::uint32_t *spareBuf = reinterpret_cast<std::uint32_t *>(spare);
			for (std::int32_t i = 0; i < 0xC / 4; i++) {
				spareBuf[i] = memoryK1(EMC_SPARE_DATA_BUF + i * 4);
			}
		}

		if (user) {
			std::uint32_t *userBuf = reinterpret_cast<std::uint32_t *>(user);
			for (std::int32_t i = 0; i < SUPPORTED_PAGE_SIZE / 4; i++) {
				userBuf[i] = memoryK1(EMC_USER_DATA_BUF + i * 4);
			}
		}

		return 0;
	}
}

int32_t iplNandInit(void) {
	iplSysregEmcsmBusClockEnable();
	iplSysregEmcsmIoEnable();

	if (iplNandIsReady()) {
		iplNandReset();
		return iplNandDetectChip();
	}

	return -1;
}

int32_t iplNandGetPageSize(void) {
	return iplNandProperties.page_size;
}

int32_t iplNandGetPagesPerBlock(void) {
	return iplNandProperties.pages_per_block;
}

int32_t iplNandGetTotalBlocks(void) {
	return iplNandProperties.total_blocks;
}

int32_t iplNandReadPage(uint32_t ppn, void *user, void *spare) {
	return iplNandReadAccess(ppn, user, spare, 0);
}
