#include "segment_info.h"

asmlinkage int show_segment_info(int pid) {
    struct task_struct *task = find_task_by_vpid(pid);
    if (task) {
        struct mm_struct *this_mm = task->mm;
        if (this_mm) {
            printk(KERN_INFO "[Memory segment addresses of process %d]", pid);
            printk(KERN_INFO "%08lx - %08lx: code segment", this_mm->start_code, this_mm->end_code);
            printk(KERN_INFO "%08lx - %08lx: data segment", this_mm->start_data, this_mm->end_data);
            printk(KERN_INFO "%08lx - %08lx: heap segment", this_mm->start_brk, this_mm->brk);
            return 0;
        }
    }
    return -1;
}
