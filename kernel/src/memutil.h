#ifndef MEMUTIL_H
#define MEMUTIL_H

unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
unsigned char *memset(unsigned char *dest, unsigned char src, int count);
unsigned short *memsetw(unsigned short *dest, unsigned short src, int count);
int strlen(const char *str);
unsigned char inb(unsigned short port);
unsigned short inw(unsigned short port);
void outb(unsigned short port, unsigned char val);
void outl(unsigned short port, unsigned long val);

#endif