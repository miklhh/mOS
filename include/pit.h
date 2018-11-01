// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#ifndef _PIT_H
#define _PIT_H

#include <system.h>

#define PIT_CHANNEL_0       0x40
#define PIT_CHANNEL_1       0x41
#define PIT_CHANNEL_2       0x42
#define PIT_CONTROL         0x43

// Routine for activating the PIT machinery.
void system_init_pit();

// Rouine for setting the PIT frequency.
void pit_set_freq(int hz);


#endif



