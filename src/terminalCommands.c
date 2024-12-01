#include "inc/terminalCommands.h"
#include "inc/terminal.h"
#include "inc/utils.h"
#include "inc/kern.h"
#include "inc/gdt.h"
#include "inc/kmalloc.h"
#include "inc/command.h"

bool change_color(uint8_t *color);
bool IsCommandOnlySpaces(uint8_t *Command);
char *GetArgumentInCommand(char *afterCommand);

void parse_command(uint8_t *Command)
{
	if (IsCommandOnlySpaces(Command))
	{
		return;
	}
	tCommand *CommandList = gCommandList;
	while (CommandList != NULL)
	{
		if (memcmp(Command, CommandList->name, strlen(CommandList->name)) == 0)
		{
			CommandList->function(Command);
			return;
		}
		CommandList = CommandList->next;
	}
	write(1, "Invalid command : ", 18);
	write(1, Command, strlen(Command));
	write(1, "\n", 1);
}

bool IsCommandOnlySpaces(uint8_t* Command)
{
	for (int i = 0; Command[i] != 0 && i < VGAWIDTH; i++)
	{
		if (Command[i] != ' ' && Command[i] != '\n')
		{
			return false;
		}
	}
	return true;
}

char *GetArgumentInCommand(char *afterCommand)
{
	int i = 0;
	while (afterCommand[i] == ' ' && afterCommand[i] != '\0') {
		i++;
	}

	if (afterCommand[i] == '\0') {
		return NULL;
	}

	return &afterCommand[i];
}

void ClearCommand(char *argument)
{
	(void)argument;
	termReset();
}

void ColorCommand(char *argument)
{
	if (change_color(GetArgumentInCommand(argument + 5))) {
		for (int y = 0; y < VGAHEIGHT * 2; y++) {
			for (int x = 0; x < VGAWIDTH; x++) {
				ACTIVE_TERMINAL.Lines[y][x] = vga_entry(ACTIVE_TERMINAL.Lines[y][x], ACTIVE_TERMINAL.ColorSet);
			}
		}
		flushTermOld();
		refreshLastRow();
	}
}

void HelpCommand(char *argument)
{
	(void)argument;
	uint16_t oldColor = ACTIVE_TERMINAL.ColorSet;
	ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
	tCommand *CommandList = gCommandList;

	while (CommandList != NULL) {
		write(1, CommandList->name, strlen(CommandList->name));
		write(1, " ", 1);
		write(1, CommandList->description, strlen(CommandList->description));
		write(1, "\n", 1);
		CommandList = CommandList->next;
	}
	ACTIVE_TERMINAL.ColorSet = oldColor;
	refreshLastRow();
}

void ReadStackCommand(char *argument)
{
	char *secondArgument = GetArgumentInCommand(argument + 9);
	if (secondArgument == NULL) {
		write(1, "Invalid address\n", 16);
		return;
	}
	if (memcmp(secondArgument, "all", 3) == 0) {
		printAllStackAddress();
	}
	else {
		if (isNumeric(secondArgument) == false) {
			write(1, "Invalid address\n", 16);
			return;
		}
		printSpesificStackAddress(atoi(secondArgument));
	}
}

void WriteStackCommand(char *argument)
{
	char secondArgument[32] = {0};
	memcpy(secondArgument, GetArgumentInCommand(argument + 10), strlen(GetArgumentInCommand(argument + 10)));

	if (secondArgument == NULL) {
		write(1, "Invalid address\n", 16);
		return;
	}
	int thirdArgIndex = 0;
	while (secondArgument[thirdArgIndex] != ' ' && secondArgument[thirdArgIndex] != '\0') {
		thirdArgIndex++;
	}
	char *thirdArgument = GetArgumentInCommand(secondArgument + thirdArgIndex);
	if (thirdArgument == NULL) {
		write(1, "Invalid value\n", 14);
		return;
	}
	if (isNumeric(secondArgument) == false || isNumeric(thirdArgument) == false) {
		write(1, "Invalid address or value\n", 25);
		return;
	}

	write_to_stack(atoi(secondArgument), atoi(thirdArgument));
}

void RebootCommand(char *argument)
{
	uint8_t good = 0x02;
	while (good & 0x02)
		good = port_in(0x64);
	port_out(0x64, 0xFE);
	asm volatile ("hlt");
}

void HaltCommand(char *argument)
{
	(void)argument;
	write(1, "Halting system...\n", 18);
	asm volatile ("hlt");
}

bool change_color(uint8_t* color)
{
	if (color == NULL)
	{
		write(1, "Where is color ?\n", 17);
		return false;
	}
	
	if (memcmp(color, "black", 5) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "blue", 4) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "green", 5) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "cyan", 4) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "red", 3) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "magenta", 7) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_MAGENTA, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "brown", 5) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_BROWN, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "lgrey", 5) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "dgrey", 5) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "lblue", 5) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "lgreen", 6) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "lcyan", 5) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "lred", 4) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "lmagenta", 8) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "lbrown", 6) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
	}
	else if (memcmp(color, "white", 5) == 0)
	{
		ACTIVE_TERMINAL.ColorSet = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	}
	else
	{
		write(1, "Invalid color\n", 14);
		return false;
	}
	return true;
}