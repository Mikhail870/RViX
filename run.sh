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
  -Iuser \
  -o user/prg1.elf \
  user/prg1.c user/lib.c user/start.c
$OBJCOPY -O binary user/prg1.elf user/prg1.bin
xxd -i user/prg1.bin >user/prg1_data.h

# Сборка приложения prg2
$CC $CFLAGS $USER_CFLAGS \
  -Wl,-Tuser/user.ld \
  -Iuser \
  -o user/prg2.elf \
  user/prg2.c user/lib.c user/start.c
$OBJCOPY -O binary user/prg2.elf user/prg2.bin
xxd -i user/prg2.bin >user/prg2_data.h

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
