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
#include"pmm.h"
#include"vmm.h"
#include"multiboot.h"


int kern_entry();

multiboot_t *glb_mboot_ptr;

char kernel_stack[STACK_SIZE];

__attribute__((section(".init.data"))) pgd_t *pgd_tmp = (pgd_t *)0x1000;
__attribute__((section(".init.data"))) pgd_t *pte_low = (pgd_t *)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_high = (pgd_t *)0x3000;


__attribute__((section(".init.text")))void entry(){
    
    pgd_tmp[0] = (uint32)pte_low | PAGE_PRESET | PAGE_WRITE;
    pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (uint32)pte_high | PAGE_PRESET | PAGE_WRITE ;

    int i;
    for(i = 0;i < 1024;i++){
        pte_low[i] = (i << 12) | PAGE_PRESET | PAGE_WRITE ;
    }
    
    for(i = 0;i < 1024;i++){
        pte_high[i] = (i << 12) | PAGE_PRESET | PAGE_WRITE;
    }

    __asm__ __volatile__("mov %0,%%cr3"::"r"(pgd_tmp));

    uint32 cr0;
    __asm__ __volatile__("mov %%cr0,%0":"=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ __volatile__("mov %0,%%cr0"::"r"(cr0));

    uint32 kernel_stack_top = ((uint32)kernel_stack + STACK_SIZE) & 0xFFFFFFF0;

    __asm__ __volatile__("mov %0,%%esp\n\t"
                                "xor %%ebp,%%ebp"::"r"(kernel_stack_top));

    glb_mboot_ptr = glb_mboot_ptr + PAGE_OFFSET;

    kern_entry();

}



int kern_entry(){

    init_gdt();
    init_idt();
    char * string = "hello word\n";     /*基础打印测试*/
    screen_clear();
    printk("%s\n",string);   
    for(int i = 0;i < 3;i++){
        printk("hello \n");
    }
    //__asm__ __volatile__("int $0x04"); 中断测试
    //init_timer(30000000000);
    //__asm__ __volatile__("sti");
      
    //printk("the mmap_lenghth = %d ,the mmap_addr = %d\n",glb_mboot_ptr->mmap_length,glb_mboot_ptr->mmap_addr);
    
    show_memory_map();
    
    show_kernel();

    init_pmm();

    uint32 alloc = NULL;
    alloc = pmm_alloc_page();
    printk("the alloc mm is %x\n",alloc);
    uint32 blloc = NULL;
    blloc = pmm_alloc_page();
    printk("the blloc mm is %x\n",blloc);
    alloc = pmm_alloc_page();
    printk("the alloc mm is %x\n",alloc);
    return 0;

}
