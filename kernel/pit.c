// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#include <system.h>
#include <io.h>
#include <pit.h>
#include <irq.h>
#include <kstdio.h>

#define PIT_FREQUENCY   1000

// ISR for timer.
static void pit_isr(struct regs *s)
{
    static int counter = 0;
    static int hour = 23, minute = 59, second = 59;
    if (counter == 0)
    {
        counter = PIT_FREQUENCY;

        // Tick that shit.
        if (++second == 60)
        {
            second = 0;
            if (++minute == 60)
            {
                minute = 0;
                if (++hour == 24)
                {
                    hour = 0;
                }
            }
        }
        
        kprintf("Uptime: %02d:%02d:%02d\n", hour, minute, second);
    }
    counter--;
}
// Routine for activating the PIT machinery.
void system_init_pit()
{
    pit_set_freq(PIT_FREQUENCY);
    irq_add(pit_isr, 0);
}

// Rouine for setting the PIT frequency.
void pit_set_freq(int hz)
{
    const int pit_scale = 1193180;
    int divisor = pit_scale / hz;
    outb(PIT_CONTROL, 0x34);
    outb(PIT_CHANNEL_0,  divisor        & 0xff);
    outb(PIT_CHANNEL_0, (divisor >> 8)  & 0xff);
}

