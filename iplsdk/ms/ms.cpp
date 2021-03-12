#include <ms.h>
#include <lowio.h>

namespace {
	constexpr inline u32 CMD_READ_PAGE_DATA = 0x2000;
	constexpr inline u32 CMD_READ_REG = 0x4000;
	constexpr inline u32 CMD_GET_INT = 0x7000;
	constexpr inline u32 CMD_SET_RW_REG_ADRS = 0x8000;
	constexpr inline u32 CMD_EX_SET_CMD = 0x9000;
	constexpr inline u32 CMD_WRITE_REG = 0xB000;
	constexpr inline u32 CMD_WRIE_PAGE_DATA = 0xD000;
	constexpr inline u32 CMD_SET_CMD = 0xE000;

	constexpr inline u32 SYS_RST = 0x8000;

	constexpr inline u32 STATUS_TIMEOUT = 0x100;
	constexpr inline u32 STATUS_CRC_ERROR = 0x200;
	constexpr inline u32 STATUS_READY = 0x1000;
	constexpr inline u32 STATUS_UNK = 0x2000;
	constexpr inline u32 STATUS_FIFO_RW = 0x4000;

	constexpr inline u32 INT_REG_CMDNK = 0x1;
	constexpr inline u32 INT_REG_BREQ = 0x20;
	constexpr inline u32 INT_REG_ERROR = 0x40;
	constexpr inline u32 INT_REG_CED = 0x80;

	inline void _sdkMsReset() {
		memoryK1(REG_MS_SYS) = SYS_RST;
		
		while ((memoryK1(REG_MS_SYS) & SYS_RST) != 0);
	}

	inline s32 _sdkWaitMsReady() {
		while((memoryK1(REG_MS_STATUS) & STATUS_READY) == 0);

		if (memoryK1(REG_MS_STATUS) & (STATUS_TIMEOUT | STATUS_CRC_ERROR)) {
			return -1;
		}

		return 0;
	}

	inline s32 _sdkSendDataAndSync(void const *buf) {
		auto data = reinterpret_cast<u32 const*>(buf);

		memoryK1(REG_MS_DATA) = data[0];
		memoryK1(REG_MS_DATA) = data[1];

		return _sdkWaitMsReady();
	}

	s32 _sdkReadMsData(void *buf, u32 const byte_count) {
		auto data = reinterpret_cast<u32*>(buf);
		u32 const dword_count = byte_count/sizeof(u32);

		for(u32 i = 0; i < dword_count; i++) {
			u32 status;
			do {
				status = memoryK1(REG_MS_STATUS);

				if (status & STATUS_TIMEOUT) {
					return -1;
				}

			} while ((status & STATUS_FIFO_RW) == 0);

			data[i] = memoryK1(REG_MS_DATA);
		}

		return 0;
	}

	s32 _sdkWriteMsData(void const *buf, u32 const byte_count) {
		auto data = reinterpret_cast<u32 const*>(buf);
		u32 const dword_count = byte_count/sizeof(u32);

		for(u32 i = 0; i < dword_count; i++) {
			u32 status;
			do {
				status = memoryK1(REG_MS_STATUS);

				if (status & STATUS_TIMEOUT) {
					return -1;
				}

			} while ((status & STATUS_FIFO_RW) == 0);

			memoryK1(REG_MS_DATA) = data[i];
		}

		return 0;
	}

	inline s32 _sdkReadMsReg(void *buf, u32 const reg) {
		memoryK1(REG_MS_CMD) = CMD_READ_REG | reg;

		return _sdkReadMsData(buf, reg);
	}

	inline s32 _sdkReadMsRegInt() {
		memoryK1(REG_MS_CMD) = CMD_GET_INT | 0x1;

		u32 data[2];

		if (_sdkReadMsData(data, sizeof(u32)*2) < 0) {
			return -1;
		}

		if (_sdkWaitMsReady() < 0) {
			return -1;
		}

		return data[0];
	}

