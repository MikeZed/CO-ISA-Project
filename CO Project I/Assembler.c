

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
	MEMIN = fopen(argv[3], "w");

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



// performs first pass over the code
// to get the labels and their addresses
void read_file(FILE* asm_file, FILE* out_file, Label Labels[], int pass_num)
{

	char line[MAX_LINE_LEN];
	char* tokens[MAX_TOKENS_IN_LINE]; // tokens, will hold parts of the line

	int PC = 0;				   // current PC 
	int i = 1;

	while (fgets(line, MAX_LINE_LEN, asm_file) != NULL && i < 100) // read file line by line 
	{
		get_tokens(line, tokens);

		/*
		for (int j = 0; j < MAX_TOKENS_IN_LINE; j++)
		{
			if (tokens[i] != NULL);
				printf("%s ", tokens[j]);
		}
		printf(" \n");
		*/


/*
		PC += update_PC(tokens);

		// first pass - get labels
		if (pass_num == 1) 
			update_labels(tokens, PC, Labels);

		// second pass - get instructions in hex and write in file 
		else			
			write_instruction(out_file, tokens, Labels);


*/
		i++;
	}
}

// recieves tokens of the line and returns how much lines we should add to the PC
int update_PC(char* tokens[])
{



}

// splits line into tokens 
void get_tokens(char* line, char* tokens[])
{
	char* delimiters = " \n\t,:";    // line delimiters 

	char* number_sign_index = NULL;
	int is_comment_start = FALSE;

	char* token = strtok(line, delimiters);
	int tokens_index = 0;


	while (token != NULL && *token != '#')
	{
		number_sign_index = strchr(token, '#'); // number_sign - '#', holds address of '#' in token if exists

		if (number_sign_index != NULL)  // check if token has '#'
		{
			*number_sign_index = '\0';  // truncate token 
			is_comment_start = TRUE;       // indicates that after current token a comment has started
		}

		tokens[tokens_index] = token;
		tokens_index++;


		if (is_comment_start) // if a comment has started -> read next line 
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
void update_labels(char* tokens[], int PC, Label* Labels[]) 
{
	if (check_value(tokens[0]) != -1) // check if label is present in the line 
		return; 

	int i = 0;
	while (Labels[i]->address != - 1) // find empty label in the labels array 
		i++;

	strcpy(Labels[i]->name, tokens[0]); //add label to the array 
	Labels[i]->address = PC;

}

// a line starts with label, an opcode or ".word"
// the function recieves the first word in the line and returns:
// its opcode number if its an opcode,
// 16 if it's ".word",
// -1 if it's  a label and -2 if it's NULL 
int check_value(char* line_start)
{
	char* opcodes[] = OPCODES;
	
	if (line_start == NULL) return -2; 

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


int write_instruction(FILE* out_file, char* tokens, Label* Labels[])
{



}




	






























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
