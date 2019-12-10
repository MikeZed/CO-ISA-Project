//הקובץ עובד פרט לטעות אחת הקובץ ממאאוט חסר לו טעינות. כלומר לא מדויק חסר לו נתונים. שאר הקבצים תקינים ונכונים.
#include <iostream>
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1 
#define FALSE 0

#define MAX_LINES 4096 
#define SIZE 5
#define LINE_SIZE 4
#define REG_SIZE 16
/*Utility func*/
int getHex(char* source);
int hex2int(char ch);
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
void sw(int rd, int rs, int* reg[REG_SIZE], char memory_in[MAX_LINES][LINE_SIZE], char memory_out[MAX_LINES][SIZE], int pc, int max_line_counter);
void halt(int* pc);
void decipher_line(char line[SIZE], int* reg[REG_SIZE], char memory_in[MAX_LINES][LINE_SIZE], char memory_out[MAX_LINES][SIZE], int* pc, int max_line_counter);

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
	
	int cnt = 0, pc = 0; int max_line_counter = 0;
	int reg_b[REG_SIZE] = { 0 };
	int* reg[REG_SIZE];
	for (int i = 0; i < REG_SIZE; i++)
	{
		reg[i] = &reg_b[i];
	}
	char memory_in[MAX_LINES][LINE_SIZE], memory_out[MAX_LINES][SIZE];
	int* counter = &cnt; int* pcp = &pc;

	while (!feof(memin))//memory_in and memory_out are filled
	{
		int check;
		if (check = fscanf(memin, "%s", memory_in[max_line_counter]) != 0) {
			snprintf(memory_out[max_line_counter],SIZE,"%s%c", memory_in[max_line_counter],'\n');
			max_line_counter++;
		}	
	}
	fclose(memin);

	while (pc > -1)
	{
		char line[SIZE];
		snprintf(line, SIZE, "%s%c", memory_in[pc], '\0');
		/*for (int i = 0; i < 4; i++)
		{
			line[i] = memory_in[pc][i];
			printf("%c", line[i]);
		}
		line[4] = '\0';
		printf("\n%c\n", memory_in[pc][1]);*/		
		/*strncpy(line, memory_in[pc], 4);
		line[4] = '\0';*/
		printf("%s\n", line);
		createTrace(trace, pc, line, reg, counter);
		decipher_line(line, reg, memory_in, memory_out, pcp, max_line_counter);
		printf(" %d ", *counter);
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
	return (int)strtol(source, NULL, 16);
}
int hex2int(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;
	return -1;
}
/*Create Func*/
void createLastFiles(FILE* memout, char memory_out[MAX_LINES][SIZE], int max_line_counter, FILE* regout, int* reg[REG_SIZE], FILE* count, int counter)
{
	for (int i = 0; i < REG_SIZE; i++)
	{
		fprintf(regout, "%04X\n", *reg[i]);
	}
	for (int i = 0; i < max_line_counter; i++)
	{
		fprintf(memout, "%04X\n", getHex(memory_out[i]));
	}
	fprintf(count, "%d\n", counter);
}
void createTrace(FILE* trace, int pc, char line[SIZE], int* reg[REG_SIZE], int* count)
{
	fprintf(trace, "%04X %s ", pc, line);
	for (int i = 0; i < 16; i++)
	{
		fprintf(trace, "%04X ", *reg[i]);
	}
	fprintf(trace, "\n");
	(*count)++;
}

/*LINE Func*/
void decipher_line(char line[SIZE], int* reg[REG_SIZE], char memory_in[MAX_LINES][LINE_SIZE], char memory_out[MAX_LINES][SIZE], int* pc, int max_line_counter) {
	
	int opcode = hex2int(line[0]);
	int rd = hex2int(line[1]);
	int rs = hex2int(line[2]);
	int rt = hex2int(line[3]);

	switch (opcode)
	{
	case 0:	add(rd, rs, rt, reg); break;	//The opcode is add
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
	case 11: sw(rd, rs, reg, memory_in, memory_out, *pc, max_line_counter); *pc += 1; break;    //The opcode is sw

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
		for (int i = 0; i < *reg[rt]; i++, p--)
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
	case 0: if (*reg[rs] == *reg[rt]) *pc = imm; //beq
		  else *pc += 2;
		break;
	case 1: if (*reg[rs] != *reg[rt]) *pc = imm; //bne
		  else *pc += 2;
		break;
	case 2: if (*reg[rs] > *reg[rt]) *pc = imm; //branch if greater than - bgt
		  else *pc += 2;
		break;
	case 3: if (*reg[rs] < *reg[rt]) *pc = imm; // branch if smaller than - bst
		  else *pc += 2;
		break;
	case 4: if (*reg[rs] >= *reg[rt]) *pc = imm; // beq or bgt
		  else *pc += 2;
		break;
	case 5: if (*reg[rs] <= *reg[rt]) *pc = imm; // beq or bst
		  else *pc += 2;
		break;
	case 6: *pc = *reg[rs];
	}
}
void jal(int** reg, int* pc, char memory_in[MAX_LINES][LINE_SIZE])
{
	*reg[15] = *pc + 2;
	char line[SIZE];
	snprintf(line,SIZE,"%s%c", memory_in[*pc + 1], '\0');
	int imm = getHex(line);
	*pc = imm;
}
void lw(int rd, int rs, int* reg[REG_SIZE], char memory_out[MAX_LINES][SIZE], int pc)
{
	char line[SIZE], line1[SIZE];
	snprintf(line, SIZE, "%s%c", memory_out[pc + 1], '\0');
	int imm = getHex(line);

	snprintf(line1, SIZE, "%s%c", memory_out[imm + *reg[rs]], '\0');
	*reg[rd] =getHex(line1);
}
void sw(int rd, int rs, int* reg[REG_SIZE], char memory_in[MAX_LINES][LINE_SIZE], char memory_out[MAX_LINES][SIZE], int pc, int max_line_counter)
{
	char line[SIZE];
	snprintf(line, SIZE, "%s%c", memory_in[pc + 1], '\0');
	int imm = getHex(line);

	snprintf(memory_out[imm + *reg[rs]], SIZE, "%04X" , *reg[rd]);
	if (max_line_counter > imm + *reg[rs])
		max_line_counter = imm + *reg[rs];
}
void halt(int* pc)
{
	*pc = -1000;
}
