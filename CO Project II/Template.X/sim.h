#pragma once

//#define _CRT_SECURE_NO_WARNINGS
#define TRUE 1 
#define FALSE 0

#define MAX_LINES 4096 
#define SIZE 5
#define LINE_SIZE 4
#define REG_SIZE 16


int reg[REG_SIZE];
char* memout[LINE_SIZE];
char* inst[LINE_SIZE];
int pc , num_of_reg, address_of_mem;
int counter;
char* ioReg[];
int flag_SWT;

/*Utility func*/
int getHex(char* source);
int hex2int(char ch);
int getAddress(int address);
/*Create func*/
void createTrace(FILE* trace, int pc, char line[SIZE], int* reg[REG_SIZE], int* count);
void createLastFiles(FILE* memout, char memory_out[MAX_LINES][SIZE], int max_line_counter, FILE* regout, int* reg[REG_SIZE], FILE* count, int counter);

/*Line Func*/
void add(int rd, int rs, int rt, int* reg[REG_SIZE]);
void sub(int rd, int rs, int rt, int* reg[REG_SIZE]);
void mul(int rd, int rs, int rt, int* reg[REG_SIZE]);
void andf(int rd, int rs, int rt, int* reg[REG_SIZE]);
void orf(int rd, int rs, int rt, int* reg[REG_SIZE]);
void sll(int rd, int rs, int rt, int* reg[REG_SIZE]);
void sra(int rd, int rs, int rt, int* reg[REG_SIZE]);
void limm(int rd, char memory_in[MAX_LINES][LINE_SIZE], int* reg[REG_SIZE], int pc);
void branch(int rd, int rs, int rt, char memory_in[MAX_LINES][LINE_SIZE], int* reg[REG_SIZE], int* pc);
void jal(int* reg[REG_SIZE], int* pc, char memory_in[MAX_LINES][LINE_SIZE]);
void lw(int rd, int rs, int* reg[REG_SIZE], char memory_out[MAX_LINES][SIZE], int pc);
void sw(int rd, int rs, int* reg[REG_SIZE], char memory_in[MAX_LINES][LINE_SIZE], char memory_out[MAX_LINES][SIZE], int pc, int *max_line_counter);
void halt(int* pc);
void decipher_line(char line[SIZE], int* reg[REG_SIZE], char memory_in[MAX_LINES][LINE_SIZE], char memory_out[MAX_LINES][SIZE], int* pc, int *max_line_counter_ptr);




int sim(int argc, const char* argv[]);

char* fib[] = {"7D00",
"0080",
"A300",
"0040",
"9000",
"0009",
"B200",
"0041",
"F000",
"7500",
"0003",
"1DD5",
"B9D0",
"0002",
"BFD0",
"0001",
"B3D0",
"0000",
"7600",
"0001",
"8236",
"0019",
"0230",
"0DD5",
"86F0",
"7500",
"0001",
"1335",
"9000",
"0009",
"0920",
"A3D0",
"0000",
"7500",
"0002",
"1335",
"9000",
"0009",
"0229",
"A3D0",
"0000",
"AFD0",
"0001",
"A9D0",
"0002",
"7500",
"0003",
"0DD5",
"86F0",
"0000",
"0000",
"0000",
"0000",
"0000",
"0000",
"0000",
"0000",
"0000",
"0000",
"0000",
"0000",
"0000",
"0000",
"0000",
"0007"};
char* stopper[] = {};
