#include "serial.h"
#include "memutil.h"
#include "console.h"

static int initialized = 0;

#define PORT 0x3F8

int init_serial()
{
    putstring("Initializing serial...\n");
    outb(PORT + 1, 0x00);
    outb(PORT + 3, 0x80);
    outb(PORT + 0, 0x03);
    outb(PORT + 1, 0x00);
    outb(PORT + 3, 0x03);
    outb(PORT + 2, 0xC7);
    outb(PORT + 4, 0x0B);
    outb(PORT + 4, 0x1E);
    outb(PORT + 0, 0xAE);
    if (inb(PORT + 0) != 0xAE)
        { putstring("Warning: Serial port is faulty or doesn't work!\n"); initialized = 1; return 1; }
    outb(PORT + 4, 0x0F);
    initialized = 1;
    return 0;
}

int serial_received()
{
    return inb(PORT + 5) & 1;
}

char read_serial()
{
    while (serial_received() == 0);
    return inb(PORT);
}

void readline(char *buffer)
{
    char current;
    int index = 0;
    while ((current = read_serial()) != '\n' || current != '\0')
        buffer[index++] = current;
    buffer[index] = '\0';
}

int is_transmit_empty()
{
    return inb(PORT + 5) & 0x20;
}

void write_serial(char a)
{
    while (is_transmit_empty() == 0);
    outb(PORT, a);
}

void write_serial_string(const char *str)
{
    for (int i = 0; i < strlen(str); i++)
        write_serial(str[i]);
}