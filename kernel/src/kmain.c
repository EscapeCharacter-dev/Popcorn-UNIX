#include "console.h"
#include "descriptor_tables.h"
#include "keyboard.h"
#include "command_mgr.h"
#include "serial.h"
#include "memory.h"
#include "mechanic/ata_driver.h"

extern int kbIsLocked_KERNEL;

void kmain()
{
    setupPaging();
    clearScreen();
    installGdt();
    installIdt();
    timer_install();
    keyboard_install();
    install_cmdmgr();
    asm("sti");

    kbIsLocked_KERNEL = 1;

    putstring("Starting Popcorn...\n");
    init_serial();
    putstring("Color test...\n");
    int tabI = 0;
    for (int i = 0; i <= 15; i++)
    {
        if (tabI == 8)
        {
            putchar('\n');
            tabI = 0;
        }
        setcolor(15 - i, i);
        putchar('A' + i);
        tabI++;
    }
    setcolor(7, 0);
    putchar('\n');

    write_serial_string("SOC0;DEVICE='popcorn';");
    while(1)
    { char c = read_serial(); putchar_comm(c); }
}