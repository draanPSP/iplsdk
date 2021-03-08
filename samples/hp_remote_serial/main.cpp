#include <lowio.h>
#include <syscon.h>
#include <serial.h>

int main() {
	//Init communication with syscon
	iplSysconInit();

	//Tell syscon to apply power to HP Remote
	iplSysconCtrlHRPower(true);

	//Initialize HP Remote Serial
	sdkUartHpRemoteInit();

	for (u32 i = 0; i < 10; ++i) {
		printf("Hello world line %d\n", i);

		sdkWait(1 * 1000 * 1000);
	}

	//PSP will not shut down properly if we don't stop UART first
	sdkKernelUartHpRemoteSuspend();
	sdkKernelUartHpRemoteEnd();

	//Shut down the PSP
	iplSysconPowerStandby();

	return 0;
}