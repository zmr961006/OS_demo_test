/*************************************************************************
	> File Name: pmm.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月14日 星期四 15时00分10秒
 ************************************************************************/

#ifndef _PMM_H
#define _PMM_H

#include "multiboot.h"
#include "types.h"
#include "debug.h"


#define STACK_SIZE  8192 

#define PAM_MAX_SIZE 0x20000000

#define PAM_PAGE_SIZE  0x1000

#define PAGE_MAX_SIZE  (PAM_MAX_SIZE/PAM_PAGE_SIZE)

#define PHY_PAGE_MASK  0xFFFFFF000

extern uint32 phy_page_count;

void init_pmm();

void show_memory_map();


uint32 pmm_alloc_page();

void pmm_free_page(uint32 p);


extern uint32 kernel_start[];
extern uint32 kernel_end[];

void show_kernel();


#endif
