#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "sim.h"
void MEMOUT_Init(char memin[MAX_LINES][SIZE], int max_line_counter)
{
	//init memout to be same as memin
	int i = 0;
	for (; i < MAX_LINES; i++)
		strcpy(memout[i], "0000");
	i = 0;
	for (; i < max_line_counter; i++)
		strcpy(memout[i], memin[i]);
}
/*int main()
{
	sim(fib);
	return 0;
}*/
int sim(char memin[MAX_LINES][SIZE])
{	

	//int max_line_counter = sizeof(memin) / sizeof(memin[0]);
	int max_line_counter = 65;
	int *max_line_counter_ptr = &max_line_counter;
	REG_Init();
	MEMOUT_Init(memin, max_line_counter);

	while (1)
	{

		while (pc > -1)
		{
			char line[SIZE];
			snprintf(line, SIZE, "%s%c", memin[pc], '\0');
			//createTrace(trace, pc, line, reg, counter);
			decipher_line(line, memin, max_line_counter_ptr);
		}
	}
	/*
	for (int i = 0; i < max_line_counter; i++)
	{
		printf("%s\n", memout[i]);
	}*/
	
}

/*Utility Func*/

void REG_Init()
{
	// Init of reg	
	int i = 0;
	for (; i < REG_SIZE; i++)
	{
		reg[i] = 0;
	}
}
void IOREG_Init()
{
	int i = 0;
	for (; i < IOREG_SIZE; i++)
	{
		ioReg[i] = 0;
	}
}
int getHex(char* source)
{
	int n = (int)strtol(source, NULL, 16);
	if (n > 0x7fff)
		n -= 0x10000;
	return n;
}
int hex2int(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;
	printf("Char given, %c, is invalid - non hex character.\n", ch);
	return -1;
}
int getAddress(int address)
{
	if (address < 0)
	{

		return -10;
	}

	if (address >= 4096)
	{
		address = address & 0x0FFF; //if given address is too high, take only 12 LSBs.		
	}

	return address;
}

/*Create Func*/

void createTrace(FILE* trace, int pc, char line[SIZE], int* count)
{
	fprintf(trace, "%04X %s ", pc, line);
	int i = 0;
	for (; i < 16; i++)
	{
		int instruction = reg[i];
		if (instruction < 0)
			instruction -= 0xFFFF0000;
		fprintf(trace, "%04X ", instruction);
	}
	fprintf(trace, "\n");
	(*count)++;
}

