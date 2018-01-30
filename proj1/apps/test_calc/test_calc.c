#include <stdio.h>
#include <stdlib.h>

#define sys_calc 397

int main(int argc, char* argv []) {
    if (argc != 4) {
        printf("NaN\n");
        return 0;
    }
    
    int param1 = atoi(argv[1]);
    int param2 = atoi(argv[3]);
    char op = argv[2][0];
    int result = -1;
    long error = syscall(sys_calc, param1, param2, op, &result);
    
    if (error == 0) {
        printf("%d\n", result);
    }
    else {
        printf("NaN\n");
    }
    
    return 0;
}
