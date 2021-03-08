#include <lowio.h>
#include <syscon.h>
#include <ctrl.h>

int main() {
	//Init communication with syscon
	iplSysconInit();

	//Tell syscon to apply power to MS and WLAN LED
	iplSysconCtrlLED(SysconLed::WLAN, true);
	iplSysconCtrlLED(SysconLed::MS, true);

	//Configure GPIO to control the LED state
	iplGpioSetPortMode(GpioPort::WLAN_LED, GpioPortMode::OUTPUT);
	iplGpioSetPortMode(GpioPort::MS_LED, GpioPortMode::OUTPUT);

	SceCtrlData pad;

	//Initially turn off WLAN LED
	iplGpioPortSet(GpioPort::WLAN_LED);
	
	while (true) {
		iplReadBufferPositive(&pad);

		//Cross turns off PSP
		if (pad.Buttons & PSP_CTRL_CROSS) {
			break;
		}

		//Light MS LED when L is held
		if (pad.Buttons & PSP_CTRL_LTRIGGER) {
			iplGpioPortSet(GpioPort::MS_LED);
		} else {
			iplGpioPortClear(GpioPort::MS_LED);
		}

		//Light WLAN LED when R is held
		if (pad.Buttons & PSP_CTRL_RTRIGGER) {
			iplGpioPortClear(GpioPort::WLAN_LED);
		} else {
			iplGpioPortSet(GpioPort::WLAN_LED);
		}
	}

	//Prevent inelegant WLAN LED flash when shutting down
	iplSysconCtrlLED(SysconLed::WLAN, false);

	//Shut down the PSP
	iplSysconPowerStandby();

	return 0;
}