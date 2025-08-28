
## 마음가짐

좋은 코드를 가늠하는 확실한 방법은 '얼마나 수정하기 쉬운가'다.

1. 처음에는 그냥 한다.
2. 비슷한 일을 두 번째로 하게 되면, 일단 계속 진행한다.
3. 비슷한 일을 세 번째 하게 되면 리팩터링한다.

## 챕터 1

운영체제는 user space와 kernel space로 나뉜다. kernel space는 하드웨어에 대한 특권을 가지며, user space는 특권을 가지지 못한다. user space는 system call로써 kernel space를 이용할 수 있으며, 이때 kernel은 특권을 보호해야 한다. 이로써 user space의 프로세스는 자신의 메모리만 참조할 수 있는 등의 제약이 생긴다. 

xv6에서 제공하는 system calls:

| 함수 프로토타입                              | 설명                                                            |
|---------------------------------------|---------------------------------------------------------------|
| int fork()                            | 프로세스를 생성한다. 자식 PID를 반환한다.                                     |
| int exit(int status)                  | 현재 프로세스를 종료한다. status는 wait()에게 보고된다. 반환이 없다.                 |
| int wait(int* status)                 | 자식 프로세스의 종료를 기다린다. status에 자식이 보고한 status가 담긴다. 자식 PID를 반환한다. |
| int kill(int pid)                     | pid 프로세스를 종료한다. 0을 반환하거나 오류 시 -1을 반환한다.                       |
| int getpid()                          | 현재 프로세스의 pid를 반환한다.                                           |
| int sleep(int n)                      | n 클록동안 멈춘다.                                                   |
| int exec(char* file, char* argv[])    | file을 적재하고 argv를 가지고 실행한다. 오류 시에만 반환한다.                       |
| char* sbrk(int n)                     | 프로세스 메모리를 n byte 확장한다. 새 메모리의 시작을 반환한다.                       |
| int open(char* file, int flags)       | file을 연다. flags는 읽기/쓰기 신호를 정한다. fd를 반환한다.                     |
| int write(int fd, char* buf, int n)   | buf에 있는 n byte를 fd에 쓴다. n을 반환한다.                              |
| int read(int fd, char* buf, int n)    | fd를 n byte 읽고 buf에 쓴다. 읽은 byte 수를 반환한다. 파일 끝이면 0을 반환한다.       |
| int close(int fd)                     | 열린 파일 fd를 방출한다.                                               |
| int dup(int fd)                       | fd와 같은 파일을 가리키는 새 fd를 반환한다.                                   |
| int pipe(int p[])                     | 파이프를 생성한다. p\[0]과 p\[1]에 읽기/쓰기 fd를 넣어야 한다.                    |
| int chdir(char* dir)                  | 현재 디렉토리를 변경한다.                                                |
| int mkdir(char* dir)                  | 새 디렉토리를 생성한다.                                                 |
| int mknod(char* file, int, int)       | 새 device file을 생성한다.                                          |
| int fstat(int fd, struct stat* st)    | 열린 파일 정보를 st에 집어 넣는다.                                         |
| int stat(char* file, struct stat* st) | file 정보를 st에 집어 넣는다.                                          |
| int link(char* file1, char* file2)    | file1을 가리키는 file2를 생성한다.                                      |
| int unlink(char* file)                | 파일을 제거한다.                                                     |

* `user/sh.c`: 셸 관련 코드.
* `kernel/fcntl.h`: open()의 flags 정의
* `kernel/stat.h`: stat 구조체 정의

fork/exec가 분리되어 있어야 I/O 전환이 쉬워진다.

과제
: pipe 쌍을 이용해 서로 바이트를 주고받는 프로그램을 작성하고, 초당 교환 성능을 측정하시오.

## 챕터 2

대부분의 CPU 아키텍처는 기계 명령을 machine mode, supervisor mode, user mode로 나누어서 본다. 커널의 주요 코드는 supervisor mode에서 작동한다. 반면 애플리케이션 프로세스는 user mode에 실행되며, 실행되어야 한다. 마이크로 커널의 경우 커널 역할이 user mode에서 실행될 수도 있다.

