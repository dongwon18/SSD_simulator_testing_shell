---
- finished_date:2022-04-01
- tags:
    - shell_programming
    - linux
    - Ubuntu
    - C
    - vi
    - Makefile
    - SSD_simulator
    - SSD_test_program
---
# SSD simulator and testing program
- make SSD simulator 
- manipulate the virtual SSD with shell
- test the simulator with testing scenario using the shell

## Environment
- Ubuntu 20.04 LTS
- GCC
- UTF-8

## SSD simulator
- simulator of SSD
    - LBA: 4 byte
    - total 100 LBA (index range 0 ~ 99)

    - suppored command
        - $./ssd write [index to be written] [value to be written]
    		write the value to given index and store total SSD state to ./nand.txt
        - $./ssd read [index to read]
  			read value of the givein index and store to result in ./result.txt

## testing shell
### suppored command
- $write [index of SSD] [value to be written]
	index range: 0 ~ 99
	value: 32 bit  hex value in upper case letter
	SSD state will be stored in ./ssd/nand.txt
	example: write 3 0xAAAABBBB

- $read [index to be read]
	index range: 0 ~ 99
	result will be stored in ./ssd/result.txt
	example: read 3

- $fullwrite [value to be written]
	write the value to all SSD LBA
	SSD state will be stored in ./ssd/nand.txt
	example: fullwrite 0x12345678

- $fullread
	read value of LBA 0 ~ 99
	result will be stored in ./ssd/result.txt
	result only include the last result which is the value of LBA 99
	example: fullread

- $testapp[number]
	run test scenario [number]
	print result of the test by comparing target answer and real value in LBA

- $clear
	set all LBA value to 0x00000000

- $exit
	terminate the shell


## How to run
1. make file
    ```
    cd ./ssd
    make clean
    make dep
    make
    ```
    ```
    cd ./shell
    make clean
    make dep
    make
    ```
2. run shell program in ./shell directory
3. if need help, enter help for detailed information of each command
4. enter 'exit' to terminate the program
## file structure
```
|-- shell
    |-- Makefile
    |-- help.txt
    |-- shell.c
|-- ssd
    |-- Makefile
    |-- nand.txt
    |-- result.txt
    |-- ssd.c
```

## 배운 점
- c를 이용한 shell 프로그래밍을 통해 기능별 코드 모듈화가 가능해졌다.
- Makefile을 이용하여 프로그램을 build할 수 있다.
- SSD를 테스팅하는 과정을 이해하였다.
- SSD controller를 제작하여 테스팅을 진행하였다.
## 한계점
- LBA를 4 byte로 설정했고 100개의 LBA만 존재하기 때문에 파일을 모두 읽은 후 값을 변경하여 다시 파일 내용을 갱신하는 방식을 사용할 수 있었다. 만약 LBA의 크기가 크거나 개수가 많아진다면 해당 방식은 매우 비효율적이다.
    이러한 경우 파일 커서를 조정하여 특정 부분만 읽고 쓰는 방식을 사용하는 것이 바람직할 것이다.
