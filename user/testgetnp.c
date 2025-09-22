//
// Created by sonjaehyuk on 25. 9. 22..
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    printf("hello world\n");
    printf("Current NP: %d\n", getnp());
    exit(0);
}