| kernel/에 있는 파일 | 설명                                 |
|----------------|------------------------------------|
| bio.c          | 파일 시스템을 위한 디스크 블록 캐시               |
| console.c      | 사용자 키보드와 스크린을 연결한다.                |
| entry.S        | xv6가 처음 시작할 때 사용하는 부팅 명령           |
| exec.c         | exec() 시스템 호출을 구현                  |
| file.c         | FD 관련 구현                           |
| fs.c           | 파일 시스템                             |
| kalloc.c       | 물리 page 할당자                        |
| kernelvec.S    | 커널의 trap을 관리하며, 타이머 인터립트 처리를 담당한다. |
| log.c          | 크래시 복구와 파일 시스템 로그를 관리한다.           |
| main.c         | 부팅될 때 초기화를 제어하는 역할                 |
| pipe.c         | pipe 관련 구현                         |
| plic.c         | RISC-V 인터럽트 관리자                    |
| printf.c       | 콘솔에 출력할 내용을 구성하는 역할                |
| proc.c         | 프로세스 및 스케줄링                        |
| sleeplock.c    | CPU에 양보하는 락                        |
| spinlock.c     | CPU에 양보하지 않는 락                     |
| start.c        | machine mode에서 실행할 부팅 코드           |
| string.c       | C 문자열과 바이트 배열 라이브러리                |
| swtch.S        | 스레드 교체                             |
| syscall.c      | 시스템 호출을 장착하는 역할                    |
| sysfile.c      | 파일 관련 시스템 호출                       |
| sysproc.c      | 프로세스 관련 시스템 호출                     |
| trampoline.S   | 사용자와 커널간 어셈블리 코드 교체                |
| trap.c         | trap과 인터럽트 반환 및 처리를 위한 C 코드        |
| uart.c         | 시리얼 콘솔 포트 드라이버                     |
| virtio_disk.c  | 디스크 드라이버                           |
| vm.c           | 페이지 테이블과 주소 공간을 관리한다.              |

모든 정의는 `kernel/def.h`에서도 찾을 수 있다.

부팅이 시작되면, 읽기 전용 메모리에 있는 부트로더가 실행된다. 부트로더는 xv6 커널을 메모리에 적재한다. 그런다음, `kernel/entry.S`에 있는 _entry를 실행한다. qemu에서 0x80000000까지는 I/O 장치로 예약이 되어 있으므로, 커널은 0x80000000부터 시작한다. _entry는 stack0로 불리는 초기 스택을 선언하고(`kernel/start.c`), 스택 포인터를 stack0 + 4096으로 옮긴다. 이후 `kernel/start.c`의 start 함수를 호출한다. start 함수는 machine mode에서 실행하다가 mret을 호출하고 `kernel/main.c`의 main 함수를 supervisor 모드에서 실행하도록 호출한다. RISC-V 사양에 의해 전환 과정에서 다양한 정보가 레지스터에 기록된다.

main 함수는 첫 프로세스를 만들기 위해 `kernel/proc.c`의 userinit 함수를 호출한다. 이렇게 만들어진 첫 프로세스(`user/init.c`)는 exec()를 호출하여 셸을 생성한다.

과제
: 사용 가능한 메모리 양을 반환하는 시스템 호출을 추가하시오.

## 챕터 3

`kernel/memlayout.h`는 커널의 주소 공간에 대한 정의를 한다.

각 프로세스의 kstack 초과를 감지하기 위해 **guard page**가 존재하며, 프로세스가 이들을 참조하는 경우 커널이 오류를 일으킬 수 있다(guard page 할당 자체가 유효하지 않음).

커널 코드와 trampoline 영역에 대해 페이지 테이블은 **PTE_R**과 **PTE_X** 권한을 설정한다. 이외 페이지에 대해서는 **PTE_R**과 **PTE_W**로 설정한다.

`kernel/vm.c`에서는 가상 메모리 주소 공간을 다룬다.

* `walk`: 가상 주소의 PTE를 찾는다.
* `mappages`: 새로운 매핑을 위한 PTE를 만든다.
* `kvm*`: 커널 페이지 테이블 관련
* `uvm*`: 사용자 페이지 테이블 관련

`kernel/kalloc.c`에서는 물리 메모리 할당에 대해 정의한다.

kmem은 사용할 수 있는 전체 유효 공간을 가지고 있다. kinit() 함수에 의해 아래와 같이 초기화될 수 있다. PHYSTOP부터 PGROUNDUP(end)까지 연결 리스트로 이어진다. PHYSTOP의 주소에서 4KB를 빼야 함을 주의해야 한다(그래야 마지막 페이지의 끝이 PHYSTOP이 되므로).

