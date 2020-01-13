
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sim.h"
int sim(char* memin[LINE_SIZE])
{		
    int max_line_counter = sizeof(memin)/sizeof(memin[0]);
	int *max_line_counter_ptr = &max_line_counter;
    // Init of reg	
	int i = 0;
	for (; i < REG_SIZE; i++)
	{
		reg[i] = 0;
	}
    
	
	 

	while (memin)//memory_in and memory_out are filled
	{
		int check;
		if (check = fscanf(memin, "%s", memory_in[max_line_counter]) != 0) { //Consider \n in fscanf and avoid using memout because probably there are changes to the code line pheraps there should be.
			snprintf(memory_out[max_line_counter], SIZE, "%s%c", memory_in[max_line_counter], '\n');
			max_line_counter++;
		}
	}
	fclose(memin);

	while (*pc > -1)
	{
		char line[SIZE];
		snprintf(line, SIZE, "%s%c", memory_in[*pc], '\0');
		printf("%s", line);
		//createTrace(trace, *pc, line, reg, counter);
		printf(" %d\n", *counter);
		decipher_line(line, reg, memory_in, memory_out, pcp, max_line_counter_ptr);
		
	}
	//Exit:	
	fclose(trace);
	createLastFiles(memout, memory_out, max_line_counter, regout, reg, count, *counter);
	fclose(memout);
	fclose(regout);
	fclose(count);
}



/*Utility Func*/
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
		printf("Address given, %X, is invalid - negative.\n", address);
		return -10;
	}

	if (address >= 4096)
	{
		printf("Address given, %X, is invalid - exceeds limited space in memory.\n", address);
		address = address & 0x0FFF; //if given address is too high, take only 12 LSBs.
		printf("Simulator refers only to 12 LSBs, %X in this case.\n", address);
	}

	return address;
}


/*Create Func*/
void createLastFiles(FILE* memout, char memory_out[MAX_LINES][SIZE], int max_line_counter, FILE* regout, int* reg[REG_SIZE], FILE* count, int counter)
{
    int i = 0;
	for (; i < REG_SIZE; i++)
	{
		fprintf(regout, "%04X\n", *reg[i]);
	}
	for (i = 0; i < max_line_counter; i++)
	{
		int instruction = getHex(memory_out[i]);
		if (instruction < 0)
			instruction -= 0xFFFF0000;
		fprintf(memout, "%04X\n" , instruction);
	}
	fprintf(count, "%d\n", counter);
}
void createTrace(FILE* trace, int pc, char line[SIZE], int* reg[REG_SIZE], int* count)
{
	fprintf(trace, "%04X %s ", pc, line);
	int i = 0;
    for (; i < 16; i++)
	{
		int instruction = *reg[i];
		if (instruction < 0)
			instruction -= 0xFFFF0000;
		fprintf(trace, "%04X ", instruction);
	}
	fprintf(trace, "\n");
	(*count)++;
}


