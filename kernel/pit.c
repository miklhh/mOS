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
#include <thread.h>

#define PIT_FREQUENCY   1000

// Uptime global variables.
int uptime_hour = 23;
int uptime_minute = 59;
int uptime_second = 59;

// ISR for timer.
static void pit_isr(struct regs *s)
{
    (void) s;

    // Uptime counter.
    static int counter = 0;
    if (counter == 0)
    {
        counter = PIT_FREQUENCY;

        // Tick that shit.
        if (++uptime_second == 60)
        {
            uptime_second = 0;
            if (++uptime_minute == 60)
            {
                uptime_minute = 0;
                if (++uptime_hour == 24)
                {
                    uptime_hour = 0;
                }
            }
        }
        //kprintf("Uptime: %02d:%02d:%02d\n", 
        //        uptime_hour, uptime_minute, uptime_second);
    }
    counter--;

    // Call the schedule ticking function.
    thread_tick();
    
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
    outb(PIT_CHANNEL_0,  divisor       & 0xff);
    outb(PIT_CHANNEL_0, (divisor >> 8) & 0xff);
}

