#include <onix/onix.h>
#include <onix/types.h>
#include <onix/io.h>
#include <onix/string.h>
#include <onix/console.h>
#include <onix/stdarg.h>
#include <onix/printk.h>
#include <onix/assert.h>




void kernel_init()
{
 
    console_init();
   
    assert(3 < 5);
    assert(3 > 5);
   



    return;
}
