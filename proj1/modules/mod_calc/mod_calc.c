#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

extern void* sys_call_table[];

int (*origin_calc)(const char *path);

long hacked_calc(int param1, int param2, char operation, int* result) {
    *result = param1 % param2;
    return 0;
}

static int __init only_modulo_init(void) {
    printk(KERN_INFO "Start modifying sys_calc...\n");
    origin_calc = sys_call_table[397];
    sys_call_table[397]=hacked_calc;
    return 0;
}

static void __exit only_modulo_exit(void) {
    sys_call_table[397] = origin_calc;
    printk(KERN_INFO "End modifying sys_calc\n");
}

module_init(only_modulo_init);
module_exit(only_modulo_exit);