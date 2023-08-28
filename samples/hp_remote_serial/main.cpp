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

	auto const tachyonVer = iplSysregGetTachyonVersion();
	auto const baryonVer = iplSysconGetBaryonVersion();

	printf("Your PSP's bootrom timestamp is 0x%x\n", sdkGetBootromTimestampFromRom());
	printf("Your PSP's Tachyon version is 0x%x\n", tachyonVer);
	printf("Your PSP's Baryon version is 0x%x\n", baryonVer);
	printf("Your PSP's model is %d\n", sdkKernelGetModel(tachyonVer, baryonVer));

	for (std::uint32_t i = 0; i < 10; ++i) {
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