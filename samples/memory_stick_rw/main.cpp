#include <syscon.h>
#include <serial.h>
#include <ff.h>

FATFS fs;
FIL file;

auto const bootrom_address = reinterpret_cast<std::uint8_t const*>(0xBFC00000);
constexpr inline std::uint32_t bootrom_size = 0x1000;

void dump_bootrom() {
	//Initialize Memory Stick & FAT Filesystem
	if (f_mount(&fs, "ms0:", 0) != FR_OK) {
		printf("f_mount failed!\n");
		return;
	}
	printf("mounted\n");
	// list dir
	FRESULT res;
	DIR dir;
	UINT i;
	static FILINFO fno;
	static char path[256] = "/";

	printf("listing dir\n");
	res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                printf("D %s\n",  fno.fname);
            } else {                                       /* It is a file. */
                printf("F %s\n", fno.fname);
            }
        }
        f_closedir(&dir);
    } else {
		printf("Could not open dir '/'\n");
	}
	printf("done listing dir\n");
	//Create bootrom file
	if (f_open(&file, "ms0:/bootrom.bin", FA_WRITE|FA_CREATE_ALWAYS) != FR_OK) {
		printf("f_open failed!\n");
		return;
	}

	std::uint32_t bytes_written;
	printf("writing file\n");
	//Write bootrom to file
	if (f_write(&file, bootrom_address, bootrom_size, &bytes_written) != FR_OK) {
		printf("f_write failed!\n");
		return;
	}

	//Close file
	if ((res = f_close(&file)) != FR_OK) {
		printf("f_close failed with %d!\n", res);
		return;
	}
	printf("done\n");
}

int main() {
	//Init communication with syscon
	iplSysconInit();

	//Tell syscon to apply power to HP Remote
	iplSysconCtrlHRPower(true);

	//Initialize HP Remote Serial
	sdkUartHpRemoteInit();

	printf("attempting to dump bootrom to ms0:/bootrom.bin\n");
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