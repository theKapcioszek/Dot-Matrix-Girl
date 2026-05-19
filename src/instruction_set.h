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

void decide_instruction(uint16_t *pc, uint8_t *memory, CpuInternals *cpu);

void nop(uint16_t *pc);
void ld_d16_to_reg16(uint16_t *pc, uint8_t *B, uint8_t *C, uint8_t *memory);
void ld_addr_reg8(uint16_t *pc, uint8_t *A, uint8_t *B, uint8_t *C, uint8_t *memory);
void inc_reg16(uint16_t *pc, uint8_t *regH, uint8_t *regL);
void inc_reg8(uint16_t *pc, uint8_t *reg8, uint8_t *flag);
void dec_reg8(uint16_t *pc, uint8_t *reg8, uint8_t *flag);

#if defined(INSTRUCTION_SET_IMPLEMENTATION)

void decide_instruction(uint16_t *pc, uint8_t *memory, CpuInternals *cpu){

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
    default:
      break;
  }

}

void nop(uint16_t *pc){ ++*pc; }
void ld_d16_to_reg16(uint16_t *pc, uint8_t *regH, uint8_t *regL, uint8_t *memory){

  *regL = memory[*pc+1];
  *regH = memory[*pc+2];

  *pc += 3;

}
void ld_addr_reg8(uint16_t *pc, uint8_t *reg8, uint8_t *regH, uint8_t *regL, uint8_t *memory){

  uint16_t addr = ((uint16_t)*regH << 8) | *regL; //combining values from high and low registers into a 16bit address
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

#endif
