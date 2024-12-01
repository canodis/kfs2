#include "inc/terminal.h"
#include "inc/utils.h"
#include "inc/kern.h"

t_Kernel Kernel;

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

uint16_t vga_entry(uint8_t uc, uint8_t color)
{
	return (uint16_t)uc | (uint16_t)color << 8;
}

void update_cursor(int32_t x, int32_t y)
{
	uint16_t pos = y * VGAWIDTH + x;

	port_out(0x3D4, 0x0F);							// burda birazdan row degeri yolliyacagimizi belirtiyoruz
	port_out(0x3D5, (uint8_t)(pos & 0xFF));			// son 8 bit
	port_out(0x3D4, 0x0E);							// burdada birazdan column degerini yolliyacagimizi belirtiyoruz
	port_out(0x3D5, (uint8_t)((pos >> 8) & 0xFF));	// ilk 8 bit
}

void initTerminal()
{
	memset(&Kernel, 0, sizeof(t_Kernel));
	for (int i = 0; i < TERMINAL_COUNT; i++) {
		Kernel.Terminals[i].ColorSet = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	}
	ChangeHeader("KnowHow");
	Kernel.PromptIndexMax = (VGAHEIGHT * 2) * VGAWIDTH;
	Kernel.TermBuff = (uint16_t *)0xB8000;
	termReset();
}

// 16bit
//  ilk 8 >> ilk 4 >> bg color >> ikinci 4 fg color
//  sonraki 8 bit asci char
void termReset(void)
{
	uint32_t width;
	uint32_t height;

	Kernel.TermBuff = (uint16_t *)0xB8000;

	height = 0;
	while (height < VGAHEIGHT)
	{
		width = 0;
		while (width < VGAWIDTH)
		{
			Kernel.TermBuff[(80 * height) + width] = vga_entry(' ', ACTIVE_TERMINAL.ColorSet);
			width++;
		}
		height++;
	}
	memset(ACTIVE_TERMINAL.Lines, 0, sizeof(ACTIVE_TERMINAL.Lines));
	refreshLastRow();
}

void flushTermOld()
{
	uint32_t counter;
	uint32_t counter2;

	if (ROW < VGAHEIGHT) {
		counter = -1;
		while (++counter < (VGAHEIGHT - (ROW + 1)) * VGAWIDTH) {
			Kernel.TermBuff[counter] = ACTIVE_TERMINAL.Lines[(50 - (VGAHEIGHT - ROW)) + (counter / VGAWIDTH)][counter % VGAWIDTH];
		}
		counter2 = -1;
		while (++counter2 < ROW * VGAWIDTH) {
			Kernel.TermBuff[counter + counter2] = ACTIVE_TERMINAL.Lines[counter2 / VGAWIDTH][counter2 % VGAWIDTH];
		}
	}
	else {
		counter = -1;
		while (++counter < ((VGAHEIGHT - 1) * VGAWIDTH)) {
			Kernel.TermBuff[counter] = ACTIVE_TERMINAL.Lines[(ROW - 24) + (counter / VGAWIDTH)][counter % VGAWIDTH];
		}
	}
}

void leftRightArrowHandler(uint8_t mode)
{
	if (mode == 0) // mode 0 = leftArrow
	{
		if (Kernel.TermNumber > 0) {
			Kernel.TermNumber--;
		}
	}
	else if (mode == 1) // mode 1 rightArrow
	{
		if (Kernel.TermNumber < 7) {
			Kernel.TermNumber++;
		}
	}
	ChangeHeader("KnowHow");
	flushTermOld();
	refreshLastRow();
}

