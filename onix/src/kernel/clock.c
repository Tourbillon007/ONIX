#include <onix/io.h>
#include <onix/interrupt.h>
#include <onix/assert.h>
#include <onix/debug.h>
#include <onix/task.h>
#include <onix/timer.h>

#define PIT_CHAN0_REG 0X40
#define PIT_CHAN2_REG 0X42
#define PIT_CTRL_REG 0X43

#define HZ 100
#define OSCILLATOR 1193182 //振荡器的频率
#define CLOCK_COUNTER (OSCILLATOR / HZ) //定义了计时器的计数器值，用于实现特定频率的时钟中断。
#define JIFFY (1000 / HZ)  //Tick。定义了一个时间单位，表示时钟中断的时间间隔。

#define SPEAKER_REG 0x61 //键盘控制器中的端口号(寄存器)
#define BEEP_HZ 440
#define BEEP_COUNTER (OSCILLATOR / BEEP_HZ)
#define BEEP_MS 100

u32 volatile jiffies = 0; //表示系统开机以来的节拍数（tick），即经历的节拍数量。即时钟周期
u32 jiffy = JIFFY;

bool volatile beeping = 0;

void start_beep()
{
    if (!beeping)
    {
        outb(SPEAKER_REG, inb(SPEAKER_REG) | 0b11);
        beeping = true;

        task_sleep(BEEP_MS);

        outb(SPEAKER_REG, inb(SPEAKER_REG) & 0xfc);
        beeping = false;
    }
}

void clock_handler(int vector)
{
    assert(vector == 0x20);
    send_eoi(vector); // 发送中断处理结束

    jiffies++;
    // DEBUGK("clock jiffies %d ...\n", jiffies);

    timer_wakeup();

    task_t *task = running_task();
    assert(task->magic == ONIX_MAGIC);

    task->jiffies = jiffies;
    task->ticks--;
    if (!task->ticks)
    {
        schedule();
    }
}

extern u32 startup_time;

time_t sys_time()
{
    return startup_time + (jiffies * JIFFY) / 1000;
}

//计数器初始化
void pit_init()
{
    // 配置计数器 0 时钟
    outb(PIT_CTRL_REG, 0b00110100);
    outb(PIT_CHAN0_REG, CLOCK_COUNTER & 0xff);
    outb(PIT_CHAN0_REG, (CLOCK_COUNTER >> 8) & 0xff);

    // 配置计数器 2 蜂鸣器
    outb(PIT_CTRL_REG, 0b10110110);
    outb(PIT_CHAN2_REG, (u8)BEEP_COUNTER);
    outb(PIT_CHAN2_REG, (u8)(BEEP_COUNTER >> 8));
}

//时钟初始化
void clock_init()
{
    pit_init();
    set_interrupt_handler(IRQ_CLOCK, clock_handler); //将时钟处理函数赋值到handler_table
    set_interrupt_mask(IRQ_CLOCK, true); // 打开中断
}