/*LINE Func*/
void decipher_line(char line[SIZE], int* reg[REG_SIZE], char memory_in[MAX_LINES][LINE_SIZE], char memory_out[MAX_LINES][SIZE], int* pc, int* max_line_counter_ptr) {

	int opcode = hex2int(line[0]);
	int rd = hex2int(line[1]);
	int rs = hex2int(line[2]);
	int rt = hex2int(line[3]);
	if (opcode == 15)
	{
		halt(pc);
		return;
	}
	if (rd == 0 && opcode != 8 && opcode != 11 && opcode!=9)
	{
		if(opcode >= 7)
			*pc += 1;
		*pc += 1;
		return;
	}
	switch (opcode)
	{
	case 0:	add(rd ,rs, rt, reg); break;	//The opcode is add
	case 1: sub(rd, rs, rt, reg); break;    //The opcode is sub
	case 2: mul(rd, rs, rt, reg); break;    //The opcode is mul
	case 3: andf(rd, rs, rt, reg); break;    //The opcode is and
	case 4: orf(rd, rs, rt, reg); break;    //The opcode is or
	case 5: sll(rd, rs, rt, reg); break;    //The opcode is sll
	case 6: sra(rd, rs, rt, reg); break;    //The opcode is sra

	case 7: limm(rd, memory_in, reg, *pc); *pc += 1; break;    //The opcode is limm
	case 8: branch(rd, rs, rt, memory_in, reg, pc); *pc -= 1; break;    //The opcode is branch
	case 9: jal(reg, pc, memory_in); *pc -= 1; break;    //The opcode is jal
	case 10: lw(rd, rs, reg, memory_out, *pc); *pc += 1; break;    //The opcode is lw
	case 11: sw(rd, rs, reg, memory_in, memory_out, *pc, max_line_counter_ptr); *pc += 1; break;    //The opcode is sw

	case 15: halt(pc);    //The opcode is halt		
	}
	*pc += 1;
}
void add(int rd, int rs, int rt, int* reg[REG_SIZE])
{
	*reg[rd] = *reg[rs] + *reg[rt];
}
void sub(int rd, int rs, int rt, int* reg[REG_SIZE])
{
	*reg[rd] = *reg[rs] - *reg[rt];
}
void mul(int rd, int rs, int rt, int* reg[REG_SIZE])
{
	*reg[rd] = *reg[rs] * *reg[rt];
}
void andf(int rd, int rs, int rt, int* reg[REG_SIZE])
{
	*reg[rd] = *reg[rs] & *reg[rt];
}
void orf(int rd, int rs, int rt, int* reg[REG_SIZE])
{
	*reg[rd] = *reg[rs] | *reg[rt];
}
void sll(int rd, int rs, int rt, int* reg[REG_SIZE])
{
	*reg[rd] = *reg[rs] << *reg[rt];
}
void sra(int rd, int rs, int rt, int* reg[REG_SIZE])
{
	if (*reg[rs] >= (int)pow(2, 15))
	{
		*reg[rd] = *reg[rs] >> *reg[rt];
		int p = 15;
        int i = 0;
		for (; i < *reg[rt]; i++, p--)
			*reg[rd] += (int)pow(2, p);

	}
	else
		*reg[rd] = *reg[rs] >> *reg[rt];
}
void limm(int rd, char memory_in[MAX_LINES][4], int* reg[REG_SIZE], int pc)
{
	char line[SIZE];
	snprintf(line, SIZE, "%s%c", memory_in[pc + 1], '\0');
	*reg[rd] = getHex(line);
}
void branch(int rd, int rs, int rt, char memory_in[MAX_LINES][LINE_SIZE], int* reg[REG_SIZE], int* pc)
{
	char line[SIZE];
	snprintf(line, SIZE, "%s%c", memory_in[*pc + 1], '\0');
	int imm = getHex(line);
	switch (rd)
	{
	case 0:
		if (*reg[rs] == *reg[rt])  //beq
			*pc = getAddress(imm);
		else *pc += 2;
		break;
	case 1:
		if (*reg[rs] != *reg[rt])  //bne
			*pc = getAddress(imm);
		else *pc += 2;
		break;
	case 2:
		if (*reg[rs] > *reg[rt]) //branch if greater than - bgt
			*pc = getAddress(imm);
		else *pc += 2;
		break;
	case 3:
		if (*reg[rs] < *reg[rt]) // branch if smaller than - bst
			*pc = getAddress(imm);
		else *pc += 2;
		break;
	case 4:
		if (*reg[rs] >= *reg[rt]) // beq or bgt
			*pc = getAddress(imm);
		else *pc += 2;
		break;
	case 5:
		if (*reg[rs] <= *reg[rt]) // beq or bst
			*pc = getAddress(imm);
		else *pc += 2;
		break;
	case 6: *pc = getAddress(*reg[rs]);
	}
}
void jal(int** reg, int* pc, char memory_in[MAX_LINES][LINE_SIZE])
{
	*reg[15] = *pc + 2;
	char line[SIZE];
	snprintf(line, SIZE, "%s%c", memory_in[*pc + 1], '\0');
	int imm = getHex(line);
	*pc = getAddress(imm);
}
void lw(int rd, int rs, int* reg[REG_SIZE], char memory_out[MAX_LINES][SIZE], int pc)
{
	char line[SIZE], line1[SIZE];
	snprintf(line, SIZE, "%s%c", memory_out[pc + 1], '\0');
	int imm = getHex(line);
	int address = getAddress(imm + *reg[rs]);
	snprintf(line1, SIZE, "%s%c", memory_out[address], '\0');
	*reg[rd] = getHex(line1);
}
void sw(int rd, int rs, int* reg[REG_SIZE], char memory_in[MAX_LINES][LINE_SIZE], char memory_out[MAX_LINES][SIZE], int pc, int *max_line_counter_ptr)
{
	char line[SIZE];
	snprintf(line, SIZE, "%s%c", memory_in[pc + 1], '\0');
	int imm = getHex(line);
	int address = getAddress(imm + *reg[rs]);
	snprintf(memory_out[address], SIZE, "%04X%c", *reg[rd], '\n');
	if (*max_line_counter_ptr < address)//&& *reg[rd] != 0
		*max_line_counter_ptr = address;
}
void halt(int* pc)
{
	*pc = -10;//DESTROY!!
}
