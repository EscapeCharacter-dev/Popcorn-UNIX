#include "memory.h"
#include "common.h"

static uint32_t page_directory[1024]    __attribute__((aligned(4096)));
uint32_t        page_table0[1024]       __attribute__((aligned(4096)));

static void setup_page_table(uint32_t *page_table, int directoryIndex)
{
    for (unsigned int i = 0; i < 1024; i++)
    {
        page_table[i] = (i * 0x1000) | 3;
    }

    page_directory[directoryIndex] = ((unsigned int)page_table) | 3;
}

static void blank_page_directory()
{
    for (int i = 0; i < 1024; i++)
    {
        page_directory[i] = 2;
    }
}

extern void loadPageDirectory(unsigned int *);
extern void enablePaging();

void setupPaging()
{
    blank_page_directory();
    setup_page_table(page_table0, 0);
    loadPageDirectory(page_directory);
    enablePaging();
}