

#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include "Assembler.h"

#define TRUE 1 
#define FALSE 0

#define MAX_LINES 4096 
#define MAX_LINE_LEN 500 
#define MAX_LABEL_LEN 50
#define OPCODES { "add", "sub", "mul", "and" ,"or", "sll", "sra", "limm", "branch", "jal", "lw", "sw", "0ph", "1ph", "2ph","halt", ".word"} 
// all values' indices match their opcode number! "0ph", "1ph" and "2ph" are place holders so the "halt"'s index would be 15, 
// they can't be labels because they start with a digit
#define OPCODES_LEN 17 // the length of the array above 

#define REGS { "$zero", "$at", "$v0", "$a0 " ,"$a1 ", "$t0", "$t1", "$t2", "$t3", "$s0", "$s1", "$s2 ", "$gp", "$sp", "$fp","$ra"} 
// all registers' indices match their number! 
#define REGS_LEN 16 // the length of the array above 





typedef	struct label {

	char name[MAX_LABEL_LEN];
	int address;
	int is_Empty;

} Label;


void read_file();
int return_value();
int get_instruction(); 
int get_opcode();

int main(int argc, char** argv)
{
	FILE * ASM_file = NULL; 
	FILE * MEMIN = NULL; 

	Label* Labels[MAX_LINES];

	intialize_labels_array(Labels); 
//	for (int i = 0; i < argc; i++)
//	{
//		printf("%s\n", argv[i]);
//	}
	
	ASM_file = fopen(argv[2], "r");
	MEMIN = fopen(argv[3], "w");

	if (ASM_file == NULL || MEMIN == NULL)
	{
		printf("Couldn't open file, terminating process\n");
		exit(1); 
	}

	// read file, perform first pass 
	read_file(ASM_file, Labels, 1, NULL); 

	// read file, perform second pass 
	read_file(ASM_file, Labels, 2, MEMIN);

	fclose(ASM_file);
	fclose(MEMIN);

	return 0; 
}


// SUGGESTION - SAVE ALL TOKENS AFTER READING A LINE INSTEAD OF ACCESSING THEM ONE BY ONE

// performs first pass over the code
// to get the labels and their addresses
void read_file(FILE* asm_file, Label* Labels[], int pass_num, FILE* out_file)
{

	char line[MAX_LINE_LEN];
	char* delim = " \n\t,:";    // line delimiters 

	char* token;               // a token, will hold part of the line
	int is_comment_start = FALSE; // if a token contains '#' it means a comment has started
	int is_line_start = TRUE;

	char* number_sign_index = NULL;

	int PC = 0;				   // current PC 

	int i = 1;

	while (fgets(line, MAX_LINE_LEN, asm_file) != NULL && i < 100) // read file line by line 
	{
		is_line_start = TRUE;
		is_comment_start = FALSE;

		token = strtok(line, delim); // get first token in line


		while (token != NULL && *token != '#')
		{

			number_sign_index = strchr(token, '#'); // number_sign - '#', holds address of '#' in token if exists

			if (number_sign_index != NULL)  // check if token has '#'
			{
				*number_sign_index = '\0';  // truncate token 
				is_comment_start = TRUE;       // indicates that after current token a comment has started
			}

			// do stuff with token 






			printf("%s \n", token);

			// first pass - get labels
			if (pass_num == 1 && is_line_start)
				update_labels(token, PC, Labels); 

		
			// second pass - get instructions in hex and write in file 
			else
			{




			}








			// end 


			if (is_comment_start) // if a comment has started -> read next line 
				break;

			token = strtok(NULL, delim); // read next token 
			is_line_start = FALSE; 

		}
	}
}


// intializes the labels array so that we know which labels are not empty
void intialize_labels_array(Label* Labels[])
{
	for (int i = 0; i < MAX_LINES; i++)
		Labels[i]->is_Empty = TRUE;
}


// updates the labels array 
void update_labels(char* token, int PC, Label* Labels[]) 
{
	if (return_value(token) != -1)
		return; 

	int i = 0;
	while (!(Labels[i]->is_Empty))
		i++;

	strcpy(Labels[i]->name, token);
	Labels[i]->address = PC;

}

// a line starts with label, an opcode or ".word"
// the function recieves the first word in the line and returns:
// its opcode number if its an opcode,
// 16 if it's ".word",
// -1 if it's  a label and 0 if it's NULL 
int return_value(char* line_start)
{
	char* opcodes[] = OPCODES;
	
	if (line_start == NULL) return 0; 

	for (int i = 0; i < OPCODES_LEN; i++)
	{
		if (strcmp(line_start, opcodes[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}


// recieves register str, returns register number in hex 
int get_reg()
{



}


int get_instruction();




	