#include "pch.h"
#include <iostream>
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1 
#define FALSE 0

#define MAX_LINES 4096 


void add(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] + reg[rt];
}
void sub(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] - reg[rt];
}
void mul(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] * reg[rt];
}
void andf(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] & reg[rt];
}
void orf(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] | reg[rt];
}
void sll(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] << reg[rt];
}
void sra(int rd, int rs, int rt, int reg[])
{
	if (reg[rs] >= pow(2, 15))
	{
		reg[rd] = reg[rs] >> reg[rt];
		int p = 15;
		for (int i = 0; i < reg[rt]; i++, p--)
			reg[rd] += 2 ^ p;

	}
	else
		reg[rd] = reg[rs] >> reg[rt];
}

void limm(int rd, char memory_in[MAX_LINES][4], int reg[], int pc)
{
	reg[rd] = atoi(memory_in[pc]);
}

void branch(int rd, int rs, int rt, int reg[], int* pc, char memory_in[MAX_LINES][4])
{
	int imm = atoi(memory_in[*pc + 1]);
	switch (rd)
	{
	case 0: if (reg[rs] == reg[rt]) *pc = imm; //beq
			else *pc += 2;
		break;
	case 1: if (reg[rs] != reg[rt]) *pc = imm; //bne
			else *pc += 2;
		break;
	case 2: if (reg[rs] > reg[rt]) *pc = imm; //branch if greater than - bgt
			else *pc += 2;
		break;
	case 3: if (reg[rs] < reg[rt]) *pc = imm; // branch if smaller than - bst
			else *pc += 2;
		break;
	case 4: if (reg[rs] >= reg[rt]) *pc = imm; // beq or bgt
			else *pc += 2;
		break;
	case 5: if (reg[rs] <= reg[rt]) *pc = imm; // beq or bst
			else *pc += 2;
		break;
	case 6: *pc = reg[rs];
	}
}

void jal(int reg[], int* pc, char memory_in[MAX_LINES][4])
{
	reg[15] = *pc + 2;
	*pc = atoi(memory_in[*pc + 1]);
}

void lw(int rd, int rs, int reg[], char memory_in[MAX_LINES][4], int pc)
{
	reg[rd] = atoi(memory_in[atoi(memory_in[pc + 1]) + reg[rs]]);
}
void sw(int rd, int rs, int reg[], char memory_in[MAX_LINES][4], char memory_out[MAX_LINES][4], int pc, int max_line_counter)
{
	strcpy(memory_out[atoi(memory_out[pc]) + reg[rs]], "" + reg[rd]);
	if (max_line_counter > atoi(memory_in[pc + 1]) + reg[rs])
		max_line_counter = atoi(memory_in[pc + 1]) + reg[rs];
}

void halt(int* pc)
{
	*pc = -2;
}


void decipher_line(char line[], int reg[], char memory_in[MAX_LINES][4], char memory_out[MAX_LINES][4], int* pc, int max_line_counter)
{
	int opcode = line[0] - '0';
	int rd = line[1] - '0';
	int rs = line[2] - '0';
	int rt = line[3] - '0';

	switch (line[0])
	{
	case 0:	add(rd, rs, rt, reg); break;	//The opcode is add
	case 1: sub(rd, rs, rt, reg); break;    //The opcode is sub
	case 2: mul(rd, rs, rt, reg); break;    //The opcode is mul
	case 3: andf(rd, rs, rt, reg); break;    //The opcode is and
	case 4: orf(rd, rs, rt, reg); break;    //The opcode is or
	case 5: sll(rd, rs, rt, reg); break;    //The opcode is sll
	case 6: sra(rd, rs, rt, reg); break;    //The opcode is sra

	case 7: limm(rd, memory_in, reg, *pc); *pc += 1; break;    //The opcode is limm
	case 8: branch(rd, rs, rt, reg, pc, memory_in); *pc -= 1; break;    //The opcode is branch
	case 9: jal(reg, pc, memory_in); *pc -= 1; break;    //The opcode is jal
	case 10: lw(rd, rs, reg, memory_in, *pc); *pc += 1; break;    //The opcode is lw
	case 11: sw(rd, rs, reg, memory_in, memory_out, *pc, max_line_counter); *pc += 1; break;    //The opcode is sw

	case 15: halt(pc);    //The opcode is halt		
	}
	*pc += 1;

}

