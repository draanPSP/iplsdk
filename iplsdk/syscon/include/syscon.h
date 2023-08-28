#ifndef IPLSDK_SYSCON
#define IPLSDK_SYSCON

#include <cstdint>

enum class SysconCmd : std::uint8_t {
	NOP = 0x0,
	GET_BARYON = 0x1,
	GET_KERNEL_DIGITAL_KEY = 0x7,

	GET_WAKE_UP_FACTOR = 0xE,

	RESET_DEVICE = 0x32,
	CTRL_HR_POWER = 0x34,
	POWER_STANDBY = 0x35,
	POWER_SUSPEND = 0x36,

	CTRL_LED = 0x47,

	CTRL_MS_POWER = 0x4C,
	CTRL_WLAN_POWER = 0x4D,
};

enum class SysconLed : std::uint8_t {
	MS,
	WLAN,
	POWER,
	BT
};

constexpr inline std::uint32_t TX_CMD = 0;
constexpr inline std::uint32_t TX_LEN = 1;

constexpr std::uint32_t TX_DATA(std::uint32_t const i) { return 2 + i; };

constexpr inline std::uint32_t RX_STATUS = 0;
constexpr inline std::uint32_t RX_LEN = 1;
constexpr inline std::uint32_t RX_RESPONSE = 2;

constexpr std::uint32_t RX_DATA(std::uint32_t const i) { return 3 + i; };

void iplSysconInit();
std::uint32_t iplSysconGetBaryonVersion();

std::int32_t sdkSysconTransmitReceive(std::uint8_t *tx, std::uint8_t *rx);

std::int32_t _iplSysconCommonRead(std::uint32_t *ptr, SysconCmd const cmd);
std::int32_t _iplSysconCommonWrite(std::uint32_t const val, SysconCmd const cmd, std::uint32_t const size);

std::int32_t _iplSysconCmdNoParam(SysconCmd const cmd);

inline std::int32_t iplSysconNop() {
	return _iplSysconCmdNoParam(SysconCmd::NOP);
}

inline std::int32_t iplSysconGetWakeUpFactor(std::uint32_t *factorPtr) {
	return _iplSysconCommonRead(factorPtr, SysconCmd::GET_WAKE_UP_FACTOR);
}

inline std::int32_t iplSysconCtrlHRPower(bool const enable) {
	return _iplSysconCommonWrite(enable, SysconCmd::CTRL_HR_POWER, 3);
}

inline std::int32_t iplSysconResetDevice(std::uint32_t reset, std::uint32_t const mode) {
	if (reset == 1) {
		if (mode == 0 || mode == 2) {
			reset |= 0x40;
		} else {
			return -1;
		}
	} else if (mode != 0) {
		reset |= 0x80;
	}
	
	return _iplSysconCommonWrite(reset, SysconCmd::RESET_DEVICE, 3);
}

inline std::int32_t iplSysconPowerStandby() {
	return _iplSysconCmdNoParam(SysconCmd::POWER_STANDBY);
}

inline std::int32_t iplSysconPowerSuspend() {
	return _iplSysconCmdNoParam(SysconCmd::POWER_SUSPEND);
}

std::int32_t iplSysconCtrlLED(SysconLed const led, bool const enable);

inline std::int32_t iplSysconCtrlMsPower(bool const enable) {
	return _iplSysconCommonWrite(enable, SysconCmd::CTRL_MS_POWER, 3);
}

inline std::int32_t iplSysconCtrlWlanPower(bool const enable) {
	return _iplSysconCommonWrite(enable, SysconCmd::CTRL_WLAN_POWER, 3);
}

#endif //IPLSDK_SYSCON