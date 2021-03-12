/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014      */
/*-----------------------------------------------------------------------*/

#include "diskio.h"

#include <ms.h>
#include <string.h>


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (void)
{
	DSTATUS stat = 0;

	sdkMsInit();

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Partial Sector                                                   */
/*-----------------------------------------------------------------------*/
DWORD g_readLastSector = -1;
BYTE g_readSectorBuffer[512];

DRESULT disk_readp (
	BYTE* buff,		/* Pointer to the destination object */
	DWORD sector,	/* Sector number (LBA) */
	UINT offset,	/* Offset in the sector */
	UINT count		/* Byte count (bit15:destination) */
)
{
	DRESULT res = 0;

	if(sector == g_readLastSector) {
		memcpy(buff, &g_readSectorBuffer[offset], count);
	} else {
		g_readLastSector = sector; 
		sdkMsReadSector(sector, g_readSectorBuffer);
		memcpy(buff, &g_readSectorBuffer[offset], count);
	}

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Partial Sector                                                  */
/*-----------------------------------------------------------------------*/
BYTE g_writeSectorBuffer[512];
DWORD g_currentWriteSector = -1;
DRESULT disk_writep (
	const BYTE* buff,		/* Pointer to the data to be written, NULL:Initiate/Finalize write operation */
	DWORD sc				/* Sector number (LBA) or Number of bytes to send */
)
{
	DRESULT res;


	if (!buff) {
		if (sc) {
			// Initiate write process
			sdkMsReadSector(sc, g_writeSectorBuffer);
			g_currentWriteSector = sc;
		} else {
			// Finalize write process
			sdkMsWriteSector(g_currentWriteSector, g_writeSectorBuffer);
			g_currentWriteSector = -1;
		}
	} else {
		// Send data to the disk
		memcpy(g_writeSectorBuffer, buff, sc);
	}

	return res;
}

