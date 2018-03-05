#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/slab.h>

asmlinkage int show_vm_areas(int pid);
unsigned long vpage_to_ppage(struct mm_struct *mm, unsigned long virt);
