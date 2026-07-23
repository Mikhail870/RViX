#!/bin/bash
set -xue

QEMU=qemu-system-riscv64

# Путь к clang и его флагам
CC=clang
CFLAGS="-std=gnu11 -O2 -g3 -Wall -Wextra --target=riscv64 -ffreestanding -nostdlib -mcmodel=medany"

# Сборка ядра
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
  main.c common.c HAL.c kalloc.c vm.c timer.c entry.S trap_handle.c \
  PLIC.c process_manager.c trampoline.S kernelvec.S switch_context.S \
  trampoline.S

# Запуск QEMU
$QEMU -machine virt -bios default -nographic \
  -serial mon:stdio --no-reboot \
  -kernel kernel.elf
