bits 32
section .multiboot
	dd 0x1BADB002	; Magic number
	dd 0x0			; Flags
	dd - (0x1BADB002 + 0x0)	; Checksum

global stack_bottom
global stack_top

extern gdt_desc

section .text
	global start
	global strlen
	global strdup
	global port_in
	global port_out
	global keyboard_handler
	global enable_interrupts
	global load_idt
	global load_gdt
	extern handle_keyboard_interrupt
	extern kmain


load_gdt:
    mov eax, [esp + 4]
    lgdt [eax]

    ; Veri segmentlerini ayarla
    mov ax, 0x10        ; GDT'deki veri segmentinin seçicisi
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax

    mov ax, 0x18        ; GDT'deki başka bir segment (örneğin, GS)
    mov gs, ax

    ; Koruma modunu etkinleştir (Protection Enable bitini ayarla)
    mov eax, cr0 
    or eax, 1           ; CR0'ın PE (Protection Enable) bitini etkinleştir
    mov cr0, eax

    ; CS'yi güncellemek için uzak (far) jump kullanın
    jmp 0x08:.flush     ; GDT'deki kod segmentine atla
.flush:
    ret


strlen:
	push ebp
	mov ebp, esp
	mov edi, [ebp + 8]

	mov eax, 0
	xor ecx, ecx

	loop:
	cmp byte [edi + ecx], 0
	je end
	inc ecx
	inc eax
	jmp loop

	end:
		pop ebp
		ret


load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	ret

enable_interrupts:
	sti
	ret

keyboard_handler:
	pushad
	call handle_keyboard_interrupt
	popad
	iretd


port_in:
	mov edx, [esp + 4]
	in al, dx
	ret

port_out:
	mov edx, [esp + 4]
	mov eax, [esp + 8]
	out dx, al
	ret

start:
	mov esp, stack_space        	; set stack pointer
	cli				; Disable interrupts
	mov esp, stack_space
	call kmain

section .bss
resb 8192			; 8KB for stack
stack_space: