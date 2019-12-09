

#include "Assembler.h"
#define MAX_TOKENS_IN_LINE 6

typedef	struct label {

	char name[MAX_LABEL_LEN];
	int address;

} Label;


int check_value(char* line_start);
void read_file(FILE* asm_file, Label* Labels[], int pass_num, FILE* out_file);
int update_PC(char* tokens[]);
void get_tokens(char* line, char* tokens[]);
void intialize_labels_array(Label* Labels[]);
void update_labels(char* tokens[], int PC, Label* Labels[]);
void write_instruction(FILE* out_file, char* tokens[], Label Labels[]); 


int main(int argc, char** argv)
{
	FILE * ASM_file = NULL; 
	FILE * MEMIN = NULL; 

	Label Labels[MAX_LINES];

	intialize_labels_array(Labels); 
//	for (int i = 0; i < argc; i++)
//	{
//		printf("%s\n", argv[i]);
//	}
	
	ASM_file = fopen(argv[2], "r");
	MEMIN = fopen(argv[3], "w+");

	if (ASM_file == NULL || MEMIN == NULL)
	{
		printf("Couldn't open file, terminating process\n");
		exit(1); 
	}

	// read file, perform first pass 
	read_file(ASM_file, NULL, Labels, 1);

	// read file, perform second pass 
	read_file(ASM_file, MEMIN, Labels, 2);

	fclose(ASM_file);
	fclose(MEMIN);

	return 0; 
}


// performs first pass and second pass over the code
// pass_num = 1 -> first pass  -> get labels      (get the labels and their addresses)
// pass_num = 2 -> second pass -> write to memory (write instructions)
void read_file(FILE* asm_file, FILE* out_file, Label Labels[], int pass_num)
{
	char line[MAX_LINE_LEN];
	char* tokens[MAX_TOKENS_IN_LINE]; // tokens, will hold parts of the line

	int PC = 0;				   // current PC 

	while (fgets(line, MAX_LINE_LEN, asm_file) != NULL) // read file line by line 
	{
		get_tokens(line, tokens);

		/*

		for (int j = 0; j < MAX_TOKENS_IN_LINE; j++)
		{
			if (tokens[j] != NULL)
				printf("%s ", tokens[j]);

			if (j == MAX_TOKENS_IN_LINE - 1)
			{
				printf(" %x \n", PC);
				break;
			}
		}
		*/

		// first pass - get labels
		if (pass_num == 1)
			update_labels(tokens, PC, Labels);

		// second pass - get instructions in hex and write in file
		else 
		{
			write_instruction(out_file, tokens, Labels);
		}
		PC += update_PC(tokens);
	}
	rewind(asm_file); 
	//for (int i = 0; i < 10; i++)
	//	if  (Labels[i].address!=-1)
	//		printf("%s %d\n", Labels[i].name, Labels[i].address);

}

// recieves tokens of the line and returns how much lines we should add to the PC
int update_PC(char* tokens[])
{
	int first_token = check_value(tokens[0]);
	int opcode;
	int rd_index = 2; // token index of the first register in the instruction 

	if (first_token == -2) return 0; // check if empty line 

	if (first_token == -1)  // check if first token is label
	{
		opcode = check_value(tokens[1]); // first token is label -> second one is opcode if exists

		if (opcode == -2)  // check if second token is NULL 
			return 0;
	}
	else  // first token is opcode
	{
		opcode = first_token;
		rd_index = 1;
	}

	if (opcode <= 6 || opcode == 15)  // opcode is "add", "sub", "mul", "and" ,"or", "sll", "sra" or "halt"
		return 1;
	else     // opcode is "limm", "branch", "jal", "lw" or "sw"
	{     
		if (opcode == 8 && strcmp(tokens[rd_index], "jr") == 0)return 1; // if opcode is "branch" and rd is "jr" increase PC by 1 
		else return 2; // else increase PC by 2
	}
}

