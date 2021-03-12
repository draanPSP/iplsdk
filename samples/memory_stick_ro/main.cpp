#include <syscon.h>
#include <serial.h>
#include <pff.h>

constexpr inline u32 buffer_size = 1024;

FATFS fs;

u8 file_buffer[buffer_size];

void read_tm_config() {
	//Initialize Memory Stick & FAT Filesystem
	if (pf_mount(&fs) != FR_OK) {
		printf("pf_mount failed!\n");
		return;
	}

	//Try to open TM config file
	if (pf_open("/TM/config.txt") != FR_OK) {
		printf("pf_open failed!\n");
		return;
	}

	auto size = fs.fsize;

	printf("File size: %d bytes\n", size);

	u32 bytes_read;

	if (pf_read(file_buffer, buffer_size-1, &bytes_read) != FR_OK) {
		printf("pf_read failed!\n");
		return;
	}

	file_buffer[bytes_read] = '\0';

	printf("Read %d bytes\n", bytes_read);
	printf("Contents: %s\n", file_buffer);
}

int main() {
	//Init communication with syscon
	iplSysconInit();

	//Tell syscon to apply power to HP Remote
	iplSysconCtrlHRPower(true);

	//Initialize HP Remote Serial
	sdkUartHpRemoteInit();

	//Try to read the config file and print it
	read_tm_config();

	printf("Shutdown\n");

	//PSP will not shut down properly if we don't stop UART first
	sdkKernelUartHpRemoteSuspend();
	sdkKernelUartHpRemoteEnd();

	//Shut down the PSP
	iplSysconPowerStandby();

	return 0;
}