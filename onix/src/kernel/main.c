#include <onix/onix.h>
#include <onix/types.h>
#include <onix/io.h>
#include <onix/string.h>
#include <onix/console.h>
#include <onix/stdarg.h>
#include <onix/printk.h>
#include <onix/assert.h>
#include <onix/debug.h>
#include <onix/global.h>
#include <onix/task.h>
#include <onix/interrupt.h>




void kernel_init()
{
    memory_map_init();
    interrupt_init();
    clock_init();
    time_init();
    rtc_init();

    console_init();
   
    gdt_init();

    
   



    return;
}
