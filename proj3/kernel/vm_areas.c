#include "vm_areas.h"
#define BUFFER_SIZE 16

unsigned long vpage_to_ppage(struct mm_struct *mm, unsigned long address) {
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    struct page *page;
    unsigned long phys;
    pgd_t *pgd = pgd_offset(mm, address);
    if (pgd_none(*pgd) || unlikely(pgd_bad(*pgd)))
        return -1;
    pud = pud_offset(pgd, address);
    if (pud_none(*pud) || unlikely(pud_bad(*pud)))
        return -1;
    pmd = pmd_offset(pud, address);
    if (pmd_none(*pmd) || unlikely(pmd_bad(*pmd)))
        return -1;
    if (!(pte = pte_offset_map(pmd, address)))
        return -1;
    if (!(page = pte_page(*pte)))
        return -1;
    phys = page_to_phys(page);
    pte_unmap(pte);
    return phys;
}

asmlinkage int show_vm_areas(int pid) {
    unsigned long vms, vme, dif, vpage, phy_addr;
    int num_phy_pages;
    char vmf[BUFFER_SIZE];
    struct task_struct *this_task;
    struct mm_struct *this_mm;
    struct vm_area_struct *this_vm;
    this_task = find_task_by_vpid(pid);
    if (this_task) {
        this_mm = get_task_mm(this_task);
        if (this_mm) {
            printk(KERN_INFO "[Memory-mapped areas of process %d]", pid);
            this_vm = this_mm->mmap;
            while (this_vm) {
                // int ret = -EINVAL;
                vms = this_vm->vm_start;
                vme = this_vm->vm_end;
                dif = vme - vms;
                if (this_vm->vm_flags & VM_LOCKED)
                    sprintf(vmf, " [L],");
                else
                    sprintf(vmf, ",");
                num_phy_pages = 0;
                for (vpage = vms; vpage < vme; vpage += PAGE_SIZE) {
                    phy_addr = vpage_to_ppage(this_vm->vm_mm, vpage);
                    // ret = follow_pfn(this_vm, vpage, &phy_addr);
                    // if (ret == 0) {
                    if (phy_addr != -1) {
                        num_phy_pages++;
                    }
                }
                printk(KERN_INFO "%08lx-%08lx: %lu bytes%s %d pages in physical memory", vms, vme, dif, vmf, num_phy_pages);
                this_vm = this_vm->vm_next;
            }
            return 0;
        }
    }
    return -1;
}