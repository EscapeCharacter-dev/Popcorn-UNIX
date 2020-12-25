#include "descriptor_tables.h"
#include "common.h"
#include "console.h"

static void command_mgr(registers *regs)
{
    putstring("Called");
    switch (regs->eax)
    {
        case 0: //print
            putstring("Hello, World!\n");
            break;
    }
}

void install_cmdmgr()
{
    irqInstallHandler(16, command_mgr);
}