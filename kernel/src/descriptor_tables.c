#include "memutil.h"
#include "console.h"
#include "common.h"
#include "descriptor_tables.h"

typedef struct gdtEntry_t
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} packed gdtEntry;

typedef struct gdtPtr_t
{
    uint16_t limit;
    uint32_t base;
} packed gdtPtr;

gdtEntry gdt[3] = {0};
gdtPtr   gdt_ptr = {0};

extern void gdtFlush();

#define gdtSetGateEmpty gdtSetGate(0, 0, 0, 0, 0)

void gdtSetGate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 255;
    gdt[num].base_high = (base >> 24) & 255;
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 15);
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void installGdt()
{
    gdt_ptr.limit = (sizeof(gdtEntry) * 3) - 1;
    gdt_ptr.base = (uint32_t)&gdt;
    gdtSetGateEmpty;
    gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    gdtFlush();
}

typedef struct idtEntry_t
{
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} packed idtEntry;

typedef struct idtPtr_t
{
    uint16_t limit;
    uint32_t base;
} packed idtPtr;

idtEntry idtEntries[256] = {0};
idtPtr   idt_ptr = {0};

extern void idtFlush();

#define ISR(x) isr##x
#define EISR(x) extern void isr##x
#define EIRQ(x) extern void irq##x
#define fp(x) (uint32_t)&(##x)

void idtSetGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    idtEntries[num].base_low = base & 0xFFFF;
    idtEntries[num].base_high = (base >> 16) & 0xFFFF;
    idtEntries[num].sel = sel;
    idtEntries[num].flags = flags;
    idtEntries[num].always0 = 0;
}

EISR(0);
EISR(1);
EISR(2);
EISR(3);
EISR(4);
EISR(5);
EISR(6);
EISR(7);
EISR(8);
EISR(9);
EISR(10);
EISR(11);
EISR(12);
EISR(13);
EISR(14);
EISR(15);
EISR(16);
EISR(17);
EISR(18);
EISR(19);
EISR(20);
EISR(21);
EISR(22);
EISR(23);
EISR(24);
EISR(25);
EISR(26);
EISR(27);
EISR(28);
EISR(29);
EISR(30);
EISR(31);
EIRQ(0);
EIRQ(1);
EIRQ(2);
EIRQ(3);
EIRQ(4);
EIRQ(5);
EIRQ(6);
EIRQ(7);
EIRQ(8);
EIRQ(9);
EIRQ(10);
EIRQ(11);
EIRQ(12);
EIRQ(13);
EIRQ(14);
EIRQ(15);
EIRQ(16);

#define fpIdt(x) (uint32_t)&isr##x
#define fpIrq(x) (uint32_t)&irq##x

void installIdt()
{
    idt_ptr.limit = (sizeof(idtEntry) * 256) - 1;
    idt_ptr.base = (uint32_t)&idtEntries;

    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    idtSetGate(0, fpIdt(0), 8, 0x8E);
    idtSetGate(1, fpIdt(1), 8, 0x8E);
    idtSetGate(2, fpIdt(2), 8, 0x8E);
    idtSetGate(3, fpIdt(3), 8, 0x8E);
    idtSetGate(4, fpIdt(4), 8, 0x8E);
    idtSetGate(5, fpIdt(5), 8, 0x8E);
    idtSetGate(6, fpIdt(6), 8, 0x8E);
    idtSetGate(7, fpIdt(7), 8, 0x8E);
    idtSetGate(8, fpIdt(8), 8, 0x8E);
    idtSetGate(9, fpIdt(9), 8, 0x8E);
    idtSetGate(10, fpIdt(10), 8, 0x8E);
    idtSetGate(11, fpIdt(11), 8, 0x8E);
    idtSetGate(12, fpIdt(12), 8, 0x8E);
    idtSetGate(13, fpIdt(13), 8, 0x8E);
    idtSetGate(14, fpIdt(14), 8, 0x8E);
    idtSetGate(15, fpIdt(15), 8, 0x8E);
    idtSetGate(16, fpIdt(16), 8, 0x8E);
    idtSetGate(17, fpIdt(17), 8, 0x8E);
    idtSetGate(18, fpIdt(18), 8, 0x8E);
    idtSetGate(19, fpIdt(19), 8, 0x8E);
    idtSetGate(20, fpIdt(20), 8, 0x8E);
    idtSetGate(21, fpIdt(21), 8, 0x8E);
    idtSetGate(22, fpIdt(22), 8, 0x8E);
    idtSetGate(23, fpIdt(23), 8, 0x8E);
    idtSetGate(24, fpIdt(24), 8, 0x8E);
    idtSetGate(25, fpIdt(25), 8, 0x8E);
    idtSetGate(26, fpIdt(26), 8, 0x8E);
    idtSetGate(27, fpIdt(27), 8, 0x8E);
    idtSetGate(28, fpIdt(28), 8, 0x8E);
    idtSetGate(29, fpIdt(29), 8, 0x8E);
    idtSetGate(30, fpIdt(30), 8, 0x8E);
    idtSetGate(31, fpIdt(31), 8, 0x8E);
    idtSetGate(32, fpIrq(0), 8, 0x8E);
    idtSetGate(33, fpIrq(1), 8, 0x8E);
    idtSetGate(34, fpIrq(2), 8, 0x8E);
    idtSetGate(35, fpIrq(3), 8, 0x8E);
    idtSetGate(36, fpIrq(4), 8, 0x8E);
    idtSetGate(37, fpIrq(5), 8, 0x8E);
    idtSetGate(38, fpIrq(6), 8, 0x8E);
    idtSetGate(39, fpIrq(7), 8, 0x8E);
    idtSetGate(40, fpIrq(8), 8, 0x8E);
    idtSetGate(41, fpIrq(9), 8, 0x8E);
    idtSetGate(42, fpIrq(10), 8, 0x8E);
    idtSetGate(43, fpIrq(11), 8, 0x8E);
    idtSetGate(44, fpIrq(12), 8, 0x8E);
    idtSetGate(45, fpIrq(13), 8, 0x8E);
    idtSetGate(46, fpIrq(14), 8, 0x8E);
    idtSetGate(47, fpIrq(15), 8, 0x8E);
    idtSetGate(48, fpIrq(16), 8, 0x8E);

    idtFlush();
}

void *irqRoutines[224] = {0};

const char *exception_messages[] =
{
    "Divison By Zero",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun (Obsolete)",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "[RESERVED]",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "Security Exception",
    "[RESERVED]",
};

void isr_handler(registers *regs)
{
    if (regs->int_no < 32)
    {
        putstring(exception_messages[regs->int_no]);
        putstring("\nSystem halted!");
        while (1) { asm volatile("cli"); asm volatile("hlt"); }
    }
}

void irqInstallHandler(int irqNum, void (*handler)(registers *regs))
{
    irqRoutines[irqNum] = handler;
}

void irqUninstallHandler(int irqNum)
{
    irqRoutines[irqNum] = 0;
}

void irq_handler(registers *regs)
{
    void (*handler)(registers *regs);
    handler = irqRoutines[regs->int_no - 32];
    if (handler) handler(regs);
    if (regs->int_no >= 40) outb(0xA0, 0x20);
    outb(0x20, 0x20);
}