```text
0x13000 → 0x12000 → 0x11000 → 0x10000 → NULL
```

여기서 kalloc()으로 물리 메모리 공간 하나를 가져가면 아래와 같이 변한다.

```text
0x12000 → 0x11000 → 0x10000 → NULL
```

함수 호출의 결과로 `0x13000`를 받을 수 있다.

kfree(0x13000)으로 kmem에 다시 넣을 수 있다.

```text
0x13000 → 0x12000 → 0x11000 → 0x10000 → NULL
```

exec는 프로세스 변환 과정 중 실패할 것을 대비해 변환 과정이 완료되기 전까지 기존 프로세스 메모리를 보존한다.

과제:

1. 컴퓨터가 가진 물리 메모리 양을 찾을 수 있도록 RISC-V의 디바이스 트리를 분석하시오.
2. sbrk(1)을 호출하여 주소 공간을 한 바이트 확장하는 사용자 프로그램을 작성하시오.
3. xv6 커널이 슈퍼 페이지를 사용하도록 수정하시오.
4. 일반적인 셸 스크립트에서 지원하는 `#!/bin/bash` 기능을 xv6에서 지원하도록 수정하시오.
5. 커널에 대한 주소 공간 배치 무작위화를 구현하시오.

```text
xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix
Version 6 (v6).  xv6 loosely follows the structure and style of v6,
but is implemented for a modern RISC-V multiprocessor using ANSI C.

ACKNOWLEDGMENTS

xv6 is inspired by John Lions's Commentary on UNIX 6th Edition (Peer
to Peer Communications; ISBN: 1-57398-013-7; 1st edition (June 14,
2000)).  See also https://pdos.csail.mit.edu/6.1810/, which provides
pointers to on-line resources for v6.

The following people have made contributions: Russ Cox (context switching,
locking), Cliff Frey (MP), Xiao Yu (MP), Nickolai Zeldovich, and Austin
Clements.

We are also grateful for the bug reports and patches contributed by
Abhinavpatel00, Takahiro Aoyagi, Marcelo Arroyo, Hirbod Behnam, Silas
Boyd-Wickizer, Anton Burtsev, carlclone, Ian Chen, clivezeng, Dan
Cross, Cody Cutler, Mike CAT, Tej Chajed, Asami Doi,Wenyang Duan,
echtwerner, eyalz800, Nelson Elhage, Saar Ettinger, Alice Ferrazzi,
Nathaniel Filardo, flespark, Peter Froehlich, Yakir Goaron, Shivam
Handa, Matt Harvey, Bryan Henry, jaichenhengjie, Jim Huang, Matúš
Jókay, John Jolly, Alexander Kapshuk, Anders Kaseorg, kehao95,
Wolfgang Keller, Jungwoo Kim, Jonathan Kimmitt, Eddie Kohler, Vadim
Kolontsov, Austin Liew, l0stman, Pavan Maddamsetti, Imbar Marinescu,
Yandong Mao, Matan Shabtay, Hitoshi Mitake, Carmi Merimovich,
mes900903, Mark Morrissey, mtasm, Joel Nider, Hayato Ohhashi,
OptimisticSide, papparapa, phosphagos, Harry Porter, Greg Price, Zheng
qhuo, Quancheng, RayAndrew, Jude Rich, segfault, Ayan Shafqat, Eldar
Sehayek, Yongming Shen, Fumiya Shigemitsu, snoire, Taojie, Cam Tenny,
tyfkda, Warren Toomey, Stephen Tu, Alissa Tung, Rafael Ubal, unicornx,
Amane Uehara, Pablo Ventura, Luc Videau, Xi Wang, WaheedHafez, Keiichi
Watanabe, Lucas Wolf, Nicolas Wolovick, wxdao, Grant Wu, x653, Andy
Zhang, Jindong Zhang, Icenowy Zheng, ZhUyU1997, and Zou Chang Wei.

ERROR REPORTS

Please send errors and suggestions to Frans Kaashoek and Robert Morris
(kaashoek,rtm@mit.edu).  The main purpose of xv6 is as a teaching
operating system for MIT's 6.1810, so we are more interested in
simplifications and clarifications than new features.

BUILDING AND RUNNING XV6

You will need a RISC-V "newlib" tool chain from
https://github.com/riscv/riscv-gnu-toolchain, and qemu compiled for
riscv64-softmmu.  Once they are installed, and in your shell
search path, you can run "make qemu".

```
