#include "segment_info.h"

asmlinkage int show_segment_info(int pid) {
    struct task_struct *task = find_task_by_vpid(pid);
    if (task) {
        struct mm_struct *this_mm = task->mm;
        if (this_mm) {
            printk(KERN_INFO "[Memory segment addresses of process %d]:\n%08lx - %08lx: code segment\n%08lx - %08lx: data segment\n%08lx - %08lx: heap segment\n",
                   pid, this_mm->start_code, this_mm->end_code, this_mm->start_data, this_mm->end_data, this_mm->start_brk, this_mm->brk);
            return 0;
        }
    }
    return -1;
}