	inline void _sdkWaitMsStatus() {
		u32 status;

		do {
			status = memoryK1(REG_MS_STATUS);
		} while ((status & STATUS_UNK) == 0);
	}

	s32 _sdkCheckMsStatus() {
		u8 status[8];

		memoryK1(REG_MS_CMD) = CMD_SET_RW_REG_ADRS | 0x4;

		u8 const buffer[8] = {0x00, 0x08, 0x10, 0x06, 0x00, 0x00, 0x00, 0x00};

		s32 ret = _sdkSendDataAndSync(buffer);

		if (ret < 0) {
			return -1;
		}

		_sdkReadMsReg(status, sizeof(u8)*8);

		if (status[4] != 0x1) {
			return -1; //Only MS Pro supported?
		}

		if (status[2] & 0x15)
			return -1;

		return 0;
	}

	inline void _sdkWaitMsCed() {
		s32 reg;

		do {
			reg = _sdkReadMsRegInt();
		} while ((reg < 0) || ((reg & INT_REG_CED) == 0));
	}

	inline s32 _sdkWaitOpFinish() {
		s32 reg;

		do {
			reg = _sdkReadMsRegInt();
			
			if (reg < 0) {
				return -1;
			}
		} while((reg & INT_REG_BREQ) == 0);

		if ((reg & INT_REG_ERROR) != 0) {
			return -1;
		}

		return 0;
	}
}

void sdkMsInit() {
    iplSysregMsifClkEnable(ClkMs::IF0);
	iplSysregMsifBusClockEnable(BusClkMs::IF0);
	iplSysregMsifIoEnable(IoMs::IF0);
	iplSysregMsifResetDisable(ResetMs::IF0);

	_sdkMsReset();

	_sdkCheckMsStatus();
	_sdkWaitMsReady();
	_sdkWaitMsCed();
}

s32 sdkMsReadSector(u32 const sector, void *buf) {
	memoryK1(REG_MS_CMD) = CMD_EX_SET_CMD | 0x7;

	u8 cmd_buf[8];
	u16 const size = 1;

	cmd_buf[0] = 0x20;
	cmd_buf[1] = size >> 8;
	cmd_buf[2] = size;
	cmd_buf[3] = sector >> 24;
	cmd_buf[4] = sector >> 16;
	cmd_buf[5] = sector >> 8;
	cmd_buf[6] = sector;
	cmd_buf[7] = 0;

	if (_sdkSendDataAndSync(cmd_buf) < 0) {
		return -1;
	}

	_sdkWaitMsStatus();

	if (_sdkWaitOpFinish() < 0) {
		return -1;
	}

	memoryK1(REG_MS_CMD) = CMD_READ_PAGE_DATA | 512;

	if (_sdkReadMsData(buf, 512) < 0) {
		return -1;
	}

	if (_sdkWaitMsReady() < 0) {
		return -1;
	}

	_sdkWaitMsStatus();
	_sdkWaitMsCed();

	return 0;
}

s32 sdkMsWriteSector(u32 const sector, void const *buf) {
	memoryK1(REG_MS_CMD) = CMD_EX_SET_CMD | 0x7;

	u8 cmd_buf[8];
	u16 const size = 1;

	cmd_buf[0] = 0x21;
	cmd_buf[1] = size >> 8;
	cmd_buf[2] = size;
	cmd_buf[3] = sector >> 24;
	cmd_buf[4] = sector >> 16;
	cmd_buf[5] = sector >> 8;
	cmd_buf[6] = sector;
	cmd_buf[7] = 0;

	if (_sdkSendDataAndSync(cmd_buf) < 0) {
		return -1;
	}

	_sdkWaitMsStatus();

	if (_sdkWaitOpFinish() < 0) {
		return -1;
	}

	memoryK1(REG_MS_CMD) = CMD_WRIE_PAGE_DATA | 512;

	if (_sdkWriteMsData(buf, 512) < 0) {
		return -1;
	}

	if (_sdkWaitMsReady() < 0) {
		return -1;
	}

	_sdkWaitMsStatus();
	_sdkWaitMsCed();

	return 0;
}