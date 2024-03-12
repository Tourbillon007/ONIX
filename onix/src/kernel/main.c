#include <onix/onix.h>
#include <onix/types.h>
#include <onix/io.h>
#include <onix/string.h>
#include <onix/console.h>

char message1[] = "hello onix\n";
char message2[] = "hello \n";
char buf[1024];


void kernel_init()
{
 
    console_init();

    u32 count = 32;
    while (count--)
    {
        console_write(message1,sizeof(message1) - 1);
        console_write(message2,sizeof(message2) - 1);
    }
    

    

    return 0;
}
