#pragma once


//#define _CRT_SECURE_NO_WARNINGS
#define TRUE 1 
#define FALSE 0

#define MAX_LINES 1000 
#define SIZE 5
#define LINE_SIZE 4
#define REG_SIZE 16
#define IOREG_SIZE 6

int reg[REG_SIZE];
char memout[MAX_LINES][SIZE];
char* inst[LINE_SIZE];
int buttons[5], swts[8];
int pc, num_of_reg, address_of_mem, flag_SWT, ioReg[IOREG_SIZE], inst_count;

/*Utility func*/

int getHex(char* source);
int hex2int(char ch);
int getAddress(int address);
void REG_Init();
void IOREG_Init();
/*Create func*/
//void createTrace(FILE* trace, int pc, char line[SIZE], int reg[REG_SIZE], int* count);

/*Line Func*/
void add(int rd, int rs, int rt);
void sub(int rd, int rs, int rt);
void mul(int rd, int rs, int rt);
void andf(int rd, int rs, int rt);
void orf(int rd, int rs, int rt);
void sll(int rd, int rs, int rt);
void sra(int rd, int rs, int rt);
void limm(int rd, char memin[MAX_LINES][SIZE]);
void branch(int rd, int rs, int rt, char memin[MAX_LINES][SIZE]);
void jal(char memin[MAX_LINES][SIZE]);
void lw(int rd, int rs);
void sw(int rd, int rs, char memin[MAX_LINES][SIZE], int *max_line_counter);
void in(int rd, int rs, char memin[MAX_LINES][SIZE]);
void out(int rd, int rs, char memin[MAX_LINES][SIZE]);
void halt();
void decipher_line(char line[SIZE], char memin[MAX_LINES][SIZE], int *max_line_counter_ptr);

int sim(char memin[MAX_LINES][SIZE]);

char fib[MAX_LINES][SIZE] = { "7D00",
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
"0007" };
char stopper[MAX_LINES][SIZE] = { "" };
