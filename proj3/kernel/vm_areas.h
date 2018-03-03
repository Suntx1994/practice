#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <linux/mm.h>

asmlinkage int show_vm_areas(int pid);