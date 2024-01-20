// step #1 shake off the dust

#include <stdio.h>

int main() {
    // define varaibles
    int x = 12;
    float y = 3.14;
    char* s = "Hello World";

    // print variable values
    printf("x is %d, y is %.6f\n", x, y);
    printf("%s\n", s);

    // print range 1 to 5 times 5
    for (int i = 0; i < 5; ++i) {
        printf("%d x 5 = %d\n", i, i * 5);
    }

    return 0;
}