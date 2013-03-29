#ifndef _SERIAL_IO_H_
#define _SERIAL_IO_H_

#include "common.h"

void serial_init();
void debug_msg(char *msg);
void serial_task(void *pvParameters) NORETURN;

#define TERM_CMD_LENGTH		20

#endif				// SERIAL_IO_H_
