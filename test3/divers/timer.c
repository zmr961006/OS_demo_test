/*************************************************************************
	> File Name: divers/timer.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月13日 星期三 21时20分13秒
 ************************************************************************/

#include"types.h"
#include"debug.h"
#include"timer.h"
#include"screen_port.h"
#include"idt.h"



void timer_callback(pt_regs_t *regs){

    static uint32 tick = 0;
    printk("tick:%d\n",tick++);

}


void init_timer(uint32 frequency){
    
    registers_interrupt_handler(IRQ0,timer_callback);
    uint32 diversor = 1193180 / frequency ;
    outb(0x43,0x36);
    uint8 low = (uint8)(diversor & 0xFF);
    uint8 high = (uint8)((diversor >> 8) & 0xFF);
    outb(0x40,low);
    outb(0x40,high);
    



}
