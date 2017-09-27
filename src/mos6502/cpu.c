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

int cycles = 0;
typedef decode_info_t* (*opcode_Map) (decode_info_t *);

//BRK
decode_info_t * v6502_00(decode_info_t *d)
{
  d->opcode = 0x00;
  d->page_crossed = 0;
  d->mode = MODE_IMPL;
  cycles = 7;
  return d;
}
/*
decode_info_t * v6502_01(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_02(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_03(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_04(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_05(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_06(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_07(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_08(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_09(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_0a(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_0b(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_0c(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_0d(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_0e(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_0f(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_10(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_11(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_12(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_13(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}


decode_info_t * v6502_14(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_15(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_16(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_17(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_18(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_19(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_1a(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_1b(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_1c(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_1d(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_1e(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_1f(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_20(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_21(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_22(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_23(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_24(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_25(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_26(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_27(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_28(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_29(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_2a(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_2b(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}


decode_info_t * v6502_2c(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_2d(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_2e(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_2f(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_30(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_31(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_32(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_33(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_34(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_35(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_36(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_37(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_38(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_39(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_3a(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_3b(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_3c(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_3d(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_3e(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_3f(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_40(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_41(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_42(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_43(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}

decode_info_t * v6502_44(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_45(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_46(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_47(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_48(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_49(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_4a(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_4b(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_4c(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_4d(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_4e(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_4f(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_50(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_51(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_52(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_53(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_54(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_55(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_56(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_57(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_58(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_59(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_5a(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_5b(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_5c(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_5d(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_5e(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_5f(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_60(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_61(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_62(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_63(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_64(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_65(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_66(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_67(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_68(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_69(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_6a(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_6b(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_6c(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_6d(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_6e(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_6f(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_70(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_71(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_72(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_73(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}


decode_info_t * v6502_74(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_75(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_76(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_77(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_78(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_79(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_7a(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_7b(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_7c(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_7d(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_7e(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_7f(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_80(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_81(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_82(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_83(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_84(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_85(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_86(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_87(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_88(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_89(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_8a(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_8b(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_8c(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_8d(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_8e(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_8f(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_90(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_91(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_92(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_93(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_94(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_95(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_96(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_97(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_98(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_99(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_9a(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_9b(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_9c(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_9d(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_9e(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_9f(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_a0(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_a1(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}

decode_info_t * v6502_a2(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_a3(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_a4(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_a5(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_a6(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_a7(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_a8(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_a9(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_aa(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_ab(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_ac(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_ad(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_ae(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_af(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_b0(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_b1(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_b2(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_b3(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_b4(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_b5(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_b6(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_b7(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_b8(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_b9(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}


decode_info_t * v6502_ba(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_bb(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_bc(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_bd(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_be(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_bf(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_c0(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_c1(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_c2(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_c3(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_c4(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_c5(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_c6(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_c7(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_c8(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_c9(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_ca(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_cb(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_cc(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_cd(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_ce(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_cf(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_d0(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_d1(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_d2(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_d3(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_d4(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_d5(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_d6(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_d7(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_d8(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_d9(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_da(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_db(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_dc(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_dd(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_de(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_df(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_e0(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_e1(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_e2(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_e3(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_e4(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_e5(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_e6(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_e7(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}

decode_info_t * v6502_e8(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_e9(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_ea(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_eb(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_ec(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_ed(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_ee(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_ef(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_f0(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_f1(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_f2(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_f3(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_f4(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_f5(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_f6(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_f7(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_f8(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_f9(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_fa(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_fb(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_fc(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_fd(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_fe(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}
decode_info_t * v6502_ff(decode_info_t *d)
{
  d->opcode =
  d->page_crossed =
  d->instr_len =
  d->mode =
  return d;
}


decode_info_t* v6502_00 (decode_info_t *d)
{
        return d;
}
*/
decode_info_t* NOP (decode_info_t *d)
{
        return d;
}

static const opcode_Map op_table[256]={
v6502_00,/*
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
NOP
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
NOP,*/
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
	cpu->pc = 0xFFFC;
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
		cpu->pc = 0xFFFA;
		cycles = 7;
		
	}
	else if(cpu->intr_status == INTR_NMI)
	{
		cpu->intr_status = INTR_NONE;
		push16(cpu, cpu->pc);
		push(cpu, (cpu->p).val);
		cpu->pc = 0xFFFE;
		cycles = 7;
	}
	else
	{
		uint8_t opCode = readbyte(cpu, cpu->pc);
		DEBUG_PRINT("Processing opcode: %" PRIu8 "\n", opCode);
		decode_info_t *decode_info = (decode_info_t*)malloc(sizeof(decode_info_t));
		op_table[opCode](decode_info);
		
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
