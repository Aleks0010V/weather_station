/* 
 * File:   SD.h
 * Author: aleks
 *
 * Created on August 22, 2020, 6:51 PM
 */

#ifndef SD_H
#define	SD_H

#ifdef	__cplusplus
extern "C" {
#endif

#define CMD0 (0x40 + 0)  // GO_IDLE_STATE - Software reset
#define CMD1 (0x40 + 1)  // SEND_OP_COND - Initiate initialization process
#define CMD8 (0x40 + 8)  // SEND_IF_COND
#define CMD9 (0x40 + 9)  // SEND_CSD - Read CSD register
#define CMD10 (0x40 + 10)  // SEND_CID - Read CID register
#define CMD12 (0x40 + 12) // STOP_TRANSMISSION
#define CMD16 (0x40 + 16)  // SET_BLOCKLEN - Change R/W block size
#define CMD17 (0x40 + 17)  // READ_SINGLE_BLOCK
#define CMD18 (0x40 + 18)  // READ_MULTIPLE_BLOCK
#define CMD23 (0x40 + 23)  // SET_BLOCK_COUNT (only for MMC, ACMD23 - for SDC)
#define CMD24 (0x40 + 24)  // WRITE_BLOCK
#define CMD25 (0x40 + 25)  // WRITE_MULTIPLE_BLOCK
#define CMD55 (0x40 + 55)  // APP_CMD - Leading command of ACMD<n> command
#define CMD58 (0x40 + 58)  // READ_OCR


#ifdef	__cplusplus
}
#endif

#endif	/* SD_H */

