#ifndef ATA_DRIVER_H
#define ATA_DRIVER_H

void read_sector(unsigned long addr, unsigned long LBA, unsigned char sectorCount);
void write_sector(unsigned long LBA, unsigned char sectorCount, unsigned long *pBytes);

#endif