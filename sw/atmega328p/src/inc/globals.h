#ifndef _GLOBALS_H_
#define _GLOBALS_H_

// Avoid including <stdbool.h>
#define TRUE	1
#define FALSE	0

// C Libraries
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// AVR Libraries
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// Local Headers
#include "macros.h"
#include "oven_control.h"
#include "ms_timer.h"
#include "max31855.h"
#include "uart.h"
#include "cmd.h"

#endif
