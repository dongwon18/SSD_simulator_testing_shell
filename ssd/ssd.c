/*
 * Copyright 2022. Dongwon Kim All rights reserved.
 *
 * File name: ssd.c
 *
 * Written by Dongwon Kim
 *
 * SSD simulator
 * 	simulator of SSD
 * 	LBA: 4 byte
 * 	total 100 LBA (index range 0 ~ 99)
 * 	
 * 	suppored command
 * 		$./ssd write [index to be written] [value to be written]
 * 			write the value to given index and store total SSD state to ./nand.txt
 * 		$./ssd read [index to read]
 * 			read value of the givein index and store to result in ./result.txt
 * Modification History
 * 	written by Dongwon Kim on Aprin 01, 2022
 *
 * Environment
 * 	Ubuntu 20.04 LTS
 * 	GCC
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fileread();
void filewrite();
void write(int index, char *value);
void read(int index);

// array to store SSD LBA value
char arr[100][20];

/*
 * get arguments from cmd
 * run functions according to mode
 *
 * input: arguments (mode, index, value)
 * output: 
 * 	file write ./nand.txt, ./result.txt
 * 	return int 0
 */
int main(int argc, char *argv[]) {
	char *mode = argv[1];
	char index[3];
	strcpy(index, argv[2]);

	if (strcmp(mode, "W") == 0) {
		char value[100];
		strcpy(value, argv[3]);
		int indexi = atoi(index);
		write(indexi, value);
	}
	else if (strcmp(mode, "R") == 0) {
		int indexi = atoi(index);
		read(indexi);
	}
	else {
		printf("[ERROR] Invalid command\n");
	}

	return 0;
}

/*
 * read ./nand.txt and store values to arr
 *
 * input:
 * 	file input: ./nand.txt - that has previous SSD state
 *
 * output:
 * 	gloal variable char arr[100][20]: store read value to arr
 */
void fileread() {
	FILE *fp = fopen("../ssd/nand.txt", "r");
	if (fp == NULL) {
		printf("No such file\n");
	}

	int i = 0;

	while (1) {
		char buf[20];
		char *log = fgets(buf, 20, fp);
		if (log == NULL) break;

		strcpy(arr[i], buf);
		i++;
	}
	fclose(fp);
}

/*
 * write arr values to ./nand.txt
 *
 * input: 
 * 	global variable char arr[100][20]: has value to be written in file
 *
 * output:
 * 	file output: write arr value to ./nand.txt
 * 		a line for arr[i]
 */
void filewrite() {
	FILE *fp = fopen("../ssd/nand.txt", "w");
	for (int i = 0; i < 100; i++) {
		fputs(arr[i], fp);
	}

	fclose(fp);
}

/*
 * read ./nand.txt -> write given value in the index -> store arr to ./nand.txt
 *
 * input:
 * 	int index: user input, index to be written
 * 	char *value: user input, value to be stored
 * 
 * output:
 * 	file output: write ./nand.txt
 */
void write(int index, char *value) {
	fileread();
	char *value_input[20];
	sprintf(value_input, "%s\n", value);
	strcpy(arr[index], value_input);
	filewrite();
}

/*
 * read ./nand.txt -> read given index -> store read value to ./result.txt
 *
 * input:
 * 	int index: user input, index to be read
 *
 * output:
 * 	file output: write ./result.txt
 */
void read(int index) {
	fileread();

	FILE *fp = fopen("../ssd/result.txt", "w");
	fputs(arr[index], fp);
	fclose(fp);
}
