#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>

asmlinkage long sys_calc(void) {
    printk(KERN_INFO "Hello world! Test system callllll\n");
    return 0;
}