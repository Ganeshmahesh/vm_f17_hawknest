#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <base.h>
#include <inttypes.h>
#include <sys.h>
#include <memctrl.h>
#include <mos6502/cpu.h>
#include <mos6502/vmcall.h>


/* 
 * KCH: HINTS
 * 	Some routines/structs/data you'll probably want to write:
 * 		- routines to set the CPU flags based on a value (esp n and z)
 * 		- maps (arrays) to map opcode to instruction length, instruction name,
 * 		  addressing mode, instruction latency (in cycles), and page crossing
 * 		  latency, and a map from opcode to handler function (for your execute stage)
 * 		- separate functions for your decode() and execute() stages in step().
 *
 * 	When you need to read/write a byte from memory, use mem_read and
 * 	mem_write, defined in include/memctrl.h. Notice that you'll need to
 * 	pass a system pointer to this routine.
 *
 *      Notice that I've given you a few convenience routines to start with.
 *
 * 	Make sure to use the buggy read16 when you're calculating the address
 * 	for an indirect jump. 
 *
 * 	Make sure to set the flags properly when executing instructions that
 * 	affect flags.
 *
 * 	Be careful when adjusting cycle counter when executing branches. They 
 * 	can have variable latency when there is a page crossing.
 *
 *	Use the macro-based print routines I've provided for you instead of, e.g. printf:
 *		INFO_PRINT  - print information to the console
 *		DEBUG_PRINT - print information only when compiled for debug mode (see .config file)
 *		ERROR_PRINT - print an error
 *
 *
 * 	Don't forget that we've added an instruction to the 6502 ISA for paravirtual calls.
 * 	This is why I've called it the 6502v. Details are:
 * 		mnemonic:  VMC (VM Call)
 * 		opcode:    0x80
 * 		length:    2 bytes (2nd byte is an immediate call #)
 * 		addr mode: MODE_IMM (immediate)
 * 		latency:   6 cycles
 * 		pagecross: doesn't matter
 *
 *
 *
 */

#define page_crossed(addr, offset) ((((addr) & 0xFF) + offset) >= 0x100)
int cycles = 0;
char current_instr[4] = { '\0', '\0', '\0', '\0'}; 
typedef decode_info_t* (*opcode_Map) (decode_info_t *);

