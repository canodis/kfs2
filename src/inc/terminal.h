#ifndef TERMINAL_H
# define TERMINAL_H

#include "types.h"

#define VGAWIDTH 80
#define VGAHEIGHT 25

# define LINES 25
# define COLUMNS_IN_LINE 80
# define TERMINAL_COUNT 8
# define MAX_HEADER_SIZE 16
# define ROW ACTIVE_TERMINAL.PromptIndex / VGAWIDTH
# define COLUMN ACTIVE_TERMINAL.PromptIndex % VGAWIDTH

typedef struct s_Terminal
{
	uint16_t Lines[VGAHEIGHT * 2][VGAWIDTH];
	uint8_t LastCommand[VGAWIDTH + 1];
	uint32_t PromptIndex;
	uint8_t ColorSet;
} t_Terminal;

typedef struct s_Kernel
{
	t_Terminal Terminals[TERMINAL_COUNT];
	uint16_t *TermBuff;
	uint32_t ArrowBuff;
	uint8_t TermNumber;
	uint32_t PromptIndexMax;
	uint8_t Header[MAX_HEADER_SIZE];
} t_Kernel;

extern t_Kernel Kernel;

#define ACTIVE_TERMINAL Kernel.Terminals[Kernel.TermNumber]

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void    update_cursor(int x, int y);
void    termReset(void);
void	flushTermOld();
void    leftRightArrowHandler(unsigned char mode);
void    initTerminal();
void	ArrowHandler(unsigned char mode);
void	write(int fd, unsigned char *str, unsigned int len);
void	refreshLastRow(void);
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
uint16_t vga_entry(uint8_t uc, uint8_t color);
void ChangeHeader(char *header);

#endif