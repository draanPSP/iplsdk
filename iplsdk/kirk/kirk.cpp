#include <kirk.h>
#include <lowio.h>
#include <cache.h>

namespace {
	enum Phase {
		P1 = 1,
		P2
	};
}

s32 sdkKirkCmd(void *dst, u32 dsize, void *src, u32 ssize, KirkCmd cmd)
{
	iplSysregKirkBusClockEnable();
	
	iplKernelDcacheWritebackInvalidateAll();

	memoryK1(REG_KIRK_CMD) = cmd;
	memoryK1(REG_KIRK_SRC_ADDR) = physicalAddr(reinterpret_cast<u32>(src));
	memoryK1(REG_KIRK_DEST_ADDR) = physicalAddr(reinterpret_cast<u32>(dst));
	memoryK1(REG_KIRK_PROC_PHASE) = Phase::P1;

	u32 status;
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
