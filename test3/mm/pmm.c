/*************************************************************************
	> File Name: pmm.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月14日 星期四 14时52分48秒
 ************************************************************************/

#include "multiboot.h"
#include "debug.h"
#include "pmm.h"

void show_memory_map(){
        
    uint32 mmap_addr = glb_mboot_ptr->mmap_addr;
    uint32 mmap_length = glb_mboot_ptr->mmap_length;

    printk("Memory map: \n");

    mmap_entry_t *mmap = (mmap_entry_t *)mmap_addr;
    
    for(mmap = (mmap_entry_t *)mmap_addr;(uint32)mmap < mmap_addr + mmap_length;mmap++){
        printk("base_addr = 0x%x %x,length = 0x%x %x,type = 0x%x\n"
               ,(uint32)mmap->base_addr_high,(uint32)mmap->base_addr_low
               ,(uint32)mmap->length_high,(uint32)mmap->length_low
               ,(uint32)mmap->type);
    }


}

void show_kernel(){

    printk("kernel in memory start = %x\n",kernel_start);
    printk("kernel in memory end   = %x\n",kernel_end);
    printk("kernel in memory used  = %d\n Kb",(kernel_end- kernel_start + 1023)/1024);
    
}

static uint32 pmm_stack[PAGE_MAX_SIZE+1];

static uint32 pmm_stack_top = 0;

uint32 phy_page_count;

uint32 pmm_alloc_page(){
    
    uint32 page = pmm_stack[pmm_stack_top--];

    return page;

}

void pmm_free_page(uint32 p){
    
    pmm_stack[++pmm_stack_top] = p;

}

void init_pmm(){
    
    mmap_entry_t * mmap_start_addr = (mmap_entry_t *)glb_mboot_ptr->mmap_addr;
    mmap_entry_t * mmap_end_addr   = (mmap_entry_t *)glb_mboot_ptr->mmap_addr + glb_mboot_ptr->mmap_length ;

    mmap_entry_t * map_entry;

    for(map_entry = mmap_start_addr ; map_entry < mmap_end_addr;map_entry++){
        
        if(map_entry->type == 1 && map_entry->base_addr_low == 0x100000){

            uint32 page_addr = map_entry->base_addr_low + (uint32)(kernel_end - kernel_start ) + 4096;
            
            uint32 length    = map_entry->base_addr_low + map_entry->length_low ;

            while(page_addr < length && page_addr <= PAM_MAX_SIZE){
                pmm_free_page(page_addr);
                page_addr += PAM_PAGE_SIZE;
                phy_page_count++;
            }
            
        }

        
    }

    printk("the phy_page_count = %d\n",phy_page_count);
    printk("the stack_top = %d\n",pmm_stack_top);

}

