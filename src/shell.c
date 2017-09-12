#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <base.h>
#include <sys.h>
#include <memctrl.h>
#include <shell.h>

#include <mos6502/cpu.h>


static int
cmd_help (mos6502_t * cpu, char * cmd)
{
	printf("List of commands:\n\n");
	printf("s or stepi -- Step one instruction\n");
	printf("sn or stepn -- Step N instructions\n");
	printf("j or jump <addr> -- Set PC to specified adddress (in hex) and continue\n");
	printf("r or regs -- Dump the machine registers\n");
	printf("pk or peek <addr> -- Print the contents of memory at this address\n");
	printf("po or poke <addr> <val> -- Set the contents of memory to this value at this address\n");
	printf("dm or dumpmem <addr> <n> -- Dump N bytes of memory at specified address\n");
	printf("irq -- Manually raise an IRQ\n");
	printf("nmi -- Manually raise an NMI\n");
	printf("pr -- Print the current instruction\n");
	printf("q, quit, or exit -- Quit the program\n");
	return 0;
	
}

static int
cmd_step(mos6502_t * cpu, char * cmd)
{
	return mos6502_step(cpu);
}

static int
cmd_stepn (mos6502_t * cpu, char * cmd)
{
	int n = 0;
	int i = 0;

	if (sscanf(cmd, "stepn %d", &n) == 1 ||
	    sscanf(cmd, "sn %d", &n) == 1) {
		
		for (i = 0; i < n; i++) {
			mos6502_step(cpu);
		}

	} else {
		printf("Invalid command format.\n");
		cmd_help(cpu, cmd);
	}
		
	return 0;
}

static int
cmd_jump (mos6502_t * cpu, char * cmd)
{
	uint16_t addr = 0;

	if (sscanf(cmd, "j %hx\n", &addr) == 1 ||
	    sscanf(cmd, "jump %hx\n", &addr) == 1) {
		cpu->pc = addr;
	} else {
		printf("Invalid command format.\n");
		cmd_help(cpu, cmd);
	}
	return 0;
}

static int
cmd_regs (mos6502_t * cpu, char * cmd)
{
	mos6502_dumpregs(cpu);
	return 0;
}

static int
cmd_peek (mos6502_t * cpu, char * cmd)
{
	uint16_t addr = 0;

	if (sscanf(cmd, "pk %hx", &addr) == 1 ||
	    sscanf(cmd, "peek %hx", &addr) == 1) {
		printf("%04X: %02X\n", addr, mem_read(cpu->sys, addr % MEM_END));
	} else {
		printf("Invalid command format.\n");
		cmd_help(cpu, cmd);
	}

	return 0;
}

static int
cmd_poke (mos6502_t * cpu, char * cmd)
{
	uint16_t addr = 0;
	uint8_t val = 0;

	if (sscanf(cmd, "po %hx %hhx", &addr, &val) == 2 ||
	    sscanf(cmd, "poke %hx %hhx", &addr, &val) == 2) {
		mem_write(cpu->sys, addr, val);
	} else {
		printf("Invalid command format.\n");
		cmd_help(cpu, cmd);
	}
	
	return 0;
}

static int
cmd_dumpmem (mos6502_t * cpu, char * cmd)
{
	uint16_t addr;
	uint16_t n;
	int i = 0;

	if (sscanf(cmd, "dm %hx %hd", &addr, &n) == 2 ||
	    sscanf(cmd, "dumpmem %hx %hd", &addr, &n) == 2) {

		if (n < 4) { 
			n = 4;
		}
	
		for (i = 0; i < n; i+=4, addr+=4) {
			printf("%04X: %02X %02X %02X %02X\n", 
				addr, 
				mem_read(cpu->sys, addr % MEM_END),
				mem_read(cpu->sys, (addr+1)% MEM_END),
				mem_read(cpu->sys, (addr+2)% MEM_END),
				mem_read(cpu->sys, (addr+3)% MEM_END));
		}
			
	} else {
		printf("Invalid command format.\n");
		cmd_help(cpu, cmd);
	}
	
	return 0;
}

static int
cmd_irq (mos6502_t * cpu, char * cmd)
{
	mos6502_raise_irq(cpu);
	return 0;
}

static int
cmd_nmi (mos6502_t * cpu, char * cmd)
{
	mos6502_raise_nmi(cpu);
	return 0;
}

static int
cmd_print_instr (mos6502_t * cpu, char * cmd)
{
	mos6502_print_instr(cpu);
	return 0;
}

static int
cmd_quit (mos6502_t * cpu, char * cmd)
{
	printf("Quitting. Goodbye.\n");
	exit(0);
	return 0;
}


#define CMP(x, n) (strncmp(cmd, x, n) == 0)

#define ONECHAR(x) (cmd[0] == x && cmd[1] == '\n')

static int
handle_cmd (mos6502_t * cpu, char * cmd)
{
	if (CMP("help", 4) || CMP("?", 1) || ONECHAR('h')) {
		return cmd_help(cpu, cmd);
	} else if (ONECHAR('s') || CMP("stepi", 5)) {
		return cmd_step(cpu, cmd);
	} else if (CMP("sn", 2) || CMP("stepn", 5)) {
		return cmd_stepn(cpu, cmd);
	} else if (CMP("j ", 2) || CMP("jump", 4)) {
		return cmd_jump(cpu, cmd);
	} else if (ONECHAR('r') || CMP("regs", 4)) {
		return cmd_regs(cpu, cmd);
	} else if (CMP("pk", 2) || CMP("peek", 4)) {
		return cmd_peek(cpu, cmd);
	} else if (CMP("po", 2) || CMP("poke", 4)) {
		return cmd_poke(cpu, cmd);
	} else if (CMP("dm", 2) || CMP("dumpmem", 7)) {
		return cmd_dumpmem(cpu, cmd);
	} else if (ONECHAR('q') || CMP("quit", 4) || CMP("exit", 4)) {
		return cmd_quit(cpu, cmd);
	} else if (CMP("irq", 3)) {
		return cmd_irq(cpu, cmd);
	} else if (CMP("nmi", 3)) {
		return cmd_nmi(cpu, cmd);
	} else if (CMP("pr", 2)) {
		return cmd_print_instr(cpu, cmd);
	} else {
		printf("Undefined command: \"%s\". Try \"help\".\n", strtok(cmd, "\n"));
		
	}

	return 0;
}


void
start_shell (mos6502_t * cpu) 
{
	char buf[MAX_CMD_LEN];
	char prev[MAX_CMD_LEN];
	
	memset(buf, 0, MAX_CMD_LEN);
	memset(prev, 0, MAX_CMD_LEN);

	while (1) {
		printf(PROMPT_STR);

		while (fgets(buf, MAX_CMD_LEN, stdin) != NULL) {
			if (buf[0] != '\n') {
				handle_cmd(cpu, buf);
				memcpy(prev, buf, MAX_CMD_LEN);
				memset(buf, 0, MAX_CMD_LEN);
			} else {
				if (prev[0] != 0) {
					handle_cmd(cpu, prev);
				}
			}
			printf(PROMPT_STR);
		}

	}

}