/*LINE Func*/
void decipher_line(char line[SIZE], char memin[MAX_LINES][SIZE], int* max_line_counter_ptr) {

	int opcode = hex2int(line[0]);
	int rd = hex2int(line[1]);
	int rs = hex2int(line[2]);
	int rt = hex2int(line[3]);
	if (opcode == 15)
	{
		halt();
		return;
	}
	if (rd == 0 && opcode != 8 && opcode != 11 && opcode != 9)
	{
		if (opcode >= 7)
			pc += 1;
		pc += 1;
		return;
	}
	switch (opcode)
	{
	case 0:	add(rd, rs, rt); break;	//The opcode is add
	case 1: sub(rd, rs, rt); break;    //The opcode is sub
	case 2: mul(rd, rs, rt); break;    //The opcode is mul
	case 3: andf(rd, rs, rt); break;    //The opcode is and
	case 4: orf(rd, rs, rt); break;    //The opcode is or
	case 5: sll(rd, rs, rt); break;    //The opcode is sll
	case 6: sra(rd, rs, rt); break;    //The opcode is sra

	case 7: limm(rd, memin); pc += 1; break;    //The opcode is limm
	case 8: branch(rd, rs, rt, memin); pc -= 1; break;    //The opcode is branch
	case 9: jal(memin); pc -= 1; break;    //The opcode is jal
	case 10: lw(rd, rs); pc += 1; break;    //The opcode is lw
	case 11: sw(rd, rs, memin, max_line_counter_ptr); pc += 1; break;    //The opcode is sw

	//case 12: in(rd, rs, memin); break;
	//case 13: out(rd, rs, memin); break;

	case 15: halt();    //The opcode is halt		
	}
	pc += 1;
}
void add(int rd, int rs, int rt)
{
	reg[rd] = reg[rs] + reg[rt];
}
void sub(int rd, int rs, int rt)
{
	reg[rd] = reg[rs] - reg[rt];
}
void mul(int rd, int rs, int rt)
{
	reg[rd] = reg[rs] * reg[rt];
}
void andf(int rd, int rs, int rt)
{
	reg[rd] = reg[rs] & reg[rt];
}
void orf(int rd, int rs, int rt)
{
	reg[rd] = reg[rs] | reg[rt];
}
void sll(int rd, int rs, int rt)
{
	reg[rd] = reg[rs] << reg[rt];
}
void sra(int rd, int rs, int rt)
{
	if (reg[rs] >= (int)pow(2, 15))
	{
		reg[rd] = reg[rs] >> reg[rt];
		int p = 15;
		int i = 0;
		for (; i < reg[rt]; i++, p--)
			reg[rd] += (int)pow(2, p);

	}
	else
		reg[rd] = reg[rs] >> reg[rt];
}
void limm(int rd, char memin[MAX_LINES][SIZE])
{
	char line[SIZE];
	snprintf(line, SIZE, "%s%c", memin[pc + 1], '\0');
	reg[rd] = getHex(line);
}
void branch(int rd, int rs, int rt, char memin[MAX_LINES][SIZE])
{
	char line[SIZE];
	snprintf(line, SIZE, "%s%c", memin[pc + 1], '\0');
	int imm = getHex(line);
	switch (rd)
	{
	case 0:
		if (reg[rs] == reg[rt])  //beq
			pc = getAddress(imm);
		else pc += 2;
		break;
	case 1:
		if (reg[rs] != reg[rt])  //bne
			pc = getAddress(imm);
		else pc += 2;
		break;
	case 2:
		if (reg[rs] > reg[rt]) //branch if greater than - bgt
			pc = getAddress(imm);
		else pc += 2;
		break;
	case 3:
		if (reg[rs] < reg[rt]) // branch if smaller than - bst
			pc = getAddress(imm);
		else pc += 2;
		break;
	case 4:
		if (reg[rs] >= reg[rt]) // beq or bgt
			pc = getAddress(imm);
		else pc += 2;
		break;
	case 5:
		if (reg[rs] <= reg[rt]) // beq or bst
			pc = getAddress(imm);
		else pc += 2;
		break;
	case 6: pc = getAddress(reg[rs]);
	}
}
void jal(char memin[MAX_LINES][SIZE])
{
	reg[15] = pc + 2;
	char line[SIZE];
	snprintf(line, SIZE, "%s%c", memin[pc + 1], '\0');
	int imm = getHex(line);
	pc = getAddress(imm);
}
void lw(int rd, int rs)
{
	char line[SIZE], line1[SIZE];
	snprintf(line, SIZE, "%s%c", memout[pc + 1], '\0');
	int imm = getHex(line);
	int address = getAddress(imm + reg[rs]);
	snprintf(line1, SIZE, "%s%c", memout[address], '\0');
	reg[rd] = getHex(line1);
}
void sw(int rd, int rs, char memin[MAX_LINES][SIZE], int *max_line_counter_ptr)
{
	char line[SIZE];
	snprintf(line, SIZE, "%s%c", memin[pc + 1], '\0');
	int imm = getHex(line);
	int address = getAddress(imm + reg[rs]);
	snprintf(memout[address], SIZE, "%04X%c", reg[rd], '\n');
	if (*max_line_counter_ptr < address)//&& reg[rd] != 0
		*max_line_counter_ptr = address;
}
void halt()
{
	pc = -10;//DESTROY!!
}
