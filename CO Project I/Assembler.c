

#include <stdio.h>

#define TRUE 1 
#define FALSE 0

#define MAX_LINES 4096 
#define MAX_LINE_LEN 500 
#define MAX_LABEL_LEN 50


typedef	struct label {

	char* name; 
	int address;

} Label;


int get_labels(); 
int check_label();
int get_instruction(); 
int get_opcode();



int main(int argc, char** argv)
{

	FILE * ASM_file = NULL; 
	Label* Labels[MAX_LINES];


//	for (int i = 0; i < argc; i++)
//	{
//		printf("%s\n", argv[i]);
//	}
	

	ASM_file = fopen(argv[2], 'r'); 

	if (ASM_file == NULL)
	{
		printf("Couldn't open file, terminating process\n");
		exit(1); 
	}


	fclose(ASM_file);



	return 0; 

}


// performs first pass over the code
// to get the labels and their addresses

int get_labels();



int get_instruction();




	