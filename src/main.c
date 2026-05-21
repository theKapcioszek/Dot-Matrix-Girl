#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "cwd.h"

#define INSTRUCTION_SET_IMPLEMENTATION
#include "instruction_set.h"

#define MEM_SIZE 0xffff
#define ROM_SIZE 0x7fff
#define CWD_PATH_SIZE 200

#define PC_START 0x0100
#define SP_START 0xfffe

int main(int argc, char* argv[])
{
  CpuInternals cpu = {0};
  cpu.pc = PC_START;
  cpu.sp = SP_START;

  uint8_t memory[MEM_SIZE] = {0};

  if(argc < 2){
    printf("\nNo ROM file provided\nUSAGE: \n");
    printf(argv[0]);
    printf(" <path to ROM file>\n\n");

    return 1;
  }

  char file_path[CWD_PATH_SIZE] = {0};
  {
    char cwd_buffer[CWD_PATH_SIZE] = {0};
    cwd(cwd_buffer,CWD_PATH_SIZE);
    strcpy(file_path,cwd_buffer);
    strcat(file_path,"/");
    strcat(file_path,argv[1]);
  }

  if(access(file_path, F_OK) != 0){
    printf("\nProvided ROM file does not exist!\n\n");

    return 1;
  }

  FILE *rom;
  rom = fopen(file_path,"rb");
  fread(memory,ROM_SIZE,1,rom);
  fclose(rom);
 
  for (int i = 308;i < 323;i++) {
    printf("%c",memory[i]);
  }
  printf("\n");

  while(1){
    decide_instruction(memory,&cpu);
    break;
  }

  return 0;
} 