decode_info_t * v6502_00(decode_info_t *d)
{
  d->opcode = 0x00;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 7;
  current_instr[0] = 'B';
  current_instr[1] = 'R';
  current_instr[2] = 'K';
  current_instr[3] = '\0';
  return d;
}
decode_info_t * v6502_01(decode_info_t *d)
{
  d->opcode = 0x01;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_INDIDX;
  cycles = 6;
  return d;
}
decode_info_t * v6502_02(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_03(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_04(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_05(decode_info_t *d)
{
  d->opcode = 0x05;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  return d;
}
decode_info_t * v6502_06(decode_info_t *d)
{
  d->opcode = 0x06;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 5;
  return d;
}
decode_info_t * v6502_07(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_08(decode_info_t *d)
{
  d->opcode = 0x08;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 3;
  return d;
}
decode_info_t * v6502_09(decode_info_t *d)
{
  d->opcode = 0x09;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IMM;
  cycles = 2;
  return d;
}
decode_info_t * v6502_0a(decode_info_t *d)
{
  d->opcode = 0x0A;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_ACC;
  cycles = 2;
  return d;
}
decode_info_t * v6502_0b(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_0c(decode_info_t *d)
{
 
  return d;
}
decode_info_t * v6502_0d(decode_info_t *d)
{
  d->opcode = 0x0D;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 4;
  return d;
}
decode_info_t * v6502_0e(decode_info_t *d)
{
  d->opcode = 0x0E;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 6;
  return d;
}
decode_info_t * v6502_0f(decode_info_t *d)
{

  return d;
}
//TODO:+1 branch succeeds and +2 new page
decode_info_t * v6502_10(decode_info_t *d)
{
  d->opcode = 0x10;
  d->page_crossed = 2;
  d->instr_len = 2;
  d->mode = MODE_REL;
  cycles = 2;
  current_instr[0] = 'B';
  current_instr[1] = 'P';
  current_instr[2] = 'L';
  current_instr[3] = '\0';
  return d;
}
decode_info_t * v6502_11(decode_info_t *d)
{
  d->opcode = 0x11;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IND;
  cycles = 6;
  return d;
}
decode_info_t * v6502_12(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_13(decode_info_t *d)
{

  return d;
}


decode_info_t * v6502_14(decode_info_t *d)
{
 
  return d;
}
decode_info_t * v6502_15(decode_info_t *d)
{
  d->opcode = 0x15;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_16(decode_info_t *d)
{
  d->opcode = 0x16;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 6;
  return d;
}
decode_info_t * v6502_17(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_18(decode_info_t *d)
{
  d->opcode = 0x18;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
  current_instr[0] = 'C';
  current_instr[1] = 'L';
  current_instr[2] = 'C';
  current_instr[3] = '\0';
  return d;
}
decode_info_t * v6502_19(decode_info_t *d)
{
  d->opcode = 0x19;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSY;
  cycles = 4;
  return d;
}
decode_info_t * v6502_1a(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_1b(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_1c(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_1d(decode_info_t *d)
{
  d->opcode = 0x1D;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_1e(decode_info_t *d)
{
  d->opcode = 0x1E;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 7;
  return d;
}
decode_info_t * v6502_1f(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_20(decode_info_t *d)
{
  d->opcode = 0x20; 
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 6; 
  return d;
}
decode_info_t * v6502_21(decode_info_t *d)
{

  d->opcode = 0x21; 
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_INDIDX;
  cycles = 6; 
  return d;
}
decode_info_t * v6502_22(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_23(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_24(decode_info_t *d)
{
  d->opcode = 0x24;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  current_instr[0] = 'B';
  current_instr[1] = 'I';
  current_instr[2] = 'T';
  current_instr[3] = '\0';
  return d;
}
decode_info_t * v6502_25(decode_info_t *d)
{
  d->opcode = 0x25;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  return d;
}
decode_info_t * v6502_26(decode_info_t *d)
{
  d->opcode = 0x26;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_ZEROP;
  cycles = 5;
  return d;
}
decode_info_t * v6502_27(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_28(decode_info_t *d)
{
  d->opcode = 0x28;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 4;
  return d;
}
decode_info_t * v6502_29(decode_info_t *d)
{
  d->opcode = 0x29;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IMM;
  cycles = 2;
  return d;
}
decode_info_t * v6502_2a(decode_info_t *d)
{
  d->opcode = 0x2A;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_ACC;
  cycles = 2;
  return d;
}
decode_info_t * v6502_2b(decode_info_t *d)
{

  return d;
}


decode_info_t * v6502_2c(decode_info_t *d)
{
  d->opcode = 0x2C;
  d->page_crossed = 0;
  d->instr_len =3;
  d->mode = MODE_ABS;
  cycles = 4;
  current_instr[0] = 'B';
  current_instr[1] = 'I';
  current_instr[2] = 'T';
  current_instr[3] = '\0';
  return d;
}
decode_info_t * v6502_2d(decode_info_t *d)
{
  d->opcode = 0x2D;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 4;
  return d;
}
decode_info_t * v6502_2e(decode_info_t *d)
{
  d->opcode = 0x2E;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 6;
  return d;
}
decode_info_t * v6502_2f(decode_info_t *d)
{

  return d;
}
//TODO:+1 branch succeeds +2 new page
decode_info_t * v6502_30(decode_info_t *d)
{
  d->opcode = 0x30;
  d->page_crossed = 2;
  d->instr_len = 2;
  d->mode = MODE_REL;
  cycles = 2;
  current_instr[0] = 'B';
  current_instr[1] = 'M';
  current_instr[2] = 'I';
  current_instr[3] = '\0';
  return d;
}
decode_info_t * v6502_31(decode_info_t *d)
{
  d->opcode = 0x31;
  d->page_crossed = 1;
  d->instr_len = 2;
  d->mode = MODE_IND;
  cycles = 5;
  return d;
}
decode_info_t * v6502_32(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_33(decode_info_t *d)
{
  
  return d;
}
decode_info_t * v6502_34(decode_info_t *d)
{
 
  return d;
}
decode_info_t * v6502_35(decode_info_t *d)
{
  d->opcode = 0x35;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_36(decode_info_t *d)
{
  d->opcode = 0x36;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 6;
  return d;
}
decode_info_t * v6502_37(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_38(decode_info_t *d)
{
d->opcode = 0x38;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_39(decode_info_t *d)
{
  d->opcode = 0x39;
  d->page_crossed = 1;
  d->instr_len = 3;
  d->mode = MODE_ABSY;
  cycles = 4;
  return d;
}
decode_info_t * v6502_3a(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_3b(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_3c(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_3d(decode_info_t *d)
{
  d->opcode = 0x3D;
  d->page_crossed = 1;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_3e(decode_info_t *d)
{
  d->opcode = 0x3E;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 7;
  return d;
}
decode_info_t * v6502_3f(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_40(decode_info_t *d)
{
  d->opcode = 0x40;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 6;
  return d;
}
decode_info_t * v6502_41(decode_info_t *d)
{
  d->opcode = 0x41;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_INDIDX;
  cycles = 6;
  return d;
}
decode_info_t * v6502_42(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_43(decode_info_t *d)
{

  return d;
}

decode_info_t * v6502_44(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_45(decode_info_t *d)
{
  d->opcode = 0x45;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  return d;
}
decode_info_t * v6502_46(decode_info_t *d)
{
  d->opcode = 0x46;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 5;
  return d;
}
decode_info_t * v6502_47(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_48(decode_info_t *d)
{
  d->opcode = 0x48;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 3;
  return d;
}
decode_info_t * v6502_49(decode_info_t *d)
{
  d->opcode = 0x49;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IMM;
  cycles = 2;
  return d;
}
decode_info_t * v6502_4a(decode_info_t *d)
{
  d->opcode = 0x4A;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_ACC;
  cycles = 2;
  return d;
}
decode_info_t * v6502_4b(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_4c(decode_info_t *d)
{
  d->opcode = 0x4C;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 3;
  return d;
}
decode_info_t * v6502_4d(decode_info_t *d)
{
  d->opcode = 0x4D;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 4;
  return d;
}
decode_info_t * v6502_4e(decode_info_t *d)
{
  d->opcode = 0x4E;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 6;
  return d;
}
decode_info_t * v6502_4f(decode_info_t *d)
{

  return d;
}
//TODO: +1 branch succeeds +2 new page
decode_info_t * v6502_50(decode_info_t *d)
{
  d->opcode = 0x50;
  d->page_crossed = 2;
  d->instr_len = 2;
  d->mode = MODE_REL;
  cycles = 2;
  current_instr[0] = 'B';
  current_instr[1] = 'V';
  current_instr[2] = 'C';
  current_instr[3] = '\0';
  return d;
}
decode_info_t * v6502_51(decode_info_t *d)
{
  d->opcode = 0x51;
  d->page_crossed = 1;
  d->instr_len = 2;
  d->mode = MODE_IND;
  cycles = 5;
  return d;
}
decode_info_t * v6502_52(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_53(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_54(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_55(decode_info_t *d)
{
  d->opcode = 0x55;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_56(decode_info_t *d)
{
  d->opcode = 0x56;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 6;
  return d;
}
decode_info_t * v6502_57(decode_info_t *d)
{
 
  return d;
}
decode_info_t * v6502_58(decode_info_t *d)
{
  d->opcode = 0x58;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_59(decode_info_t *d)
{
  d->opcode = 0x59;
  d->page_crossed = 1;
  d->instr_len = 3;
  d->mode = MODE_ABSY;
  cycles = 4;
  return d;
}
decode_info_t * v6502_5a(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_5b(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_5c(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_5d(decode_info_t *d)
{
  d->opcode = 0x5D;
  d->page_crossed = 1;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_5e(decode_info_t *d)
{
  d->opcode = 0x5E;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 7;
  return d;
}
decode_info_t * v6502_5f(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_60(decode_info_t *d)
{
d->opcode = 0x60;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 6;
  return d;
}
decode_info_t * v6502_61(decode_info_t *d)
{
  d->opcode = 0x61;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_INDIDX;
  cycles = 6;
  return d;
}
decode_info_t * v6502_62(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_63(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_64(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_65(decode_info_t *d)
{
  d->opcode = 0x65;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  return d;
}
decode_info_t * v6502_66(decode_info_t *d)
{
  d->opcode = 0x66;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 5;
  return d;
}
decode_info_t * v6502_67(decode_info_t *d)
{
  d->opcode = 0x65;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  return d;

}
decode_info_t * v6502_68(decode_info_t *d)
{
  d->opcode = 0x68;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 4;
  return d;
}
decode_info_t * v6502_69(decode_info_t *d)
{
  d->opcode = 0x69;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IMM;
  cycles = 2;
  return d;
}
decode_info_t * v6502_6a(decode_info_t *d)
{
  d->opcode = 0x6A;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_ACC;
  cycles = 2;
  return d;
}
decode_info_t * v6502_6b(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_6c(decode_info_t *d)
{
  d->opcode = 0x6C;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_IND;
  cycles = 5;
  return d;
}
decode_info_t * v6502_6d(decode_info_t *d)
{
  d->opcode = 0x6D;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 4;
  return d;
}
decode_info_t * v6502_6e(decode_info_t *d)
{
  d->opcode = 0x6E;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 6;
  return d;
}
decode_info_t * v6502_6f(decode_info_t *d)
{
  return d;
}
//TODO:+1 branch succeeds +2 new page
decode_info_t * v6502_70(decode_info_t *d)
{
  d->opcode = 0x70;
  d->page_crossed = 2;
  d->instr_len = 2;
  d->mode = MODE_REL;
  current_instr[0] = 'B';
  current_instr[0] = 'V';
  current_instr[0] = 'S';
  current_instr[0] = '\0';
  cycles = 2;
  return d;
}
decode_info_t * v6502_71(decode_info_t *d)
{
  d->opcode = 0x71;
  d->page_crossed = 1;
  d->instr_len = 2;
  d->mode = MODE_IND;
  cycles = 5;
  return d;
}
decode_info_t * v6502_72(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_73(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_74(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_75(decode_info_t *d)
{
  d->opcode = 0x75;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_76(decode_info_t *d)
{
  d->opcode = 0x76;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 6;
  return d;
}
decode_info_t * v6502_77(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_78(decode_info_t *d)
{
  d->opcode = 0x78;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_79(decode_info_t *d)
{
  d->opcode = 0x79;
  d->page_crossed = 1;
  d->instr_len = 3;
  d->mode = MODE_ABSY;
  cycles = 3;
  return d;
}
decode_info_t * v6502_7a(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_7b(decode_info_t *d)
{
 return d;
}
decode_info_t * v6502_7c(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_7d(decode_info_t *d)
{
  d->opcode = 0x7D;
  d->page_crossed = 1;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_7e(decode_info_t *d)
{
 d->opcode = 0x7E;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 7;
  return d;
}
decode_info_t * v6502_7f(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_80(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_81(decode_info_t *d)
{
  d->opcode = 0x81;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_INDIDX;
  cycles = 6;
  return d;
}
decode_info_t * v6502_82(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_83(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_84(decode_info_t *d)
{
  d->opcode = 0x84;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  return d;
}
decode_info_t * v6502_85(decode_info_t *d)
{
  d->opcode = 0x85;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  return d;
}
decode_info_t * v6502_86(decode_info_t *d)
{
  d->opcode = 0x86;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  return d;
}
decode_info_t * v6502_87(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_88(decode_info_t *d)
{
  d->opcode = 0x88;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
return d;
}
decode_info_t * v6502_89(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_8a(decode_info_t *d)
{
d->opcode = 0x8A;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_8b(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_8c(decode_info_t *d)
{
d->opcode = 0x8C;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 4;
  return d;
}
decode_info_t * v6502_8d(decode_info_t *d)
{
d->opcode = 0x8D;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 4;
  return d;
}
decode_info_t * v6502_8e(decode_info_t *d)
{
d->opcode = 0x8E;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 4;
  return d;
}
decode_info_t * v6502_8f(decode_info_t *d)
{
return d;
}
//TODO:need to represent page crossed with a value +1 if branch succeeds else +2 if new page
decode_info_t * v6502_90(decode_info_t *d)
{
  d->opcode = 0x90;
  d->page_crossed = 1;
  d->instr_len = 2;
  d->mode = MODE_REL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_91(decode_info_t *d)
{
  d->opcode = 0x91;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IND;
  cycles = 6;
  return d;
}
decode_info_t * v6502_92(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_93(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_94(decode_info_t *d)
{
d->opcode = 0x94;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_95(decode_info_t *d)
{
d->opcode = 0x95;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_96(decode_info_t *d)
{
d->opcode = 0x96;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPY;
  cycles = 4;
  return d;
}
decode_info_t * v6502_97(decode_info_t *d)
{
 return d;
}
decode_info_t * v6502_98(decode_info_t *d)
{
d->opcode = 0x98;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_99(decode_info_t *d)
{
d->opcode = 0x99;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSY;
  cycles = 5;
  return d;
}
decode_info_t * v6502_9a(decode_info_t *d)
{
 d->opcode = 0x9A;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_9b(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_9c(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_9d(decode_info_t *d)
{
d->opcode = 0x9D;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 5;
  return d;
}
decode_info_t * v6502_9e(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_9f(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_a0(decode_info_t *d)
{
  d->opcode = 0xA0;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IMM;
  cycles = 2;
  return d;
}
decode_info_t * v6502_a1(decode_info_t *d)
{
  d->opcode = 0xA1;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_INDIDX;
  cycles = 6;
  return d;
}

decode_info_t * v6502_a2(decode_info_t *d)
{
  d->opcode = 0xA2;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IMM;
  cycles = 2;
  return d;
}
decode_info_t * v6502_a3(decode_info_t *d)
{
  return d;
}
decode_info_t * v6502_a4(decode_info_t *d)
{
  d->opcode = 0xA4;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  return d;
}
decode_info_t * v6502_a5(decode_info_t *d)
{
  d->opcode = 0xA5;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  return d;
}
decode_info_t * v6502_a6(decode_info_t *d)
{
  d->opcode = 0xA6;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  return d;
}
decode_info_t * v6502_a7(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_a8(decode_info_t *d)
{
d->opcode = 0xA8;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_a9(decode_info_t *d)
{
  d->opcode = 0xA9;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IMM;
  cycles = 2;
  return d;
}
decode_info_t * v6502_aa(decode_info_t *d)
{
d->opcode = 0xAA;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_ab(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_ac(decode_info_t *d)
{
  d->opcode = 0xAC;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 4;
  return d;
}
decode_info_t * v6502_ad(decode_info_t *d)
{
  d->opcode = 0xAD;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 4;
  return d;
}
decode_info_t * v6502_ae(decode_info_t *d)
{
  d->opcode = 0xAE;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 4;
  return d;
}
decode_info_t * v6502_af(decode_info_t *d)
{
return d;
}
//TODO: change page crossed +1 for branch succeeds else +2 for new page
decode_info_t * v6502_b0(decode_info_t *d)
{
  d->opcode = 0xB0;
  d->page_crossed = 2;
  d->instr_len = 2;
  d->mode = MODE_REL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_b1(decode_info_t *d)
{
  d->opcode = 0xB1;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IND;
  cycles = 5;
  return d;
}
decode_info_t * v6502_b2(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_b3(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_b4(decode_info_t *d)
{
  d->opcode = 0xB4;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_b5(decode_info_t *d)
{
  d->opcode = 0xB5;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_b6(decode_info_t *d)
{
  d->opcode = 0xB6;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPY;
  cycles = 4;
  return d;
}
decode_info_t * v6502_b7(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_b8(decode_info_t *d)
{
  d->opcode = 0xB8;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_b9(decode_info_t *d)
{
  d->opcode = 0xB9;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSY;
  cycles = 4;
  return d;
}
decode_info_t * v6502_ba(decode_info_t *d)
{
d->opcode = 0xBA;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_bb(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_bc(decode_info_t *d)
{
  d->opcode = 0xBC;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_bd(decode_info_t *d)
{
  d->opcode = 0xBD;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_be(decode_info_t *d)
{
  d->opcode = 0xBE;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSY;
  cycles = 4;
  return d;
}
decode_info_t * v6502_bf(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_c0(decode_info_t *d)
{
  d->opcode = 0xC0;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IMM;
  cycles = 2;
return d;
}
decode_info_t * v6502_c1(decode_info_t *d)
{
  d->opcode = 0xC1;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_INDIDX;
  cycles = 6;
return d;
}
decode_info_t * v6502_c2(decode_info_t *d)
{
 return d;
}
decode_info_t * v6502_c3(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_c4(decode_info_t *d)
{
  d->opcode = 0xC4;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
return d;
}
decode_info_t * v6502_c5(decode_info_t *d)
{
  //CMP
  d->opcode = 0xC5;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  return d;
}
decode_info_t * v6502_c6(decode_info_t *d)
{
  d->opcode = 0xC6;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 5;
return d;
}
decode_info_t * v6502_c7(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_c8(decode_info_t *d)
{
  d->opcode = 0xC8;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
return d;
}
decode_info_t * v6502_c9(decode_info_t *d)
{
  d->opcode = 0xC9;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IMM;
  cycles = 2;
  return d;
}
decode_info_t * v6502_ca(decode_info_t *d)
{
  d->opcode = 0xCA;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
return d;
}
decode_info_t * v6502_cb(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_cc(decode_info_t *d)
{
  d->opcode = 0xCC;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 4;
return d;
}
decode_info_t * v6502_cd(decode_info_t *d)
{
  d->opcode = 0xCD;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 4;
  return d;
}
decode_info_t * v6502_ce(decode_info_t *d)
{
  d->opcode = 0xCE;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 6;
return d;
}
decode_info_t * v6502_cf(decode_info_t *d)
{
return d;
}
//TODO:+1 branch succeds +2 new page
decode_info_t * v6502_d0(decode_info_t *d)
{
  d->opcode = 0xD0;
  d->page_crossed = 2;
  d->instr_len = 2;
  d->mode = MODE_REL;
  cycles = 2;
  current_instr[0] = 'B';
  current_instr[1] = 'N';
  current_instr[2] = 'E';
  current_instr[3] = '\0'; 
  return d;
}
decode_info_t * v6502_d1(decode_info_t *d)
{
  d->opcode = 0xD1;
  d->page_crossed = 1;
  d->instr_len = 2;
  d->mode = MODE_IND;
  cycles = 5;
  return d;
}
decode_info_t * v6502_d2(decode_info_t *d)
{
 return d;
}
decode_info_t * v6502_d3(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_d4(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_d5(decode_info_t *d)
{
  d->opcode = 0xD5;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_d6(decode_info_t *d)
{
  d->opcode = 0xD6;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 6;
return d;
}
decode_info_t * v6502_d7(decode_info_t *d)
{
return d;
}
decode_info_t * v6502_d8(decode_info_t *d)
{
  d->opcode = 0xD8;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
	return d;
}
decode_info_t * v6502_d9(decode_info_t *d)
{
  d->opcode = 0xD9;
  d->page_crossed = 1;
  d->instr_len = 3;
  d->mode = MODE_ABSY;
  cycles = 4;
	return d;
}
decode_info_t * v6502_da(decode_info_t *d)
{
	return d;
}
decode_info_t * v6502_db(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_dc(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_dd(decode_info_t *d)
{
  d->opcode = 0xDD;
  d->page_crossed = 1;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_de(decode_info_t *d)
{
  d->opcode = 0xDE;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 7;
  return d;
}
decode_info_t * v6502_df(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_e0(decode_info_t *d)
{
  d->opcode = 0xE0;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IMM;
  cycles = 2;
  return d;
}
decode_info_t * v6502_e1(decode_info_t *d)
{
  d->opcode = 0xE1;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_INDIDX;
  cycles = 6;
  return d;
}
decode_info_t * v6502_e2(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_e3(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_e4(decode_info_t *d)
{
  d->opcode = 0xE4;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  return d;
}
decode_info_t * v6502_e5(decode_info_t *d)
{
d->opcode = 0xE5;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 3;
  return d;
}
decode_info_t * v6502_e6(decode_info_t *d)
{
  d->opcode = 0xE6;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROP;
  cycles = 5;
  return d;
}
decode_info_t * v6502_e7(decode_info_t *d)
{

  return d;
}

decode_info_t * v6502_e8(decode_info_t *d)
{
  d->opcode = 0xE8;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_e9(decode_info_t *d)
{
d->opcode = 0xE9;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IMM;
  cycles = 2;
  return d;
}
decode_info_t * v6502_ea(decode_info_t *d)
{
  d->opcode = 0xEA;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_eb(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_ec(decode_info_t *d)
{
  d->opcode = 0xEC;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 4;
  return d;
}
decode_info_t * v6502_ed(decode_info_t *d)
{
d->opcode = 0xED;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 4;
  return d;
}
decode_info_t * v6502_ee(decode_info_t *d)
{
  d->opcode = 0xEE;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABS;
  cycles = 6;
  return d;
}
decode_info_t * v6502_ef(decode_info_t *d)
{

  return d;
}
//TODO:+1 branch succeeds else +2 for new page
decode_info_t * v6502_f0(decode_info_t *d)
{
  d->opcode = 0xF0;
  d->page_crossed = 2;
  d->instr_len = 2;
  d->mode = MODE_REL;
  cycles = 2;
  current_instr[0] = 'B';
  current_instr[1] = 'E';
  current_instr[2] = 'Q';
  current_instr[3] = '\0';
  return d;
}
decode_info_t * v6502_f1(decode_info_t *d)
{
d->opcode = 0xF1;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_IND;
  cycles = 5;
  return d;
}
decode_info_t * v6502_f2(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_f3(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_f4(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_f5(decode_info_t *d)
{
d->opcode = 0xF5;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_f6(decode_info_t *d)
{
  d->opcode = 0xF6;
  d->page_crossed = 0;
  d->instr_len = 2;
  d->mode = MODE_ZEROPX;
  cycles = 6;
  return d;
}
decode_info_t * v6502_f7(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_f8(decode_info_t *d)
{
d->opcode = 0xF8;
  d->page_crossed = 0;
  d->instr_len = 1;
  d->mode = MODE_IMPL;
  cycles = 2;
  return d;
}
decode_info_t * v6502_f9(decode_info_t *d)
{
  d->opcode = 0xF9;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSY;
  cycles = 4;
  return d;
}
decode_info_t * v6502_fa(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_fb(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_fc(decode_info_t *d)
{

  return d;
}
decode_info_t * v6502_fd(decode_info_t *d)
{
d->opcode = 0xFD;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 4;
  return d;
}
decode_info_t * v6502_fe(decode_info_t *d)
{
  d->opcode = 0xFE;
  d->page_crossed = 0;
  d->instr_len = 3;
  d->mode = MODE_ABSX;
  cycles = 7;
  return d;
}
decode_info_t * v6502_ff(decode_info_t *d)
{

  return d;
}

decode_info_t* NOP (decode_info_t *d)
{
        return d;
}

static const opcode_Map op_table[256]={
v6502_00,
v6502_01,
NOP,
NOP,
NOP,
v6502_05,
v6502_06,
NOP,
v6502_08,
v6502_09,
v6502_0a,
NOP,
NOP,
v6502_0d,
v6502_0e,
NOP,
v6502_10,
v6502_11,
NOP,
NOP,
NOP,
v6502_15,
v6502_16,
NOP,
v6502_18,
v6502_19,
NOP,
NOP,
NOP,
v6502_1d,
v6502_1e,
NOP,
v6502_20,
v6502_21,
NOP,
NOP,
v6502_24,
v6502_25,
v6502_26,
NOP,
v6502_28,
v6502_29,
v6502_2a,
NOP,
v6502_2c,
v6502_2d,
v6502_2e,
NOP,
v6502_30,
v6502_31,
NOP,
NOP,
NOP,
v6502_35,
v6502_36,
NOP,
v6502_38,
v6502_39,
NOP,
NOP,
NOP,
v6502_3d,
v6502_3e,
NOP,
v6502_40,
v6502_41,
NOP,
NOP,
NOP,
v6502_45,
v6502_46,
NOP,
v6502_48,
v6502_49,
v6502_4a,
NOP,
v6502_4c,
v6502_4d,
v6502_4e,
NOP,
v6502_50,
v6502_51,
NOP,
NOP,
NOP,
v6502_55,
v6502_56,
NOP,
v6502_58,
v6502_59,
NOP,
NOP,
NOP,
v6502_5d,
v6502_5e,
NOP,
v6502_60,
v6502_61,
NOP,
NOP,
NOP,
v6502_65,
v6502_66,
NOP,
v6502_68,
v6502_69,
v6502_6a,
NOP,
v6502_6c,
v6502_6d,
v6502_6e,
NOP,
v6502_70,
v6502_71,
NOP,
NOP,
NOP,
v6502_75,
v6502_76,
NOP,
v6502_78,
v6502_79,
NOP,
NOP,
NOP,
v6502_7d,
v6502_7e,
NOP,
NOP,
v6502_81,
NOP,
NOP,
v6502_84,
v6502_85,
v6502_86,
NOP,
v6502_88,
NOP,
v6502_8a,
NOP,
v6502_8c,
v6502_8d,
v6502_8e,
NOP,
v6502_90,
v6502_91,
NOP,
NOP,
v6502_94,
v6502_95,
v6502_96,
NOP,
v6502_98,
v6502_99,
v6502_9a,
NOP,
NOP,
v6502_9d,
NOP,
NOP,
v6502_a0,
v6502_a1,
v6502_a2,
NOP,
v6502_a4,
v6502_a5,
v6502_a6,
NOP,
v6502_a8,
v6502_a9,
v6502_aa,
NOP,
v6502_ac,
v6502_ad,
v6502_ae,
NOP,
v6502_b0,
v6502_b1,
NOP,
NOP,
v6502_b4,
v6502_b5,
v6502_b6,
NOP,
v6502_b8,
v6502_b9,
v6502_ba,
NOP,
v6502_bc,
v6502_bd,
v6502_be,
NOP,
v6502_c0,
v6502_c1,
NOP,
NOP,
v6502_c4,
v6502_c5,
v6502_c6,
NOP,
v6502_c8,
v6502_c9,
v6502_ca,
NOP,
v6502_cc,
v6502_cd,
v6502_ce,
NOP,
v6502_d0,
v6502_d1,
NOP,
NOP,
NOP,
v6502_d5,
v6502_d6,
NOP,
v6502_d8,
v6502_d9,
NOP,
NOP,
NOP,
v6502_dd,
v6502_de,
NOP,
v6502_e0,
v6502_e1,
NOP,
NOP,
v6502_e4,
v6502_e5,
v6502_e6,
NOP,
v6502_e8,
v6502_e9,
v6502_ea,
NOP,
v6502_ec,
v6502_ed,
v6502_ee,
NOP,
v6502_f0,
v6502_f1,
NOP,
NOP,
NOP,
v6502_f5,
v6502_f6,
NOP,
v6502_f8,
v6502_f9,
NOP,
NOP,
NOP,
v6502_fd,
v6502_fe,
NOP
}; 

uint8_t
readbyte (mos6502_t * cpu, uint16_t addr)
{
	DEBUG_PRINT("Handling memory read at address 0x%04x\n", addr);
	uint8_t val = mem_read(cpu->sys, addr);
	return val;
} 

uint16_t 
read16 (mos6502_t * cpu, uint16_t addr)
{
	DEBUG_PRINT("Handling memory read at address 0x%04x\n", addr);
	uint16_t lo = (uint16_t)mem_read(cpu->sys, addr);
	uint16_t hi = (uint16_t)mem_read(cpu->sys, addr+1);
	return lo | (hi << 8);
}

// emulates a wraparound bug: low byte wraps, high byte 
// not incremented (occurs with indirect addressing, only on JMP)
uint16_t 
buggy_read16 (mos6502_t * cpu, uint16_t addr)
{
	uint16_t first = addr;
	uint16_t secnd = (addr & 0xFF00) | (uint16_t)((uint8_t)addr + 1);
	uint16_t lo = (uint16_t)mem_read(cpu->sys, first);
	uint16_t hi = (uint16_t)mem_read(cpu->sys, secnd);
	return (hi << 8) | lo;
}

void
push (mos6502_t * cpu, uint8_t val)
{
	mem_write(cpu->sys, 
		  (0x100 | cpu->sp),
		  val);
	cpu->sp--;
}

// KCH: really, this is pop
uint8_t
pull (mos6502_t * cpu)
{
	cpu->sp++;
	return mem_read(cpu->sys, (0x100 | cpu->sp));
}

static void
push16 (mos6502_t * cpu, uint16_t val)
{
	uint8_t hi = (uint8_t)(val >> 8);
	uint8_t lo = (uint8_t)(val & 0xff);
	push(cpu, hi);
	push(cpu, lo);
}

static uint16_t
pull16 (mos6502_t * cpu)
{
	uint16_t lo = (uint16_t)pull(cpu);
	uint16_t hi = (uint16_t)pull(cpu);
	return (hi << 8) | lo;
}

uint8_t
resolve_addr(mos6502_t *cpu, decode_info_t *d_info)
{
	uint16_t mem_low = 0;
	uint16_t mem_high = 0;
	uint8_t value = 0;
	uint8_t pc_update_value = 0;
	uint16_t mem = 0;

	switch(d_info->mode)
	{

        	case MODE_ABS:
			mem = read16(cpu, cpu->pc); //fetching the address
			value = readbyte(cpu, mem); //fetching value at address
			pc_update_value = 16;
			break;

		case MODE_ABSX:
			mem = read16(cpu, cpu->pc);
			pc_update_value = 16;
			mem += cpu->x;
			value = readbyte(cpu, mem);
			break;

		case MODE_ABSY:
			mem = read16(cpu, cpu->pc);
			pc_update_value = 16;
			mem += cpu->y;
			value = readbyte(cpu, mem);
			break;

		case MODE_ACC:
			value = cpu->a;
			break;

		case MODE_IMM:
			value = readbyte(cpu, cpu->pc);
			pc_update_value = 8;
			break;

		case MODE_IMPL:
			break;

		case MODE_IDXIND:
			mem_low = readbyte(cpu, cpu->pc);
			pc_update_value = 8;
			mem_high = 0;
			mem = mem_low | (mem_high << 8);
			mem += cpu->x;
			value = readbyte(cpu, mem);
			break;

		case MODE_IND:
			//JMP execution code so skipping for now
			/*
			mem = read16(cpu, cpu->pc);
			pc_update_value = 16;
			cpu->pc = read16(cpu, mem);
			*/
			break;

		case MODE_INDIDX:
			mem_low = readbyte(cpu, cpu->pc);
			mem_high = 0;
			pc_update_value = 8;
			mem = mem_low | (mem_high << 8);
			value = read16(cpu, mem);
			value += cpu->y;
			break;

		case MODE_REL:
			value = readbyte(cpu,cpu->pc);
			pc_update_value = 8;
			break;

		case MODE_ZEROP:
			mem_low = readbyte(cpu, cpu->pc);
			pc_update_value = 8;
			mem_high = 0;
			mem = mem_low | (mem_high << 8);
			value = readbyte(cpu, mem);
			break;

		case MODE_ZEROPX:
			mem_low = readbyte(cpu, cpu->pc);
			pc_update_value = 8;
			mem_high = 0;
			mem = mem_low | (mem_high << 8);
			mem += cpu->x;
			value = readbyte(cpu, mem);
			break;
			
		case MODE_ZEROPY:
			mem_low = readbyte(cpu, cpu->pc);
			pc_update_value = 8;
			mem_high = 0;
			mem = mem_low | (mem_high << 8);
			mem += cpu->y;
			value = readbyte(cpu, mem);
			break;
		case MODE_NONE:
		break;

	}
	cpu->pc += pc_update_value;
	return value;
}



/* 
 * KCH: Here the CPU is created and it is set to its reset state (by calling your 
 * mos6502_reset() routine). I've only allocated the structure for you to 
 * avoid segmentation faults in other parts of the code.
 *
 */
mos6502_t *
mos6502_init (system_t * sys)
{
	mos6502_t * cpu = (mos6502_t*)malloc(sizeof(mos6502_t));
	if (!cpu) {
		ERROR_PRINT("Could not allocate CPU\n");
		return NULL;
	}
	memset(cpu, 0, sizeof(mos6502_t));
	INFO_PRINT("Initializing CPU...\n");
	cpu->sys = sys;
	mos6502_reset(cpu);
	
	return cpu;
}


/* 
 * KCH: put the CPU in its reset state. You can find
 * an example of what this should mean at the following URL:
 *     http://nesdev.com/NESDoc.pdf ; pp. 13
 *
 */
void
mos6502_reset (mos6502_t * cpu)
{
	INFO_PRINT("Resetting CPU...\n");
	cpu->pc = read16(cpu, 0xFFFC);
	(cpu->p).val = 0;
	cpu->intr_status = 0;
}


/*
 * KCH: fetch, decode, and execute one instruction. This routine
 * will be called by the system code to move the CPU forward. The
 * rough order of things should be:
 * 	- check for interrupts, if there are any, handle them appropriately
 * 	- reset the interrupt state
 * 	- fetch an instruction opcode
 * 	- decode the instruction (filling in a decode_info_t struct appropriately)
 * 	- increment PC based on the above
 * 	- execute the instruction, passing the decoder info to the handler
 * 	- increase the cycle counter by the instruction latency
 * 	- make sure to increase cycle counter further if there was a page crossing and
 * 	  page crossings matter for this instruction
 * 	- RETURN the total number of cycles it took to execute the instruction
 */
int
mos6502_step (mos6502_t * cpu)
{
	DEBUG_PRINT("Inside step routine\n");
	//Handle interrupt
	if(cpu->intr_status == INTR_IRQ)
	{
		//reset interrupt status
		cpu->intr_status = INTR_NONE;
		push16(cpu, cpu->pc);
		push(cpu, (cpu->p).val);
		//handle interrupt
		cpu->pc = read16(cpu, 0xFFFA);
		cycles = 7;
		
	}
	else if(cpu->intr_status == INTR_NMI)
	{
		cpu->intr_status = INTR_NONE;
		push16(cpu, cpu->pc);
		push(cpu, (cpu->p).val);
		cpu->pc = read16(cpu, 0xFFFE);
		cycles = 7;
	}
	else
	{
		uint8_t opCode = readbyte(cpu, cpu->pc);
		cpu->pc += 8;
		DEBUG_PRINT("Processing opcode: %" PRIu8 "\n", opCode);
		decode_info_t *decode_info = (decode_info_t*)malloc(sizeof(decode_info_t));
		op_table[opCode](decode_info);
		resolve_addr(cpu, decode_info);
		
		//Now execute the instruction
		
	}
	return 0;
}



/*
 *
 * KCH: Raise an IRQ. Set the interrupt status correctly to reflect this. Your
 * step routine will check the status and act on it.  This routine will be
 * called by external devices (viz. the controller when a button is presserd)
 *
 */
void 
mos6502_raise_irq (mos6502_t * cpu)
{
	// FILL ME IN
}

/*
 * KCH: Raise an NMI. Set the interrupt status correctly to reflect this.
 *
 */
void
mos6502_raise_nmi (mos6502_t * cpu)
{
	// FILL ME IN
}


/*
 * KCH: this routine should print out the registers in the machine,
 * including:
 * 	- A, X, and Y
 * 	- PS (processor status reg)
 * 	- SP (stack pointer)
 * 	- PC
 *
 * Feel free to either print out the raw PS value or to actually parse
 * each flag. It is up to you.
 *
 */
void
mos6502_dumpregs (mos6502_t * cpu)
{
	// FILL ME IN
}

/* 
 * KCH: this function should print out details about the current instruction,
 * 	including:
 * 	- the current PC
 * 	- each byte of the instruction (including the opcode)
 * 	- the instruction mnemonic (e.g. STX, TSX, JMP, RTI, etc.)
 * 	- the addressing mode
 * 	- the latency of the instruction
 */
void
mos6502_print_instr (mos6502_t * cpu)
{
	// FILL ME IN
}
