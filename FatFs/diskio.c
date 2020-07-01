/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h" /* FatFs lower layer API */
#include "./sdio/bsp_sdio_sd.h"
#include "string.h"

/* Definitions of physical drive number for each drive */
#define SDIO_SD_CARD 0 /* Example: Map ATA harddisk to physical drive 0 */

#define SD_BLOCKSIZE 512
extern SD_CardInfo SDCardInfo;
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv)
	{

	case SDIO_SD_CARD:
		stat &= ~STA_NOINIT;
		break;

	default:
		stat = STA_NOINIT;
	}
	return stat;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
	switch (pdrv)
	{
	case SDIO_SD_CARD:
		if (SD_Init() == SD_OK)
		{
			stat &= ~STA_NOINIT;
		}
		else
		{
			stat = STA_NOINIT;
		}
		break;

	default:
		stat = STA_NOINIT;
	}
	return stat;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
	BYTE pdrv,	  /* Physical drive nmuber to identify the drive */
	BYTE *buff,	  /* Data buffer to store read data */
	DWORD sector, /* Sector address in LBA */
	UINT count	  /* Number of sectors to read */
)
{
	DRESULT res = RES_ERROR;

	switch (pdrv)
	{
	case SDIO_SD_CARD:

		if ((DWORD)buff & 3)
		{
			DRESULT res = RES_OK;
			DWORD scratch[SD_BLOCKSIZE / 4];

			while (count--)
			{
				res = disk_read(SDIO_SD_CARD, (void *)scratch, sector++, 1);

				if (res != RES_OK)
				{
					break;
				}
				memcpy(buff, scratch, SD_BLOCKSIZE);
				buff += SD_BLOCKSIZE;
			}
			return res;
		}

		if (SD_ReadMultiBlocks(buff, sector * SD_BLOCKSIZE, SD_BLOCKSIZE, count) == SD_OK)
		{
			res = RES_OK;
		}
		else
		{
			res = RES_ERROR;
		}
		if (SD_WaitReadOperation() == SD_OK)
		{
			res = RES_OK;
		}
		else
		{
			res = RES_ERROR;
		}
		while (SD_GetStatus() != SD_TRANSFER_OK)
			;
		break;

	default:
		res = RES_ERROR;
	}

	return res;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write(
	BYTE pdrv,		  /* Physical drive nmuber to identify the drive */
	const BYTE *buff, /* Data to be written */
	DWORD sector,	  /* Sector address in LBA */
	UINT count		  /* Number of sectors to write */
)
{
	DRESULT res = RES_ERROR;
	if (!count)
	{
		return RES_PARERR; /* Check parameter */
	}
	switch (pdrv)
	{
	case SDIO_SD_CARD:

		if ((DWORD)buff & 3)
		{
			DRESULT res = RES_OK;
			DWORD scratch[SD_BLOCKSIZE / 4];

			while (count--)
			{
				memcpy(scratch, buff, SD_BLOCKSIZE);
				res = disk_write(SDIO_SD_CARD, (void *)scratch, sector++, 1);
				if (res != RES_OK)
				{
					break;
				}
				buff += SD_BLOCKSIZE;
			}
			return res;
		}

		if (SD_WriteMultiBlocks((uint8_t *)buff, sector * SD_BLOCKSIZE, SD_BLOCKSIZE, count) == SD_OK)
		{
			res = RES_OK;
		}
		else
		{
			res = RES_ERROR;
		}
		if (SD_WaitWriteOperation() == SD_OK)
		{
			res = RES_OK;
		}
		else
		{
			res = RES_ERROR;
		}
		while (SD_GetStatus() != SD_TRANSFER_OK)
			;
		break;

	default:
		res = RES_ERROR;
	}

	return res;
}
#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl(
	BYTE pdrv, /* Physical drive nmuber (0..) */
	BYTE cmd,  /* Control code */
	void *buff /* Buffer to send/receive control data */
)
{
	DRESULT res = RES_ERROR;

	switch (pdrv)
	{
	case SDIO_SD_CARD:
		switch (cmd)
		{
		/* 扇区数量 */
		case GET_SECTOR_COUNT:
			*(DWORD *)buff = SDCardInfo.CardCapacity / SDCardInfo.CardBlockSize;
			break;
		/* 扇区大小  */
		case GET_SECTOR_SIZE:
			*(WORD *)buff = SD_BLOCKSIZE;
			break;
		/* 同时擦除扇区个数 */
		case GET_BLOCK_SIZE:
			*(DWORD *)buff = 1;
			break;
		}

		res = RES_OK;
		break;

	default:
		res = RES_ERROR;
	}

	return res;
}
#endif
