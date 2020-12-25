#ifndef CONSOLE_H
#define CONSOLE_H

void clearScreen();
void putchar(char c);
void putstring(const char *text);
void setcolor(unsigned char fore, unsigned char back);
void putint10(int num);
void putint16(int num);
void putchar_comm(char c);

#endif