void createTrace(FILE* trace, int pc, char line[], int reg[], int* count)
{
	fprintf(trace, "%x %s ", pc, line);
	for (int i = 0; i < 16; i++)
	{
		fprintf(trace, "%x ", reg[i]);
	}
	fprintf(trace, "\n");
	(*count)++;
}

void createRegOut(FILE* regout, int reg[])
{
	for (int i = 0; i < 16; i++)
	{
		fprintf(regout, "%x\n", reg[i]);
	}
}

void createMemout(FILE* memout, char memory_out[MAX_LINES][4], int max_line_counter)
{
	for (int i = 0; i < max_line_counter; i++)
	{
		fprintf(memout, "%04X\n", atoi(memory_out[i]));
	}

}

int main(int argc, const char* argv[])
{
	if (argc < 6)
	{
		printf("Arg Amount Error");
		return 0;
	}
	FILE* memin, *memout, *regout, *trace, *count;
	memin = fopen(argv[1], "r");
	memout = fopen(argv[2], "w");
	regout = fopen(argv[3], "w");
	trace = fopen(argv[4], "w");
	count = fopen(argv[5], "w");

	if (memin == NULL || memout == NULL || regout == NULL || trace == NULL || count == NULL)
	{
		printf("FILE Error");
		return 0;
	}
	int cnt = 0, pc = 0, reg[16] = { 0 }, max_line_counter = 0;
	char memory_in[MAX_LINES][4], memory_out[MAX_LINES][4], line[4];
	int* counter = &cnt, *pcp = &pc;

	while (!feof(memin))//memory_in and memory_out are filled
	{
		int check;
		if (check = fscanf(memin, "%s", memory_in[max_line_counter]) != 0) {
			strcpy(memory_out[max_line_counter], memory_in[max_line_counter]);
			max_line_counter++;
		}
	}
	fclose(memin);

	while (*pcp > -1)
	{
		strcpy(line, memory_in[*pcp]);
		createTrace(trace, *pcp, line, reg, counter);
		decipher_line(line, reg, memory_in, memory_out, pcp, max_line_counter);
	}
	//Exit:	
	fclose(trace);
	createRegOut(regout, reg);
	fclose(regout);
	createMemout(memout, memory_out, max_line_counter);


}












































#include <iostream>
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1 
#define FALSE 0

#define MAX_LINES 4096 


void add(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] + reg[rt];
}
void sub(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] - reg[rt];
}
void mul(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] * reg[rt];
}
void andf(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] & reg[rt];
}
void orf(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] | reg[rt];
}
void sll(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] << reg[rt];
}
void sra(int rd, int rs, int rt, int reg[])
{
	if (reg[rs] >= pow(2,15))
	{
		reg[rd] = reg[rs] >> reg[rt];
		int p = 15;
		for (int i = 0; i < reg[rt]; i++, p--)
			reg[rd] += 2 ^ p;

	}
	else
		reg[rd] = reg[rs] >> reg[rt];
}

void limm(int rd, char memory_in[MAX_LINES][4], int reg[], int pc)
{
	reg[rd] = atoi(memory_in[pc]);
}

void branch(int rd, int rs, int rt, int reg[], int* pc, char memory_in[MAX_LINES][4])
{
	int imm = atoi(memory_in[*pc + 1]);
	switch (rd)
	{
	case 0: if (reg[rs] == reg[rt]) *pc = imm; //beq
		  else *pc += 2;
		break;
	case 1: if (reg[rs] != reg[rt]) *pc = imm; //bne
		  else *pc += 2;
		break;
	case 2: if (reg[rs] > reg[rt]) *pc = imm; //branch if greater than - bgt
		  else *pc += 2;
		break;
	case 3: if (reg[rs] < reg[rt]) *pc = imm; // branch if smaller than - bst
		  else *pc += 2;
		break;
	case 4: if (reg[rs] >= reg[rt]) *pc = imm; // beq or bgt
		  else *pc += 2;
		break;
	case 5: if (reg[rs] <= reg[rt]) *pc = imm; // beq or bst
		  else *pc += 2;
		break;
	case 6: *pc = reg[rs];
	}
}

void jal(int reg[], int* pc, char memory_in[MAX_LINES][4])
{
	reg[15] = *pc + 2;
	*pc = atoi(memory_in[*pc + 1]);
}

