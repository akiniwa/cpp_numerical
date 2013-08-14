#include <stdio.h>

int main(int arg, char* args[]) {
    printf("%d %s %s\n", arg, args[0], args[1]);
    if (args[1]) {
        printf("hoge\n");
    } else {
        printf("none\n");
    }
}
