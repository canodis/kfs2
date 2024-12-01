#ifndef KERN_H

# define KERN_H
# define IDT_SIZE 256 // Specific to x86 architecture
# define KERNEL_CODE_SEGMENT_OFFSET 0x8
# define IDT_INTERRUPT_GATE_32BIT 0x8e

# define PIC1_COMMAND_PORT 0x20
# define PIC1_DATA_PORT 0x21
# define PIC2_COMMAND_PORT 0xA0
# define PIC2_DATA_PORT 0xA1

# define KEYBOARD_DATA_PORT 0x60
# define KEYBOARD_STATUS_PORT 0x64


# pragma GCC diagnostic ignored "-Waddress-of-packed-member"

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

struct IDT_pointer {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

struct IDT_entry {
	uint16_t offset_lowerbits; // 16 bits
	uint16_t selector; // 16 bits
	uint8_t zero; // 8 bits
	uint8_t type_attr; // 8 bits
	uint16_t offset_upperbits; // 16 bits
} __attribute__((packed));

extern struct IDT_entry IDT[IDT_SIZE];

//boot.s funcs
extern void				keyboard_handler();
extern	char			port_in(uint16_t nbr);
extern	void			port_out(uint16_t portNo, uint8_t val);
extern void				load_idt(uint32_t* idt_address);
extern void				enable_interrupts();
extern uint32_t			strlen(uint8_t *str);

//keyboard.c funcs
void	kb_init();
void	init_idt();
void	handle_keyboard_interrupt();

//extern	uint8_t *strdup(uint8_t *str);
void	termReset(void);
void	write(int32_t fd, uint8_t *str, uint32_t len);
void	putnbr(int32_t nbr);
void	ArrowHandler(uint8_t mode);
void	leftRightArrowHandler(uint8_t mode);
void	init_gdt();


#endif