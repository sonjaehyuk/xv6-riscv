//
// Created by sonjaehyuk on 25. 9. 24..
//
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if(argc < 3){
        printf("Usage: snice pid value\n");
        exit(1);
    }

    int pid = atoi(argv[1]);
    int value = atoi(argv[2]);
    printf("%d %s\n", value, argv[2]);
    // syscall 호출 전에 먼저 검사
    if (value < 0 || value > 40 || argv[2][0] == '-') {
        printf("value는 0부터 40까지만 유효합니다.\n");
        exit(0);
    }

    int result = setnice(pid, value);
    if (result == 0) {
        printf("PID가 %d인 프로세스의 nice를 %d로 설정하였습니다.\n", pid, value);
    } else if (result == -1) {
        printf("존재하지 않는 pid입니다.\n");
    } else if (result == -2) {
        printf("pid 설정에 실패하였습니다.\n");
    } else {
        printf("알 수 없는 치명적인 오류");
    }
    exit(0);
}