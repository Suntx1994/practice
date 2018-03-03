#include "vm_areas.h"
#define BUFFER_SIZE 16

asmlinkage int show_vm_areas(int pid) {
    unsigned long vms, vme, dif;
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
                vms = this_vm->vm_start;
                vme = this_vm->vm_end;
                dif = vme - vms;
                if (this_vm->vm_flags & VM_LOCKED)
                    sprintf(vmf, "[L]");
                else
                    sprintf(vmf, " ");
                printk(KERN_INFO "%08lx-%08lx: %lu bytes %s", vms, vme, dif, vmf);
                this_vm = this_vm->vm_next;
            }
            return 0;
        }
    }
    return -1;
}