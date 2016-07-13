/*************************************************************************
	> File Name: entry.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月11日 星期一 20时48分49秒
 ************************************************************************/

#include"screen_operate.h"
#include"string.h"
#include"debug.h"
#include"panic.h"
#include"elf.h"
#include"gdt.h"
#include"idt.h"
#include"timer.h"

int kern_entry(){

    //init_debug();
    init_gdt();
    init_idt();
    char * string = "hello word\n";
    screen_clear();
    printk("%s\n",string);   
    for(int i = 0;i < 3;i++){
        printk("hello \n");
    }
    __asm__ __volatile__("int $0x04");
    init_timer(30000000000);
    __asm__ __volatile__("sti");
    return 0;

}
