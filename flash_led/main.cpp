#include <cache.h>
#include <lowio.h>
#include <syscon.h>

int main() {
	//Init communication with syscon
	iplSysconInit();

	//Tell syscon to apply power to MS and WLAN LED
	iplSysconCtrlLED(SysconLed::WLAN, true);
	iplSysconCtrlLED(SysconLed::MS, true);

	//Configure GPIO to control the LED state
	iplGpioSetPortMode(GpioPort::WLAN_LED, GpioPortMode::OUTPUT);
	iplGpioSetPortMode(GpioPort::MS_LED, GpioPortMode::OUTPUT);

	//Poke watchdog :)
	const auto status = iplSysconNop();

	//Test Icache and Dcache clear
	iplKernelDcacheWritebackInvalidateAll();
	iplKernelIcacheInvalidateAll();

	//Flash LEDs every other second for roughly 10s!
	//Note: on 02g units the following code causes both
	//LEDs to flash at the same time, because WLAN LED logic
	//is inverted there.
	for (u32 i = 0; i < 10; ++i) {
		iplGpioPortSet(GpioPort::MS_LED);
		iplGpioPortClear(GpioPort::WLAN_LED);

		sdkWait(1 * 1000 * 1000);

		iplGpioPortClear(GpioPort::MS_LED);
		iplGpioPortSet(GpioPort::WLAN_LED);

		sdkWait(1 * 1000 * 1000);
	}

	//Prevent inelegant WLAN LED flash when shutting down
	iplSysconCtrlLED(SysconLed::WLAN, false);

	//Shut down the PSP
	iplSysconPowerStandby();

	return 0;
}