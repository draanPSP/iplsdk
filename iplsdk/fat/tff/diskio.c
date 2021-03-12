#include <string.h>
#include <tff.h>		/* Tiny-FatFs declarations */
#include <diskio.h>		/* Include file for user provided disk functions */
#include <ms.h>

DSTATUS disk_initialize(BYTE Drive) {
	sdkMsInit();
	return 0;
}

DSTATUS disk_status(BYTE Drive) {
	return 0;
}

DRESULT disk_read(BYTE Drive, BYTE* Buffer, DWORD SectorNumber, BYTE SectorCount) {
	while(SectorCount--) {
		if (sdkMsReadSector(SectorNumber,Buffer) < 0) {
			return RES_ERROR;
		}
		SectorNumber++;
		Buffer+=0x200;
	}
	return RES_OK;
}

#if _FS_READONLY == 0

DRESULT disk_write(BYTE Drive, const BYTE* Buffer, DWORD SectorNumber, BYTE SectorCount) {
	while(SectorCount--) {
		if (sdkMsWriteSector(SectorNumber,Buffer) < 0) {
			return RES_ERROR;
		}
		SectorNumber++;
		Buffer+=0x200;
	}
	return RES_OK;
}

DRESULT disk_ioctl (BYTE Drive, BYTE cmd, void* ptr) {
// CTRL_SYNC : Clean-up cached data      
	return RES_OK;
}

DWORD get_fattime (void) {
#if 0
	// do not RTC time
	u32 clk;
	pspSysconReadClock(&clk);
	Kprintf("pspSysconReadClock %08X\n",clk);
	pspSysconWriteClock(0x001FCEA4);
	pspSysconReadClock(&clk);
	Kprintf("pspSysconReadClock %08X\n",clk);

#endif

	return 0x00000000;

}

#endif


