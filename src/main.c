#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "cwd.h"

#define MEM_SIZE 65535
#define ROM_SIZE 32767
#define CWD_PATH_SIZE 200

typedef unsigned short u16;
typedef char u8;

int main(int argc, char* argv[])
{
  u16 PC = 0;
  u16 SP = 0;
  u8 A = 0;
  u8 B = 0;
  u8 C = 0;
  u8 D = 0;
  u8 E = 0;
  u8 F = 0;
  u8 H = 0;
  u8 L = 0;

  u8 memory[MEM_SIZE] = {0};

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

  return 0;
} 