// splits line into tokens 
void get_tokens(char* line, char* tokens[])
{
	char* delimiters = " \n\t,:";    // line delimiters 

	char* number_sign_index = NULL;

	int is_comment_start = FALSE;

	int tokens_index = 0;

	char* token = strtok(line, delimiters);

	while (token != NULL && *token != '#') // check if token is not NULL and doesnt begin with '#' 
	{
		number_sign_index = strchr(token, '#'); // number_sign - '#', holds address of '#' in token if exists

		if (number_sign_index != NULL)  // check if token has '#'
		{
			*number_sign_index = '\0';  // truncate token 
			is_comment_start = TRUE;    // indicates that after current token a comment has started
		}

		tokens[tokens_index] = token;
		tokens_index++;

		if (is_comment_start) // if a comment has started -> no more tokens 
			break;

		token = strtok(NULL, delimiters); // read next token 
	}

	for (int i = tokens_index; i < MAX_TOKENS_IN_LINE; i++) // set rest tokens to NULL 
		tokens[i] = NULL; 
}


// intializes the labels array so that we know which labels are not empty
void intialize_labels_array(Label Labels[])
{
	for (int i = 0; i < MAX_LINES; i++)
		Labels[i].address = -1; 
}


// updates the labels array 
void update_labels(char* tokens[], int PC, Label Labels[]) 
{
	if (check_value(tokens[0]) != -1) // check if label is present in the line 
		return; 

	int i = 0;
	while (Labels[i].address != - 1) // find empty label in the labels array 
		i++;

	strcpy(Labels[i].name, tokens[0]); //add label to the array 
	Labels[i].address = PC;

}


// opcode reg1, reg2, reg3, imm -> AAAA
void write_instruction(FILE* out_file, char* tokens[], Label Labels[])
{
	int opcode, rd, rs, rt, imm;
	int first_token, index_offset = 0;
	int use_imm = FALSE;

	first_token = check_value(tokens[0]);

	if (first_token == -2) return; // check if empty line 

	if (first_token == -1)  // check if first token is label
	{
		index_offset = 1; // instruction starts after first token

		int second_token = check_value(tokens[1]);

		if (second_token == -2)  // check if second token is NULL 
			return;
	}

	opcode = check_value(tokens[0 + index_offset]);
	//printf("%u\n", opcode);

	if (opcode == 16) // if opcode is ".word"
	{
		write_to_memory(out_file, tokens);
		return;
	}

	rd = get_reg(tokens[1 + index_offset]);
	rs = get_reg(tokens[2 + index_offset]);
	rt = get_reg(tokens[3 + index_offset]);

	imm = get_imm(tokens[4 + index_offset], Labels);


	if (opcode >= 7 && opcode <= 11)   // opcode is "limm", "branch", "jal", "lw" or "sw"
	{
		use_imm = TRUE; 
		if (opcode == 8 && rd == 6) // if opcode is "branch" and rd is "jr" increase PC by 1 
			use_imm = FALSE; 
	}

	fprintf(out_file,  "%X%X%X%X\n", opcode, rd, rs, rt); 

	//printf("%u %u %u %u\n", opcode, rd, rs, rt);
	//printf( "%X%X%X%X\n", opcode, rd, rs, rt);

	if (use_imm)
		fprintf(out_file, "%04X\n", imm);
		

}

// a line starts with label, an opcode or ".word"
// the function recieves the first word in the line and returns:
// its opcode number if its an opcode,
// 16 if it's ".word",
// -1 if it's  a label and -2 if it's NULL 
int check_value(char* line_start)
{
	if (line_start == NULL) return -2;

	char* opcodes[] = OPCODES;

	for (int i = 0; i < OPCODES_LEN; i++)
		if (strcmp(line_start, opcodes[i]) == 0)
			return i;

	return -1;
}


// recieves register str, returns register number in hex 
int get_reg(char* reg)
{
	char* regs[] = REGS;
	char* branch_rd[] = BRANCH_RD;

	for (int i = 0; i < REGS_LEN; i++) // check which register reg is
		if (strcmp(reg, regs[i]) == 0)
			return i;

	for (int i = 0; i < BRANCH_RD_LEN; i++) 	// if reg is not one of the register it is part of the branch instruction
		if (strcmp(reg, branch_rd[i]) == 0)
			return i;

	return -1; // shouldn't get here
} 

// converts string of a number (decimal or hex) to int
int get_imm(char* str, Label Labels[])
{



}
// .word 
write_to_memory(FILE* out_file, char* tokens[])
{
	int address, data; 




}



	

// TODO - add label using, .word , imm conversion from correct base 

































/*


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


	*/