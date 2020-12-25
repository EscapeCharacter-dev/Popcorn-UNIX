#ifndef SERIAL_H
#define SERIAL_H

int serial_received();
char read_serial();
void readline(char *buffer);
int is_transmit_empty();
void write_serial(char c);
void write_serial_string(const char *str);
int init_serial();

#endif