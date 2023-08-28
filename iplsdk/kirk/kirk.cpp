#include <kirk.h>
#include <lowio.h>
#include <cache.h>

namespace {
	enum Phase {
		P1 = 1,
		P2
	};
}

std::int32_t sdkKirkCmd(void *dst, std::uint32_t dsize, void *src, std::uint32_t ssize, KirkCmd cmd)
{
	iplSysregKirkBusClockEnable();
	
	iplKernelDcacheWritebackInvalidateAll();

	memoryK1(REG_KIRK_CMD) = cmd;
	memoryK1(REG_KIRK_SRC_ADDR) = physicalAddr(reinterpret_cast<std::uint32_t>(src));
	memoryK1(REG_KIRK_DEST_ADDR) = physicalAddr(reinterpret_cast<std::uint32_t>(dst));
	memoryK1(REG_KIRK_PROC_PHASE) = Phase::P1;

	std::uint32_t status;
	do {
		status = memoryK1(REG_KIRK_PATTERN);
	} while ((status & 0x11) == 0);

	memoryK1(REG_KIRK_PATTERN_END) = status;

	if (status & 0x10) {
		memoryK1(REG_KIRK_PROC_PHASE) = Phase::P2;

		do {
			status = memoryK1(REG_KIRK_PATTERN);
		} while ((status & 0x02) == 0);

		memoryK1(REG_KIRK_PATTERN_END) = status;

		sdkSync();

		return -1;
	}

	sdkSync();

	iplSysregKirkBusClockDisable();

	return memoryK1(REG_KIRK_RESULT);
}
