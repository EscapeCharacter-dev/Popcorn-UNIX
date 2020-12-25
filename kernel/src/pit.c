#include "common.h"
#include "descriptor_tables.h"
#include "memutil.h"
#include "console.h"

void timer_phase(int hertz)
{
    int divisor = 1193180 / hertz;
    outb(0x43, 0x36);
    outb(0x40, divisor & 255);
    outb(0x40, divisor >> 8);
}

static int ticks = 0;

static void timer_handler(registers *regs)
{
    ticks++;
}

void timer_wait(int ticks_)
{
    unsigned long eticks;
    eticks = ticks + ticks_;
    while (ticks < eticks);
}

void timer_install()
{
    irqInstallHandler(0, timer_handler);
}