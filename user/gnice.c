//
// Created by sonjaehyuk on 25. 9. 24..
//
#include "kernel/types.h"
#include "user/user.h"

// 사용법: gnice pid
int main(int argc, char *argv[]) {
    if(argc < 2){
        printf("Usage: gnice pid\n");
        exit(1);
    }
    int pid = atoi(argv[1]);
    int nice = getnice(pid);

    if (nice < 0) {
        printf("존재하지 않는 pid입니다.\n");
    } else if (nice > 40) {
        printf("nice 값의 심각한 오류 발생\n");
    } else {
        printf("%d의 nice는 %d입니다.\n", pid, nice);
    }
    exit(0);
}