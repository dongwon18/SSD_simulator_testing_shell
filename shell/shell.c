/*
 * Copyright 2022. Dongwon Kim All rights reserved
 *
 * File name: shell.c
 *
 * Written by Dongwon Kim
 *
 * shell
 * 	shell for SSD simulation
 * 	read, write to SSD simulator
 *	can run test scenario and check if the result is proper or not
 *
 * 	supported command
 * 		write, read, fullwrite, fullread, clear, testapp1, testapp2, exit
 * 		detailed information is in ./ssd/help.txt
 *
 * Modification History
 * 	written by Dongwon Kim on April 01, 2022
 *
 * Environment
 * 	Ubuntu 20.04 LTS
 * 	GCC
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LBA_LIMIT 99
#define INPUT_MAX 100
#define TRUE 1
#define FALSE 0

void help();
int check_index(char *index);
int check_value(char *value);
int runcmd(char *user_input);
int testapp1();
int testapp2();

/*
 * get user input
 * -> run command
 * if returned value is TRUE, terminate the program
 *
 * input: user input from cmd
 * output: return integer 0
 */
int main(){
	char user_input[INPUT_MAX];
	char input_backup[INPUT_MAX];
	char *cmd;

	while(1){
		printf("SSD SHELL >> ");
		gets(user_input);
		int flag = runcmd(user_input);
		if(flag)break;
	}
	return 0;
}

/*
 * print help.txt
 */
void help(){
	system("cat ./help.txt");
}

/*
 * varify index
 *	if index out of range, return FALSE
 *	range:int 0 ~ 99
 *
 * input: 
 * 	char * index: string that contain index to be written or read
 * output:
 * 	return int 1: if index is in proper range
 * 	return int 0: if index out of range
 */
int check_index(char *index){
	int indexi = atoi(index);
	if(indexi < 0 || indexi > LBA_LIMIT){
		printf("[ERROR] invalid index range(0~99)\n");
		return FALSE;
	}
	else return TRUE;
}

/*
 * varify value
 *	if value format is improper, return FALSE
 *	format: 0x-------- (hex format -:0 ~9, A ~Z)
 *
 * input: 
 * 	char * value: string that contain value to be written
 * output:
 * 	return int 1: if value is in proper format
 * 	return int 0: if value is not proper format
 */
int check_value(char *value){
	int len = strlen(value);
	if(value[0]!='0' || value[1]!='x'){
		printf("[ERROR] invalid value format(0x--------)\n");
		return FALSE;
	}
	for(int i = 2; i < len; i++){
		if((value[i]>='A'&& value[i]<='F')|| (value[i]>='0'&& value[i] <='9'))continue;
		printf("[ERROR] invalid hex format\n");
		return FALSE;
	}
	return TRUE;
}

/*
 * run functions according to user input
 * 	for each cmd, checking if arguement from user is proper or not
 * 	if not proper, give error message
 *
 * input:
 * 	char * user_input: user input in a row
 * 		example: write 10 0x11110000
 * output:
 *	print result in cmdof user input if needed
 *	modify ./ssd/nand.txt, ./ssd/result.txt
 *	return int 1: if user_input == exit
 *	return int 0: if user_input != exit
 */
