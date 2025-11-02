#include "kernel/types.h"
#include "user/user.h"

// Xorshift32 + LCG 혼합
inline uint32
xorshift32(uint32 *state)
{
    uint32 x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

// 사용법:
// loop 500000 40 &;loop 500000 30 &;loop 500000 20 &;loop 500000 10 &;loop 500000 1 &;
int
main(int argc, char *argv[])
{
    int i, j;
    int count;
    int nice = 20;
    uint32 checksum = 0u;
    int t0, t1;
    int mypid = getpid();

    if (argc < 2) {
        printf("Usage: loop count [nice, default=20]\n");
        exit(1);
    }

    count = atoi(argv[1]);
    if (argc >= 3 && argv[2]) {
        nice = atoi(argv[2]);
    }

    printf("loop count: %d AND my pid: %d \n", count, mypid);

    t0 = uptime();

    int result = setnice(mypid, nice);

    if (result == 0) {
        // 총 연산량은 count * INNER_ITERS에 비례
        const int INNER_ITERS = 200000;

        // 각 외부 루프마다 다른 시드로 시작
        for (i = 0; i < count; i++) {
            uint32 s = (uint32)(i + 1) * 2654435761u; // 골든 프라임 기반 해싱 시드
            for (j = 0; j < INNER_ITERS; j++) {
                // 혼합: LCG 한 번, Xorshift 한 번
                s = s * 1664525u + 1013904223u;   // LCG
                s ^= xorshift32(&s);              // Xorshift와 XOR 혼합
                checksum ^= (s + (uint32)j);      // 체크섬 누적(최적화 방지용 관측값)
            }
        }

        printf("sum: %u", checksum);

        t1 = uptime();
        printf("\nNice: %d => Elapsed time: %d\n", getnice(mypid), t1 - t0);
    } else {
        printf("치명적인 오류");
    }

    exit(0);
}