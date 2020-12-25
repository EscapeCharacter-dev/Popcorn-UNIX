unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

unsigned char *memset(unsigned char *dest, unsigned char src, int count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = src;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short src, int count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = src;
    return dest;
}

int strlen(const char *str)
{
    unsigned int count = 0;
    for (count = 0; str[count] != '\0'; count++);
    return count;
}

unsigned char inb(unsigned short port)
{
    unsigned char ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

void outb(unsigned short port, unsigned char val)
{
    asm volatile("outb %1, %0" : : "dN"(port), "a"(val));
}

void outl(unsigned short port, unsigned long val)
{
    asm volatile("outl %1, %0" : : "dN"(port), "a"(val));
}

unsigned short inw(unsigned short port)
{
    unsigned short ret;
    asm volatile("inw %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}