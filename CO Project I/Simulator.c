#define _CRT_SECURE_NO_WARNINGS

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
void and(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] & reg[rt];
}
void or(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] | reg[rt];
}
void sll(int rd, int rs, int rt, int reg[])
{
	reg[rd] = reg[rs] << reg[rt];
}
void sra(int rd, int rs, int rt, int reg[])
{
	for(int i = 0; i < reg[rt]; i++)
	{
		int lsb = reg[rs]//Need to get the lsb of reg[rs], check if reg[rs] is binary if not convert to binary then take the lsb.
		reg[rd] = reg[rs] >> 1;
		reg[rs][msb] = lsb; 
		// This code is incorrect, we need to find out how to get to the msb position and put the pervious lsb in there.
	}
}

void limm(int rd, char memory_in[], int reg[], int pc)
{
	reg[rd] = atoi(memory_in[pc]);
}
void branch(int rd, int rs, int rt, int reg[], int *pc, int memory_in[][])
{
	int imm = atoi(memory_in[*pc+1]);
	switch(rd)
	{
		case 0: if(reg[rs]==reg[rt]) *pc = imm; \\beq
				else *pc += 2; 
				break:
		case 1: if(reg[rs]!=reg[rt]) *pc = imm; \\bne
				else *pc += 2; 
				break:
		case 2: if(reg[rs]>reg[rt]) *pc = imm; \\branch if greater than - bgt
				else *pc += 2; 
				break:
		case 3: if(reg[rs]<reg[rt]) *pc = imm; \\ branch if smaller than - bst
				else *pc += 2; 
				break:
		case 4: if(reg[rs]>=reg[rt]) *pc = imm; \\ beq or bgt
				else *pc += 2; 
				break:
		case 5: if(reg[rs]<=reg[rt]) *pc = imm; \\ beq or bst
				else *pc += 2; 
				break:
		case 6: *pc = reg[rs];
	}
}

void jal(int reg[], int *pc, int memory_in[][])
{
	reg[15]=*pc+2;
	*pc=atoi(memory_in[*pc+1]);
}
void lw(int rd, int rs, int reg[], char memory_in[], int pc)
{
	reg[rd] = atoi(memory_in[atoi(memory_in[pc])+ reg[rs]]);
}
void sw(int rd, int rs, int reg[], char memory_out[], int pc, int max_line_counter)
{
	memory_out[atoi(memory_in[pc])+ reg[rs]] = itoa(reg[rd]);
	if(max_line_counter > atoi(memory_in[pc])+ reg[rs] )
		max_line_counter = atoi(memory_in[pc])+ reg[rs];

void halt(int *pc)
{
	*pc = -1;
}


decipher_line(char line[],int reg[],char memory_in[],char memory_out[],int *pc, int max_line_counter)
{
	int opcode = line[0] - '0'; 
	int rd = line[1] - '0';
	int rs = line[2] - '0';
	int rt = line[3] - '0';
	
	switch(line[0])
	{
		case 0:	add(rd,rs,rt,reg);break;	//The opcode is add
		case 1: sub(rd,rs,rt,reg);break;    //The opcode is sub
		case 2: mul(rd,rs,rt,reg);break;    //The opcode is mul
		case 3: and(rd,rs,rt,reg);break;    //The opcode is and
		case 4: or(rd,rs,rt,reg);break;    //The opcode is or
		case 5: sll(rd,rs,rt,reg);break;    //The opcode is sll
		case 6: sra(rd,rs,rt,reg);break;    //The opcode is sra
		
		case 7: limm(rd,memory_in,reg,*pc);*pc+=1;break;    //The opcode is limm
		case 8: branch(rd,rs,rt,reg,pc);*pc-=1;break;    //The opcode is branch
		case 9: jal(rd,rs,rt,reg);*pc-=1;break;    //The opcode is jal
		case 10: lw(rd,rs,reg,memory_in,*pc);*pc+=1;break;    //The opcode is lw
		case 11: sw(rd,rs,rt,reg,memory_out,max_line_counter);*pc+=1;break;    //The opcode is sw
		
		case 15: halt(pc);break;    //The opcode is halt		
	}
	*pc +=1
	
}
  
  void createTrace(FILE *trace, int pc, char line[], int reg[], int *count)
{
	fprintf(trace,"%x %s ",pc,line);
	for(int i =0 ; i<16 ; i++)
	{
		fprintf(trace, "%x ",reg[i]);
	}
	fprintf(trace, "\n");
	*count++;
}
  
void createRegOut(FILE *regout,int reg[])
{
	for(int i =0 ; i<16 ; i++)
	{
		fprintf(regout, "%x ",reg[i]);
	}
	fprintf(regout, "\n");
}

  
  
int main()
{
	FILE *memin,*memout,*regout,*trace,*count;
	memin = fopen("memin.txt","r");
	trace = fopen("trace.txt","w");
	memout = fopen("memout.txt","w");
	regout = fopen("regout.txt","w");
	count = fopen("count.txt","w");
	
	if(memin == NULL || memout == NULL || regout == NULL || trace == NULL || count == NULL)
		return 0;
	int cnt = 0, pc = 0, reg[16] = {0}, max_line_counter = 0;
	char memory_in[4096][4],memory_out[4096][4], line[4];;
	int *counter = &cnt, *pcp = &pc;	
	
	while(!feof(memin) && memin!= NULL)//memory_in and memory_out are filled
	{	
		fscanf(memin,"%s",memory_in[line_counter]);
		memory_out[line_counter]=memory_in[line_counter];
		max_line_counter++;
	}	
	fclose(memin);
	
	while(*pcp > -1)
	{			
		line = memory_in[*pcp];
		createTrace(pc,line,reg,count);
		decipher_line(line,reg,memory_in,memory_out,pcp,max_line_counter);	
	}
	//Exit:	
	fclose(trace);
	createRegOut(regout,reg);
	fclose(regout);
	memout();
	
	
}
  


