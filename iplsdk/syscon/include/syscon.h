#ifndef IPLSDK_SYSCON
#define IPLSDK_SYSCON

#include <psptypes.h>
#include <cstdint>

enum class SysconCmd : u8 {
	NOP = 0x0,
	GET_BARYON = 0x1,
	GET_KERNEL_DIGITAL_KEY = 0x7,

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

void iplSysconInit();

s32 iplSysconCtrlLED(SysconLed const led, bool enable);

s32 iplSysconCmdNoParam(SysconCmd const cmd);

inline s32 iplSysconNop() {
	return iplSysconCmdNoParam(SysconCmd::NOP);
}

inline s32 iplSysconPowerStandby() {
	return iplSysconCmdNoParam(SysconCmd::POWER_STANDBY);
}

inline s32 iplSysconPowerSuspend() {
	return iplSysconCmdNoParam(SysconCmd::POWER_SUSPEND);
}

#endif //IPLSDK_SYSCON