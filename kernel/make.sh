i686-elf-gcc -c src/kmain.c -o obj/kmain.obj -ffreestanding -Wall -Wextra \
	-Werror -O2 -std=gnu99 -Wno-div-by-zero
i686-elf-gcc -c src/memutil.c -o obj/memutil.obj -ffreestanding -Wall -Wextra \
	-Werror -O2 -std=gnu99
i686-elf-gcc -c src/console.c -o obj/console.obj -ffreestanding -Wall -Wextra \
	-O2 -std=gnu99
i686-elf-gcc -c src/descriptor_tables.c -o obj/descriptor_tables.obj \
	-O2 -std=gnu99 -ffreestanding
i686-elf-gcc -c src/pit.c -o obj/pit.obj -O2 -std=gnu99 -ffreestanding
i686-elf-gcc -c src/keyboard.c -o obj/keyboard.obj -O2 -std=gnu99 -ffreestanding
i686-elf-gcc -c src/command_mgr.c -o obj/command_mgr.obj -O2 -std=gnu99 \
	-ffreestanding
i686-elf-gcc -c src/mechanic/ata_driver.c -o obj/mechanic_ata_driver.obj -O2 \
	-std=gnu99 -ffreestanding
i686-elf-gcc -c src/serial.c -o obj/serial.obj -O2 -std=gnu99 -ffreestanding
i686-elf-gcc -c src/memory.c -o obj/memory.obj -O2 -std=gnu99 -ffreestanding

i686-elf-as -c grub-boot.asm -o obj/grub-boot.obj
i686-elf-as -c src/memory.asm -o obj/memory.asm.obj
nasm -f elf src/descriptor_tables.asm -o obj/descriptor_tables.asm.obj

# linking
i686-elf-gcc -T linker.ld -o bin/popcorn.elf -ffreestanding -O2 -nostdlib \
	obj/kmain.obj obj/grub-boot.obj obj/memutil.obj obj/console.obj \
	obj/descriptor_tables.asm.obj obj/descriptor_tables.obj \
	obj/pit.obj obj/keyboard.obj obj/command_mgr.obj obj/serial.obj \
	obj/memory.asm.obj obj/memory.obj obj/mechanic_ata_driver.obj -lgcc

grub-mkrescue -o popcorn-tests.iso bin