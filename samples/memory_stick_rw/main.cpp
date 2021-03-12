#include <syscon.h>
#include <serial.h>
#include <tff.h>

FATFS fs;
FIL file;

auto const bootrom_address = reinterpret_cast<u8 const*>(0xBFC00000);
constexpr inline u32 bootrom_size = 0x1000;

void dump_bootrom() {
	//Initialize Memory Stick & FAT Filesystem
	if (f_mount(0, &fs) != FR_OK) {
		printf("f_mount failed!\n");
		return;
	}

	//Create bootrom file
	if (f_open(&file, "bootrom.bin", FA_WRITE|FA_CREATE_ALWAYS) != FR_OK) {
		printf("f_open failed!\n");
		return;
	}

	u16 bytes_written;

	//Write bootrom to file
	if (f_write(&file, bootrom_address, bootrom_size, &bytes_written) != FR_OK) {
		printf("f_write failed!\n");
		return;
	}

	//Close file
	if (f_close(&file) != FR_OK) {
		printf("f_close failed!\n");
		return;
	}
}

int main() {
	//Init communication with syscon
	iplSysconInit();

	//Tell syscon to apply power to HP Remote
	iplSysconCtrlHRPower(true);

	//Initialize HP Remote Serial
	sdkUartHpRemoteInit();

	//Try to dump bootrom to file
	dump_bootrom();

	printf("Shutdown\n");

	//PSP will not shut down properly if we don't stop UART first
	sdkKernelUartHpRemoteSuspend();
	sdkKernelUartHpRemoteEnd();

	//Shut down the PSP
	iplSysconPowerStandby();

	return 0;
}