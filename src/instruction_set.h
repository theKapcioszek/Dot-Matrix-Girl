#pragma once

typedef struct {
  uint16_t pc;
  uint16_t sp;
  uint8_t A;
  uint8_t B;
  uint8_t C;
  uint8_t D;
  uint8_t E;
  uint8_t F;
  uint8_t H;
  uint8_t L;
} CpuInternals;

#define x00_nop nop(pc)
#define x01_ld_BC_d16 ld_d16_to_reg16(pc,&cpu->B,&cpu->C,memory)
#define x02_ld_addrBC_A ld_addr_reg8(pc,&cpu->A,&cpu->B,&cpu->C,memory)
#define x03_inc_BC inc_reg16(pc,&cpu->B,&cpu->C)
#define x04_inc_B inc_reg8(pc,&cpu->B,&cpu->F)
#define x05_dec_B dec_reg8(pc,&cpu->B,&cpu->F)
#define x06_ld_B_d8 ld_reg8_d8(pc,&cpu->B,memory)
#define x07_rotate_lcA rotate_left_reg8(pc,&cpu->A,&cpu->F)
#define x08_ld_addrd16_sp ld_addrd16_sp(pc,&cpu->sp,memory)

void decide_instruction(uint8_t *memory, CpuInternals *cpu);

uint16_t combine_into_addr(uint8_t high, uint8_t low);

void nop(uint16_t *pc);
void ld_d16_to_reg16(uint16_t *pc, uint8_t *B, uint8_t *C, uint8_t *memory);
void ld_addr_reg8(uint16_t *pc, uint8_t *A, uint8_t *B, uint8_t *C, uint8_t *memory);
void inc_reg16(uint16_t *pc, uint8_t *regH, uint8_t *regL);
void inc_reg8(uint16_t *pc, uint8_t *reg8, uint8_t *flag);
void dec_reg8(uint16_t *pc, uint8_t *reg8, uint8_t *flag);
void ld_reg8_d8(uint16_t *pc, uint8_t *reg8, uint8_t *memory);
void rotate_left_reg8(uint16_t *pc, uint8_t *reg8, uint8_t *flag);
void ld_addrd16_sp(uint16_t *pc, uint16_t *sp, uint8_t *memory);

#if defined(INSTRUCTION_SET_IMPLEMENTATION)

void decide_instruction(uint8_t *memory, CpuInternals *cpu){

  uint16_t *pc = &cpu->pc;

  switch(memory[*pc]){
    case 0x00:
      x00_nop;
      break;
    case 0x01:
      x01_ld_BC_d16;
      break;
    case 0x02:
      x02_ld_addrBC_A;
      break;
    case 0x03:
      x03_inc_BC;
      break;
    case 0x04:
      x04_inc_B;
      break;
    case 0x05:
      x05_dec_B;
      break;
    case 0x06:
      x06_ld_B_d8;
      break;
    case 0x07:
      x07_rotate_lcA;
      break;
    case 0x08:
      x08_ld_addrd16_sp;
      break;
    default:
      break;
  }

}

uint16_t combine_into_addr(uint8_t high, uint8_t low){

  return ((uint16_t)high << 8) | low; //combining values from high and low bytes into a 16bit address
  
}

void nop(uint16_t *pc){ ++*pc; }
void ld_d16_to_reg16(uint16_t *pc, uint8_t *regH, uint8_t *regL, uint8_t *memory){

  *regL = memory[*pc+1];
  *regH = memory[*pc+2];

  *pc += 3;

}
void ld_addr_reg8(uint16_t *pc, uint8_t *reg8, uint8_t *regH, uint8_t *regL, uint8_t *memory){

  uint16_t addr = combine_into_addr(*regH, *regL); //combining values from high and low registers into a 16bit address
  memory[addr] = *reg8;

  ++*pc;

}
void inc_reg16(uint16_t *pc, uint8_t *regH, uint8_t *regL){

  uint16_t value = ((uint16_t)*regH << 8) | *regL; //combining registers into a single value then incrementing it and lastly splitting it again and saving into registers
  ++value;
  *regL = value >> 8;
  *regH = value & 0xff;

  ++*pc;

}
void inc_reg8(uint16_t *pc, uint8_t *reg8, uint8_t *flag){

  int value = *reg8;++value;
  if (value == 0) *flag = *flag | (1 << 7);   //set Z flag
  *flag = *flag | (0 << 6);                   //set N flag
  if (value > 0xff) *flag = *flag | (1 << 5); //set H flag
  ++*reg8;

  ++*pc;
}
void dec_reg8(uint16_t *pc, uint8_t *reg8, uint8_t *flag){
  
  int value = *reg8;--value;
  if (value == 0) *flag = *flag | (1 << 7);   //set Z flag
  *flag = *flag | (1 << 6);                   //set N flag
  if (value < 0) *flag = *flag | (1 << 5);    //set H flag
  --*reg8;

  ++*pc;
}
void ld_reg8_d8(uint16_t *pc, uint8_t *reg8, uint8_t *memory){

  *reg8 = memory[*pc+1];

  *pc += 2;

}
void rotate_left_reg8(uint16_t *pc, uint8_t *reg8, uint8_t *flag){

  *flag = (*reg8 & 0b10000000) == 0 ? *flag | (0 << 4) : *flag | (1 << 4);
  *reg8 = *reg8 << 1;
  ++*pc;

}
void ld_addrd16_sp(uint16_t *pc, uint16_t *sp, uint8_t *memory){

  uint16_t addr = combine_into_addr(memory[*pc+2], memory[*pc+1]);
  memory[addr] = *sp & 0xff;
  memory[addr+1] = *sp >> 8;

  *pc += 3;

}


#endif
