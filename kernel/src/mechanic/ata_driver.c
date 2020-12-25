#include "ata_driver.h"
#include "../memutil.h"
#include "../console.h"

#define ATA_PORT       0x1F0
#define STATUS_BUSY     0x80
#define STATUS_READY    0x40
#define STATUS_DRQ      0x08
#define STATUS_DF       0x20
#define STATUS_ERR      0x01

static void waitBusy() { while(inb(0x1F7) & STATUS_BUSY); }
static void waitDRQ() { while(!(inb(0x1F7) & STATUS_READY)); }

void read_sector(unsigned long addr, unsigned long LBA, unsigned char sectorCount)
{
    waitBusy();
    outb(ATA_PORT + 6,   0xE0 | ((LBA >> 24) & 15));
    outb(ATA_PORT + 2,                 sectorCount);
    outb(ATA_PORT + 3,          (unsigned char)LBA);
    outb(ATA_PORT + 4,   (unsigned char)(LBA >> 8));
    outb(ATA_PORT + 5,  (unsigned char)(LBA >> 16));
    outb(ATA_PORT + 7,                        0x20);

    unsigned short *target = (unsigned short*)addr;

    for (int j = 0; j < sectorCount; j++)
    {
        waitBusy();
        waitDRQ();
        for (int i = 0; i < 256; i++)
            target[i] = inw(ATA_PORT);
        target += 256;
    }
}

void write_sector(unsigned long LBA, unsigned char sectorCount, unsigned long *pBytes)
{
    waitBusy();
    outb(ATA_PORT + 6,   0xE0 | ((LBA >> 24) & 15));
    outb(ATA_PORT + 2,                 sectorCount);
    outb(ATA_PORT + 3,          (unsigned char)LBA);
    outb(ATA_PORT + 4,   (unsigned char)(LBA >> 8));
    outb(ATA_PORT + 5,  (unsigned char)(LBA >> 16));
    outb(ATA_PORT + 7,                        0x30);

    for (int j = 0; j < sectorCount; j++)
    {
        waitBusy();
        waitDRQ();
        for (int i = 0; i < 256; i++)
            outl(0x1F0, pBytes[i]);
    }
}