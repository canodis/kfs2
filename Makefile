NSRC = boot.s
CSRC = $(shell find ./src -name "*.c")
OBJ = kern.o boot.o printFuncs.o keyboard.o utils.o terminal.o gdt.o terminalCommands.o kmalloc.o command.o
CFLAGS = -fno-builtin -fno-builtin -fno-builtin -nostdlib -nodefaultlibs
CC = gcc
AC = nasm

docker-run:
	-docker stop oyumusak-kfs
	-docker rm	oyumusak-kfs
	mkdir -p boot
	docker build --platform linux/amd64 -t oyumusak-kfs .
	docker run -d --name oyumusak-kfs --rm -i -t oyumusak-kfs
	docker cp . oyumusak-kfs:/kfs
	docker cp grub.cfg oyumusak-kfs:/kfs
	docker exec -t oyumusak-kfs nasm -f elf32 boot.s -o boot.o
	docker exec -t oyumusak-kfs gcc -m32 -ffreestanding -I../inc ${CFLAGS} -c ${CSRC}
	docker exec -t oyumusak-kfs ld -m elf_i386 -T linker.ld -o omex.bin ${OBJ}
	docker exec -t oyumusak-kfs mkdir -p iso/boot/grub
	docker exec -t oyumusak-kfs mv grub.cfg iso/boot/grub
	docker exec -t oyumusak-kfs mv omex.bin iso/boot
	docker exec -t oyumusak-kfs rm -rf /kfs/omex.iso
	docker exec -t oyumusak-kfs grub-mkrescue -o omex.iso ./iso/
	docker cp oyumusak-kfs:/kfs/omex.iso boot/omex.iso
	qemu-system-i386 -cdrom boot/omex.iso
	