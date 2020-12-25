#ifndef DESCRIPTOR_TABLES_H
#define DESCRIPTOR_TABLES_H

typedef struct registers_t
{
    unsigned long gs, fs, es, ds,
    edi, esi, ebp, esp, ebx, edx, ecx, eax,
    int_no, err_core, eip, cs, eflags, useresp,
    ss;
} registers;

void gdtSetGate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void installGdt();
void installIdt();
void irqInstallHandler(int irqNum, void (*handler)(registers *regs));
void irqUninstallHandler(int irqNum);
void timer_install();
void timer_wait(int ticks_);
void timer_phase(int hertz);

#endif