#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>

asmlinkage long sys_calc(int param1, int param2, char operation, int* result);
