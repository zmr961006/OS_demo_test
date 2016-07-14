/*************************************************************************
	> File Name: vmm.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月14日 星期四 17时29分12秒
 ************************************************************************/

#ifndef _VMM_H
#define _VMM_H 

#include "types.h"
#include "idt.h"


#define PAGE_OFFSET  0xc0000000

#define PAGE_PRESET  0x1 

#define PAGE_WRITE   0x2

#define PAGE_USER    0x4 

#define PAGE_SIZE    4096

#define PAGE_MASK    0xFFFFF000

#define PGD_INDEX(x)   (((x) >> 22) & 0x3FF)

#define PTE_INDEX(x)   (((x) >> 12) & 0x3FF)

#define OFFSET_INDEX(x)  ((x) && 0xFFF)

typedef uint32 pgd_t;
typedef uint32 pte_t;

#define PGD_SIZE   (PAGE_SIZE / sizeof(pte_t))

#define PTE_SIZE   (PAGE_SIZE / sizeof(uint32)) 

#define PTE_COUNT  128 

extern pgd_t pgd_kernel[PGD_SIZE];

void init_vmm();

void switch_pgd(uint32 pd);

void map(pgd_t *pgd_now,uint32 va,uint32 pa,uint32 flags);

void umap(pgd_t *pgd_now,uint32 va);

uint32 get_mapping(pgd_t *pgd_now,uint32 va,uint32 *pa);

void page_fault(pt_regs_t *regs);


#endif
