//
// Created by sonjaehyuk on 25. 10. 31..
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int fact(int x)
{
    if (x==0) return 1;
    else return x * fact(x-1);
}

int
main(int argc, char *argv[])
{
    int i, j;
    int count = atoi(argv[1]);
    int nice = 20;
    int sum = 0;
    int t0, t1;

    if (argc < 2) {
        printf("Usage: loop count [nice, default=20] \n");
        exit(1);
    }
    if (argv[2]) nice = atoi(argv[2]);

    printf("loop count: %d \n", count);

    t0 = uptime();
    setnice(getpid(), nice);

    for (i=sum=0; i<count; i++) {
        for (j=0; j<5000; j++) sum = fact(j);
    }
    printf("sum: %d", sum);

    t1 = uptime();

    printf("\nNice: %d => Elapsed time: %d\n", getnice(getpid()), t1-t0);

    exit(0);
}
//loop 500 40 &;loop 500 30 &;loop 500 20 &;loop 500 10 &;loop 500 1 &;