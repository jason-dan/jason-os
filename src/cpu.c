/****************************************************
 * cpu.c - Contains data-structures and algorithms  *
 * to simulate task switching.                      *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include "cpu.h"

struct CPU {int IP; char IR[1000]; int quanta};