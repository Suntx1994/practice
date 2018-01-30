#include "sys_calc.h"

asmlinkage long sys_calc(int param1, int param2, char operation, int* result) {
    switch (operation) {
        case '+':
            *result = param1 + param2;
            break;
        case '-':
            *result = param1 - param2;
            break;
        case '*':
            *result = param1 * param2;
            break;
        case '/':
            if (param2 == 0) {
                return -1;
            }
            *result = param1 / param2;
            break;
        default:
            return -1;
    }
    printk(KERN_INFO "Result = %d\n", *result);
    return 0;
}

