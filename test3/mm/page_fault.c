/*************************************************************************
	> File Name: page_fault.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月14日 星期四 19时24分03秒
 ************************************************************************/

#include"vmm.h"
#include"debug.h"


void page_fault(pt_regs_t *regs){

    uint32 cr2;
    __asm__ __volatile__ ("mov %%cr2,%0":"=r"(cr2));

    printk("page fault is %x ,vMM is %x\n",regs->eip,cr2);
    printk("Error code :%x\n",regs->err_code);

    if(!(regs->err_code & 0x1)){
        printk("Becase the page wasnot present\n");
    }

    if(regs->err_code & 0x2){
        printk("Write error\n");
    }else{
        printk("Read erroe\n");
    }

    if(regs->err_code & 0x04){
        printk("In use mode\n");
    }else{
        printk("in kernel mode\n");
    }

    if(regs->err_code & 0x08){
        printk("Reserved bits Being overwriten \n");
    }

    if(regs->err_code & 0x10){
        printk("the fault occurred deuring an instrction fetch\n");
    }

    while(1);
}
