#include "inc/kern.h"
#include "inc/terminal.h"
#include "inc/command.h"


int	kmain()
{
	init_gdt();
	initTerminal();

	init_idt();
	kb_init();

	enable_interrupts();
	InitAllCommands();
	
	while (42)
		;
	return (0);
}