int runcmd(char *user_input){
	// tokenize to find cmd mode
	char *cmd;
	cmd = strtok(user_input, " ");

	// run cmd
	if(strcmp(cmd, "exit")==0){
		return 1;
	}else if(strcmp(cmd, "help")==0){
		help();
	}else if(strcmp(cmd, "write")==0){
		// tokenize to find index and value to be written
		char *index, *value;
		index = strtok(NULL, " ");
		value = strtok(NULL, " ");

		if(!check_index(index) || !check_value(value)){	
			return 0;
		}

		// make cmd to run ssd program
		char ssd_cmd[INPUT_MAX + 20];
		sprintf(ssd_cmd, "../ssd/ssd W %s %s", index, value);
		system(ssd_cmd);
	}else if(strcmp(cmd, "read")==0){
		// tokenize to find index to be read
		char *index;
		index = strtok(NULL, " ");
		if(!check_index(index)){
			return 0;
		}
		
		// make cmd to run ssd program
		char ssd_cmd[INPUT_MAX + 20];
		sprintf(ssd_cmd, " ../ssd/ssd R %s", index);
		system(ssd_cmd);
	}else if(strcmp(cmd, "fullwrite")==0){
		// tokeize to find value
		char *value;
		value = strtok(NULL, " ");
		if(!check_value(value)){
			return 0;
		}

		// write the value from 0 to 99
		char ssd_cmd[INPUT_MAX + 20];
		for(int i = 0; i < 99; i++){
			sprintf(ssd_cmd, "../ssd/ssd W %d %s", i, value);
			system(ssd_cmd);
		}
	}else if(strcmp(cmd, "fullread")==0){
		// by running R [index] cmd of ssd program,
		// write result.txt
		// read result.txt and print the value in the file.
		char ssd_cmd[INPUT_MAX + 20];
		for(int i = 0; i < 99; i++){
			sprintf(ssd_cmd, "../ssd/ssd R %d", i);
			system(ssd_cmd);
			FILE *fp = fopen("../ssd/result.txt", "r");
			char read_value[20];
			fgets(read_value, 20, fp);
			printf("%s", read_value);
			fclose(fp);
		}
	}else if(strcmp(cmd, "clear")==0){
		// clear all LBA to 0x00000000
		FILE *fp=fopen("../ssd/nand.txt", "w");
		char value[20]="0x00000000\n";
		for(int i = 0; i < 100; i++){
			fputs(value, fp);
		}
	}else if(strcmp(cmd, "testapp1")==0){
		int flag = testapp1();
		if(!flag){
			printf("[TEST RESULT] testapp1 failed\n");
		}else{
			printf("[TEST RESULT] testapp1 succeed\n");
		}
	}else if(strcmp(cmd, "testapp2")==0){
		int flag = testapp2();
		if(!flag){
			printf("[TEST RESULT] testapp2 failed\n");
		}else{
			printf("[TEST RESULT] testapp2 succeed\n");
		}
	}else{
		printf("[ERROR] INVALID COMMAND\n");
	}
	return 0;
}

/*
 * test scenario 1
 * 	write 0x11111111 to all LBA
 * 	read all LBA and check if stored value is 0x11111111
 * 
 * input: none
 * output: 
 * 	write ./ssd/result.txt, ./ssd/nand.txt
 * 	return int 1: no failure
 * 	return int 0: test fail(stored value != written value)
 */
int testapp1(){
	printf("==============================\n");
	printf("[TEST: testapp1]full write 0x11111111\n");

	char user_input[INPUT_MAX]="fullwrite 0x11111111";
	runcmd(user_input);

	char ans[20]="0x11111111\n";
	int flag = 0;

	char ssd_cmd[INPUT_MAX + 20];
	for(int i = 0; i < 99; i++){
		sprintf(ssd_cmd, "../ssd/ssd R %d", i);
		system(ssd_cmd);
		FILE *fp = fopen("../ssd/result.txt", "r");
		char read_value[20];
		fgets(read_value, 20, fp);
		printf("%s", read_value);
		if(strcmp(read_value, ans)!=0){
			return FALSE;
		}
		fclose(fp);
	}
	
	return TRUE;
}

/*
 * test scenario2
 * 	write LBA 0 ~ 5 0xAAAABBBB 30 times
 * 	write LBA 0 ~ 5 0x12345678 1 time
 * 	read LBA 0 ~ 5 and check the stored value
 *
 * input: none
 * output: 
 * 	write ./ssd/nand.txt, ./ssd/result.txt file
 *	return 1: no failure
 *	return 0: failure (stored value in LBA is not matched with answer)
 */
int testapp2(){
	printf("==============================\n");
	printf("[TEST: testapp2]write LBA 0 ~ 5\n");


	char user_input[INPUT_MAX]="clear";
	runcmd(user_input);

	for(int i = 0; i < 30; i++){
		for(int j = 0; j < 6; j++){
			sprintf(user_input, "write %d 0xAAAABBBB", j);
			runcmd(user_input);
		}
	}
	
	for(int i = 0; i < 6; i++){
		sprintf(user_input, "write %d 0x12345678", i);
		runcmd(user_input);
	}

	char ans[20]="0x12345678\n";
	int flag = 0;

	for(int i = 0; i < 6; i++){
		char ssd_cmd[INPUT_MAX + 20];
		sprintf(ssd_cmd, "../ssd/ssd R %d", i);
		system(ssd_cmd);
		FILE *fp = fopen("../ssd/result.txt", "r");
		char read_value[20];
		fgets(read_value, 20, fp);
		if(strcmp(read_value, ans)!=0){
			return FALSE;
		}
		fclose(fp);
	}
	
	return TRUE;
}
