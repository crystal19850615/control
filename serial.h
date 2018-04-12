#ifndef SERIAL_H
#define SERIAL_H

#include <termios.h>
#include <string.h>

int set_interface_attribs (int fd, int speed, int parity);
void set_blocking (int fd, int should_block);

#endif // SERIAL_H