void ArrowHandler(uint8_t mode)
{
	uint32_t counter;
	uint32_t counter2;
	uint32_t supRow;

	if (mode == 1) // up arrow
	{
		if (ROW <= Kernel.ArrowBuff) {
			return;
		}
		Kernel.ArrowBuff++;
	}
	else if (mode == 0) // down arrow
	{
		if (Kernel.ArrowBuff < 1) {
			return;
		}
		Kernel.ArrowBuff--;
	}

	supRow = ROW - Kernel.ArrowBuff;

	if (supRow < VGAHEIGHT)
	{
		counter = -1;
		while (++counter < (VGAHEIGHT - (supRow + 1)) * VGAWIDTH) {
			Kernel.TermBuff[counter] = ACTIVE_TERMINAL.Lines[((VGAHEIGHT * 2) - (VGAHEIGHT - supRow)) + (counter / VGAWIDTH)][counter % VGAWIDTH];
		}
		counter2 = -1;
		while (++counter2 < supRow * VGAWIDTH)
		{
			Kernel.TermBuff[counter + counter2] = ACTIVE_TERMINAL.Lines[counter2 / VGAWIDTH][counter2 % VGAWIDTH];
		}
	}
	else
	{
		counter = -1;
		while (++counter < ((VGAHEIGHT - 1) * VGAWIDTH)) {
			Kernel.TermBuff[counter] = ACTIVE_TERMINAL.Lines[(supRow - 24) + (counter / VGAWIDTH)][counter % VGAWIDTH];
		}
	}
}

// fd = 1 str = deneme len = 6
void write(int32_t fd, uint8_t *str, uint32_t len)
{
	uint32_t counter;
	uint32_t tmp;

	(void)fd;

	counter = -1;
	while (++counter < len)
	{
		if (*(str + counter) == '\n') {
			memset(ACTIVE_TERMINAL.LastCommand, 0, sizeof(ACTIVE_TERMINAL.LastCommand));
			for (uint32_t i = 0; i < COLUMN; i++) {
				ACTIVE_TERMINAL.LastCommand[i] = (uint8_t)(ACTIVE_TERMINAL.Lines[ROW][i] & 0xFF);
			}
			ACTIVE_TERMINAL.LastCommand[COLUMN] = '\0';
			tmp = ACTIVE_TERMINAL.PromptIndex + (VGAWIDTH - (COLUMN));
			while (ACTIVE_TERMINAL.PromptIndex < tmp) {
				ACTIVE_TERMINAL.Lines[ROW][COLUMN] = vga_entry(' ', ACTIVE_TERMINAL.ColorSet);
				ACTIVE_TERMINAL.PromptIndex++;
			}
			if (ACTIVE_TERMINAL.PromptIndex >= Kernel.PromptIndexMax)
			{
				ACTIVE_TERMINAL.PromptIndex = 0;
			}
			memset(ACTIVE_TERMINAL.Lines[ROW], 0, sizeof(ACTIVE_TERMINAL.Lines[ROW]));
			flushTermOld();
		}
		else if (*(str + counter) == '\b') {
			if (COLUMN > 0) {
				ACTIVE_TERMINAL.PromptIndex--;
				ACTIVE_TERMINAL.Lines[ROW][COLUMN] = vga_entry(' ', ACTIVE_TERMINAL.ColorSet);
			}
		}
		else {
			ACTIVE_TERMINAL.Lines[ROW][COLUMN] = vga_entry(*(str + counter), ACTIVE_TERMINAL.ColorSet);
			ACTIVE_TERMINAL.PromptIndex++;
			if (ACTIVE_TERMINAL.PromptIndex >= Kernel.PromptIndexMax) {
				ACTIVE_TERMINAL.PromptIndex = 0;
			}
		}
	}
	refreshLastRow();
}

void refreshLastRow(void)
{
	uint16_t headerSize;

	headerSize = strlen(Kernel.Header);
	for (int i = 0, j = 0; i < VGAWIDTH; i++)
	{
		if (i < headerSize) {
			Kernel.TermBuff[((VGAHEIGHT - 1) * VGAWIDTH) + i] = vga_entry(Kernel.Header[i], ACTIVE_TERMINAL.ColorSet);
		}
		else {
			Kernel.TermBuff[((VGAHEIGHT - 1) * VGAWIDTH) + i] = vga_entry(ACTIVE_TERMINAL.Lines[ROW][j++], ACTIVE_TERMINAL.ColorSet);
		}
	}
	update_cursor(COLUMN + headerSize, VGAHEIGHT - 1);
}

void ChangeHeader(char *header)
{
	uint8_t headerSize = strlen(header);
	if (headerSize > MAX_HEADER_SIZE) {
		return;
	}
	memset(Kernel.Header, 0, sizeof(Kernel.Header));
	memcpy(Kernel.Header, header, headerSize);
	char HeaderNumber[2] = {0};
	itoa(Kernel.TermNumber, HeaderNumber, 10);
	Kernel.Header[headerSize+ 1] = ':';
	Kernel.Header[headerSize] = HeaderNumber[0];
}
