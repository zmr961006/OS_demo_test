/*************************************************************************
	> File Name: mm/vmm.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月14日 星期四 18时44分52秒
 ************************************************************************/

#include "vmm.h"
#include "pmm.h"
#include "debug.h"
#include "string.h"
#include "idt.h"


pgd_t  pgd_kernel[PGD_SIZE]  __attribute__((aligned(PAGE_SIZE))) ;

static pte_t pte_kernel[PTE_COUNT][PTE_SIZE] __attribute__((aligned(PAGE_SIZE))) ;


void init_vmm(){
    
    uint32 kernel_pte_first_idx = PGD_INDEX(PAGE_SIZE);

    uint32 i;
    uint32 j;

    for(i = kernel_pte_first_idx,j = 0;i < PTE_COUNT + kernel_pte_first_idx;i++,j++){
        pgd_kernel[i] = ((uint32)pte_kernel[j] - PAGE_SIZE) | PAGE_PRESET | PAGE_WRITE;
    }

    uint32 * pte = (uint32 *)pte_kernel;
    
    for(i  = 1;i < PTE_COUNT * PTE_SIZE;i++){
        pte[i] = (i << 12) | PAGE_PRESET | PAGE_WRITE;
    } 

    uint32 pgd_kernel_phy_addr = (uint32) pgd_kernel - PAGE_OFFSET;

    register_interrupt_handler(14,&page_fault);
    
    switch_pgd(pgd_kernel_phy_addr);


}

void switch_pgd(uint32 pd){

    __asm__ __volatile__ ("mov %0,%%cr3"::"r"(pd));

}

void map(pgd_t *pgd_now,uint32 va,uint32 pa,uint32 flags){
    
    uint32 pgd_idx = PGD_INDEX(va);
    uint32 pte_idx = PTE_INDEX(va);

    pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);

    if(!pte){
        pte = (pte_t *)pmm_alloc_page();
        pgd_now[pgd_idx] = (uint32)pte | PAGE_PRESET | PAGE_WRITE;
        pte = (pte_t *)((uint32)pte + PAGE_OFFSET);
        bzer(pte,PAGE_SIZE);
    }else{
        
        pte = (pte_t *)((uint32)pte + PAGE_OFFSET);

    }
    pte[pte_idx] = (pa & PAGE_MASK) | flags;


    __asm__ __volatile__ ("invlpg (%0)"::"a"(va));


}

void unmap(pgd_t *pgd_now,uint32 va){
    
    uint32 pgd_idx = PGD_INDEX(va);
    uint32 pte_idx = PTE_INDEX(va);

    pte_t * pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);

    if(!pte){
        return;
    }
    
    pte = (pte_t *)(uint32)pte + PAGE_OFFSET;

    pte[pte_idx] = 0;

    __asm__ __volatile__ ("invlpg (%0)"::"a"(va));

}

uint32 get_mapping(pgd_t *pgd_now,uint32 va,uint32 *pa){
    
    uint32 pgd_idx  = PGD_INDEX(va);
    uint32 pte_idx  = PTE_INDEX(va);

    pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK );

    if(!pte){
        return  0;

    }
    
    pte = (pte_t *)((uint32)pte + PAGE_OFFSET);

    if(pte[pte_idx] != 0 && pa){
        *pa = pte[pte_idx] & PAGE_MASK;
        return 1;
    }

    return 0;

}


