#include <stdio.h>

void print_string(char* s) {
    while(*s) { putchar(*s++); }
}

void print_every_third_character(char* string) {
    int i = 0;
    while(string[i] != '\0') {
        if (i % 3 == 0) {
            putchar(string[i]);
        }
        i++;
    }

}

int main(void) {
    print_string("Hello, World!\n");
    print_every_third_character("Hello, World!\n");
}    