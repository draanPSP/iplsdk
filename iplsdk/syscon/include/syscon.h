#ifndef IPLSDK_SYSCON
#define IPLSDK_SYSCON

#include <psptypes.h>

enum class SysconCmd : u8 {
	NOP = 0x0,
	GET_BARYON = 0x1,
	GET_KERNEL_DIGITAL_KEY = 0x7,

	GET_WAKE_UP_FACTOR = 0xE,

	CTRL_HR_POWER = 0x34,
	POWER_STANDBY = 0x35,
	POWER_SUSPEND = 0x36,

	CTRL_LED = 0x47,

	CTRL_MS_POWER = 0x4C,
	CTRL_WLAN_POWER = 0x4D,
};

enum class SysconLed : u8 {
	MS,
	WLAN,
	POWER,
	BT
};

constexpr inline u32 TX_CMD = 0;
constexpr inline u32 TX_LEN = 1;

constexpr u32 TX_DATA(u32 const i) { return 2 + i; };

constexpr inline u32 RX_STATUS = 0;
constexpr inline u32 RX_LEN = 1;
constexpr inline u32 RX_RESPONSE = 2;

constexpr u32 RX_DATA(u32 const i) { return 3 + i; };

void iplSysconInit();
u32 iplSysconGetBaryonVersion();

s32 sdkSysconTransmitReceive(u8 *tx, u8 *rx);

s32 _iplSysconCommonRead(u32 *ptr, SysconCmd const cmd);
s32 _iplSysconCommonWrite(u32 const val, SysconCmd const cmd, u32 const size);

s32 _iplSysconCmdNoParam(SysconCmd const cmd);

inline s32 iplSysconNop() {
	return _iplSysconCmdNoParam(SysconCmd::NOP);
}

inline s32 iplSysconGetWakeUpFactor(u32 *factorPtr) {
	return _iplSysconCommonRead(factorPtr, SysconCmd::GET_WAKE_UP_FACTOR);
}

inline s32 iplSysconCtrlHRPower(bool const enable) {
	return _iplSysconCommonWrite(enable, SysconCmd::CTRL_HR_POWER, 3);
}

inline s32 iplSysconPowerStandby() {
	return _iplSysconCmdNoParam(SysconCmd::POWER_STANDBY);
}

inline s32 iplSysconPowerSuspend() {
	return _iplSysconCmdNoParam(SysconCmd::POWER_SUSPEND);
}

s32 iplSysconCtrlLED(SysconLed const led, bool const enable);

inline s32 iplSysconCtrlMsPower(bool const enable) {
	return _iplSysconCommonWrite(enable, SysconCmd::CTRL_MS_POWER, 3);
}

inline s32 iplSysconCtrlWlanPower(bool const enable) {
	return _iplSysconCommonWrite(enable, SysconCmd::CTRL_WLAN_POWER, 3);
}

#endif //IPLSDK_SYSCON