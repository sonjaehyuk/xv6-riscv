
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