void lw(int rd, int rs, int reg[], char memory_in[MAX_LINES][4], int pc)
{
	reg[rd] = atoi(memory_in[atoi(memory_in[pc + 1]) + reg[rs]]);
}
void sw(int rd, int rs, int reg[], char memory_in[MAX_LINES][4], char memory_out[MAX_LINES][4], int pc, int max_line_counter)
{
	strcpy(memory_out[atoi(memory_out[pc]) + reg[rs]], "" + reg[rd]);
	if (max_line_counter > atoi(memory_in[pc + 1]) + reg[rs])
		max_line_counter = atoi(memory_in[pc + 1]) + reg[rs];
}

void halt(int* pc)
{
	*pc = -2;
}


void decipher_line(char line[], int reg[], char memory_in[MAX_LINES][4], char memory_out[MAX_LINES][4], int* pc, int max_line_counter)
{
	int opcode = line[0] - '0';
	int rd = line[1] - '0';
	int rs = line[2] - '0';
	int rt = line[3] - '0';

	switch (line[0])
	{
	case 0:	add(rd, rs, rt, reg); break;	//The opcode is add
	case 1: sub(rd, rs, rt, reg); break;    //The opcode is sub
	case 2: mul(rd, rs, rt, reg); break;    //The opcode is mul
	case 3: andf(rd, rs, rt, reg); break;    //The opcode is and
	case 4: orf(rd, rs, rt, reg); break;    //The opcode is or
	case 5: sll(rd, rs, rt, reg); break;    //The opcode is sll
	case 6: sra(rd, rs, rt, reg); break;    //The opcode is sra

	case 7: limm(rd, memory_in, reg, *pc); *pc += 1; break;    //The opcode is limm
	case 8: branch(rd, rs, rt, reg, pc, memory_in); *pc -= 1; break;    //The opcode is branch
	case 9: jal(reg, pc, memory_in); *pc -= 1; break;    //The opcode is jal
	case 10: lw(rd, rs, reg, memory_in, *pc); *pc += 1; break;    //The opcode is lw
	case 11: sw(rd, rs, reg, memory_in, memory_out, *pc, max_line_counter); *pc += 1; break;    //The opcode is sw

	case 15: halt(pc);    //The opcode is halt		
	}
	*pc += 1;

}

void createTrace(FILE* trace, int pc, char line[], int reg[], int* count)
{
	fprintf(trace, "%x %s ", pc, line);
	for (int i = 0; i < 16; i++)
	{
		fprintf(trace, "%x ", reg[i]);
	}
	fprintf(trace, "\n");
	(*count)++;
}

void createRegOut(FILE* regout, int reg[])
{
	for (int i = 0; i < 16; i++)
	{
		fprintf(regout, "%x\n", reg[i]);
	}
}

void createMemout(FILE* memout, char memory_out[MAX_LINES][4], int max_line_counter)
{
	for (int i = 0; i < max_line_counter; i++)
	{
		fprintf(memout, "%04X\n", atoi(memory_out[i]));
	}

}

int main(int argc, const char* argv[])
{
	if (argc < 6)
	{
		printf("Arg Amount Error");
		return 0;
	}
	FILE* memin, * memout, * regout, * trace, * count;
	memin = fopen(argv[1], "r");
	memout = fopen(argv[2], "w");
	regout = fopen(argv[3], "w");
	trace = fopen(argv[4], "w");
	count = fopen(argv[5], "w");

	if (memin == NULL || memout == NULL || regout == NULL || trace == NULL || count == NULL)
	{
		printf("FILE Error");
		return 0;
	}
	int cnt = 0, pc = 0, reg[16] = { 0 }, max_line_counter = 0;
	char memory_in[MAX_LINES][4], memory_out[MAX_LINES][4], line[4];
	int* counter = &cnt, * pcp = &pc;

	while (!feof(memin))//memory_in and memory_out are filled
	{
		int check;
		if (check = fscanf(memin, "%s", memory_in[max_line_counter]) != 0) {
			strcpy(memory_out[max_line_counter], memory_in[max_line_counter]);
			max_line_counter++;
		}	
	}
	fclose(memin);

	while (*pcp > -1)
	{
		strcpy(line, memory_in[*pcp]);
		createTrace(trace, *pcp, line, reg, counter);
		decipher_line(line, reg, memory_in, memory_out, pcp, max_line_counter);
	}
	//Exit:	
	fclose(trace);
	createRegOut(regout, reg);
	fclose(regout);
	createMemout(memout, memory_out, max_line_counter);


}
