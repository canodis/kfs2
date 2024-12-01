/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zodiak <zodiak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 13:41:05 by rbourgea          #+#    #+#             */
/*   Updated: 2023/05/30 23:26:55 by zodiak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "inc/kern.h"
#include "inc/gdt.h"
#include "inc/utils.h"

/* ************************************************************************** */
/* Globals                                                                    */
/* ************************************************************************** */

t_gdt_entry	gdt_entries[GDT_ENTRIES];
t_gdt_ptr	*gdt_ptr = (t_gdt_ptr *)GDT_ADDRESS;

/* ************************************************************************** */
/* Code                                                                       */
/* ************************************************************************** */

static void create_descriptor(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
	gdt_entries[num].limit_low = (limit & 0xFFFF);
	gdt_entries[num].base_low = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].access = access;
	gdt_entries[num].attributes = (limit >> 16) & 0x0F;
	gdt_entries[num].attributes |= ((flags << 4) & 0xF0);
	gdt_entries[num].base_high = (base >> 24) & 0xFF;
}

void	init_gdt()
{
	gdt_ptr->limit = (sizeof(t_gdt_entry) * GDT_ENTRIES) - 1;
	gdt_ptr->base = ((uint32_t)(&gdt_entries));

	create_descriptor(0, 0, 0, 0, FLAG_D_32);				// null
	create_descriptor(1, 0, 0xFFFF, (uint8_t)(GDT_CODE_PL0),  FLAG_D_32);	// kernel code (kernel binary)
	create_descriptor(2, 0, 0xFFFF, (uint8_t)(GDT_DATA_PL0),  FLAG_D_32);	// kernel data (kernel heap)
	create_descriptor(3, 0, 0xFFFF, (uint8_t)(GDT_STACK_PL0), FLAG_D_32);	// kernel stack
	create_descriptor(4, 0, 0xBFFF, (uint8_t)(GDT_CODE_PL3),  FLAG_D_32);	// user code (user-executed binaries)
	create_descriptor(5, 0x01000000, 0xBFFF, (uint8_t)(GDT_DATA_PL3),  FLAG_D_32);	// user data (user heap)
	create_descriptor(6, 0, 0xBFFF, (uint8_t)(GDT_STACK_PL3), FLAG_D_32);	// user stack

	load_gdt(((uint32_t)gdt_ptr));
}

void printAllStackAddress(void)
{
	uint32_t stack_base = (gdt_entries[3].base_high << 24) |
						  (gdt_entries[3].base_middle << 16) |
						  (gdt_entries[3].base_low);
	uint32_t stack_limit = ((gdt_entries[3].attributes & 0x0F) << 16) |
						   (gdt_entries[3].limit_low);
	uint32_t stack_end = stack_base + stack_limit;

	write(1, "Kernel Stack:\n", 15);

	for (uint32_t addr = stack_base; addr <= stack_end; addr += 4) {
		uint32_t value = *((uint32_t*)addr);
		write(1, "Address: ", 9);
		write_hex(addr);
		write(1, " Value: ", 8);
		write_hex(value);
		write(1, "\n", 1);
	}
}

void printSpesificStackAddress(uint32_t address)
{
	uint32_t stack_base = (gdt_entries[3].base_high << 24) |
						  (gdt_entries[3].base_middle << 16) |
						  (gdt_entries[3].base_low);
	uint32_t stack_limit = ((gdt_entries[3].attributes & 0x0F) << 16) |
						   (gdt_entries[3].limit_low);
	uint32_t stack_end = stack_base + stack_limit;

	if (address < stack_base || address > stack_end) {
		write(1, "Error: Address out of kernel stack range.\n", 42);
		return;
	}

	uint32_t value = *((uint32_t*)address);

	write(1, "Address: ", 9);
	write_hex(address);
	write(1, " Value: ", 8);
	write_hex(value);
	write(1, "\n", 1);
}

void write_to_stack(uint32_t address, uint32_t value)
{
	uint32_t stack_base = (gdt_entries[3].base_high << 24) |
						  (gdt_entries[3].base_middle << 16) |
						  (gdt_entries[3].base_low);
	uint32_t stack_limit = ((gdt_entries[3].attributes & 0x0F) << 16) |
						   (gdt_entries[3].limit_low);
	uint32_t stack_end = stack_base + stack_limit;

	if (address < stack_base || address > stack_end) {
		write(1, "Error: Address out of kernel stack range.\n", 42);
		return;
	}

	*((uint32_t*)address) = value;

	uint32_t read_value = *((uint32_t*)address);
	write(1, "Wrote value: ", 13);
	write_hex(value);
	write(1, " to address: ", 13);
	write_hex(address);
	write(1, "\n", 1);

	if (read_value == value) {
		write(1, "Verification successful: Value matches.\n", 40);
	} else {
		write(1, "Verification failed: Value does not match.\n", 43);
	}
}
