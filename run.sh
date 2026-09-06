#!/bin/bash
set -xue

QEMU=qemu-system-riscv64

# Путь к clang и его флагам
CC=clang
CFLAGS="-std=gnu11 -O2 -g3 -Wall -Wextra --target=riscv64 -ffreestanding -nostdlib -mcmodel=medany"
USER_CFLAGS="-fno-pic"
OBJCOPY=llvm-objcopy

# Сборка приложения prg1
$CC $CFLAGS $USER_CFLAGS \
  -Wl,-Tuser/user.ld \
  -Iuser -I. \
  -o user/prg1.elf \
  user/prg1.c user/lib.c user/start.c
$OBJCOPY -O binary user/prg1.elf user/prg1.bin
xxd -i user/prg1.bin >user/prg1_data.h

# Сборка приложения prg2
$CC $CFLAGS $USER_CFLAGS \
  -Wl,-Tuser/user.ld \
  -Iuser -I. \
  -o user/prg2.elf \
  user/prg2.c user/lib.c user/start.c
$OBJCOPY -O binary user/prg2.elf user/prg2.bin
xxd -i user/prg2.bin >user/prg2_data.h

# Сборка приложения prg3
$CC $CFLAGS $USER_CFLAGS \
  -Wl,-Tuser/user.ld \
  -Iuser -I. \
  -o user/prg3.elf \
  user/prg3.c user/lib.c user/start.c
$OBJCOPY -O binary user/prg3.elf user/prg3.bin
xxd -i user/prg3.bin >user/prg3_data.h

# Сборка сервреа VFS
$CC $CFLAGS $USER_CFLAGS \
  -Wl,-Tuser/user.ld \
  -Iuser -I. \
  -o user/VFS.elf \
  user/VFS.c user/lib.c user/start.c
$OBJCOPY -O binary user/VFS.elf user/VFS.bin
xxd -i user/VFS.bin >user/VFS_data.h

# Сборка сервреа терминала
$CC $CFLAGS $USER_CFLAGS \
  -Wl,-Tuser/user.ld \
  -Iuser -I. \
  -o user/terminal.elf \
  user/terminal.c user/lib.c user/start.c
$OBJCOPY -O binary user/terminal.elf user/terminal.bin
xxd -i user/terminal.bin >user/terminal_data.h

# Сборка ядра
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
  main.c common.c HAL.c kalloc.c vm.c timer.c entry.S trap_handle.c \
  PLIC.c process_manager.c trampoline.S kernelvec.S switch_context.S \
  ipc.c

# Запуск QEMU
$QEMU -machine virt -bios default -nographic \
  -serial mon:stdio --no-reboot \
  -kernel kernel.elf
#-d int,exec,cpu -D qemu.log # \
