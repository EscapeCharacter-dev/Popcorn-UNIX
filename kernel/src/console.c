#include "memutil.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

unsigned short *videoBuffer = (unsigned short*)0xB8000;
int attrib = 7, crs_x = 0, crs_y = 0;

static unsigned int getBlank() { return 0x20 | (attrib << 8); }

static void move_csr()
{
    unsigned int temp = crs_y * SCREEN_WIDTH + crs_x;
    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, (unsigned char)temp);
}



void scroll()
{
    unsigned int blank = getBlank();

    if (crs_y >= SCREEN_HEIGHT)
    {
        unsigned int temp = crs_y - SCREEN_HEIGHT+ 1;

        unsigned char *src0 = (unsigned char*)(videoBuffer + temp * SCREEN_WIDTH);

        unsigned char *dest0 = (unsigned short*)(videoBuffer + (SCREEN_HEIGHT - temp) * SCREEN_WIDTH);

        unsigned int amount = (SCREEN_HEIGHT - temp) * SCREEN_WIDTH * 2;
        
        memcpy((unsigned char*)videoBuffer, src0, amount);
        memsetw(dest0, blank, SCREEN_WIDTH);
        crs_y = SCREEN_HEIGHT - 1;
    }
}



void clearScreen()
{
    unsigned int blank = getBlank();
    for (int i = 0; i < SCREEN_HEIGHT; i++)
        memsetw(videoBuffer + i * SCREEN_WIDTH, blank, SCREEN_WIDTH);
    crs_x = 0, crs_y = 0;
    move_csr();
}

void putchar(char c)
{
    unsigned int att = attrib << 8;

    if (c == 8) {if (crs_x != 0) crs_x--;}
    else if (c == 9) crs_x = (crs_x + 8) & ~(8 - 1);
    else if (c == '\r') crs_x = 0;
    else if (c == '\n') { crs_x = 0; crs_y++; }
    else if (c >= ' ')
    {
        unsigned short *where = videoBuffer + (crs_y * SCREEN_WIDTH + crs_x);
        *where = c | att;
        crs_x++;
    }
    if (crs_x >= SCREEN_WIDTH)
    {
        crs_x = 0;
        crs_y++;
    }

    scroll();
    move_csr();
}

void putchar_comm(char c)
{
    unsigned int att = attrib << 8;

    if (c == 8 || c == 127) {if (crs_x != 0) { crs_x--; putchar(' '); crs_x--; }}
    else if (c == 9) crs_x = (crs_x + 8) & ~(8 - 1);
    else if (c == '\n' || c == '\r') { crs_x = 0; crs_y++; }
    else if (c >= ' ')
    {
        unsigned short *where = videoBuffer + (crs_y * SCREEN_WIDTH + crs_x);
        *where = c | att;
        crs_x++;
    }
    if (crs_x >= SCREEN_WIDTH)
    {
        crs_x = 0;
        crs_y++;
    }

    scroll();
    move_csr();
}

void putstring(const char *text)
{
    for (int i = 0; i < strlen(text); i++) putchar(text[i]);
}

void setcolor(unsigned char fore, unsigned char back)
{
    attrib = (back << 4) | (fore & 15);
}

static char *itoa(int value, char *str, int base)
{
    char *rc;
    char *ptr;
    char *low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

void putint10(int num)
{
    char str[10];
    itoa(num, str, 10);
    putstring(str);
}

void putint16(int num)
{
    char str[8];
    itoa(num, str, 8);
    putstring(str);
}