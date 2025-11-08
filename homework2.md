# 스케줄러 과제 2

## 테스트 변경

Xorshift32 + LCG 혼합 연산으로 변경(재귀 호출은 스택 오버플로우 위험 있음)


## 테스트 1: 단순 실행

명령어:

```shell
loop 500000 40 &;loop 500000 30 &;loop 500000 20 &;loop 500000 10 &;loop 500000 1 &;
```

결과:

> 텍스트 버퍼 관련 오류가 남아 있어 해석에 주의가 필요함.

```text
loop counlto: o5p00 00c0o AuNnD tmy:  pi5d:00 l5o o
p 00c0ou ntA: ND5 00my00 0p idA: N7D  
my pid: 9 
loop count: 500000 AND my pid: 11 
loop count: 500000 AND my pid: 13 
$ sum: 0
Nice: 40 => Elapsed time: 1284
sum: 0
Nice: 30 => Elapsed time: 1326
sum: 0
Nice: 20 => Elapsed time: 1733
sum: 0
Nice: 10 => Elapsed time: 2133
sum: 0
Nice: 1 => Elapsed time: 2533
```

nice 값에 따라 Elapsed time이 선형적으로 변하는 것을 관찰할 수 있었다.

## 테스트 2: 실행 중 nice 변경

명령어:

```shell
loop 500000 40 &;loop 500000 30 &;loop 500000 20 &;loop 500000 10 &;loop 500000 1 &;
```

```shell
snice $PID 1
# 명령어 실행 첫 줄에 나온 pid를 사용. 즉, 40에서 1로 떨어트림.
```

결과

```text
loop count: 500000 ANDl oomyp  copunit:d:  51060 l
oo0p0 0c oAuNnDt :m y 5p0i0d0: 0108  A
ND my pid: 20 
loop count: 500000 AND my pid: 22 
loop count: 500000 AND my pid: 24 
sum: 0
Nice: 30 => Elapsed time: 1295
sum: 0
Nice: 20 => Elapsed time: 1445
sum: 0
Nice: 10 => Elapsed time: 1691
sum: 0
Nice: 1 => Elapsed time: 2168
sum: 0
Nice: 1 => Elapsed time: 2520
```

40에서 1로 떨어진 프로세스는 Elapsed time이 원래 1이었던 프로세스와 비슷해졌음을 알 수 있다.
