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

# Сборка приложения prg2
$CC $CFLAGS $USER_CFLAGS \
  -Wl,-Tuser/user.ld \
  -Iuser -I. \
  -o user/prg2.elf \
  user/prg2.c user/lib.c user/start.c
$OBJCOPY -O binary user/prg2.elf user/prg2.bin

# Сборка приложения prg3
$CC $CFLAGS $USER_CFLAGS \
  -Wl,-Tuser/user.ld \
  -Iuser -I. \
  -o user/prg3.elf \
  user/prg3.c user/lib.c user/start.c
$OBJCOPY -O binary user/prg3.elf user/prg3.bin

# Сборка сервреа VFS
$CC $CFLAGS $USER_CFLAGS \
  -Wl,-Tuser/user.ld \
  -Iuser -I. \
  -o user/VFS.elf \
  user/VFS.c user/lib.c user/servers.c user/start.c
$OBJCOPY -O binary user/VFS.elf user/VFS.bin

# Сборка сервреа терминала
$CC $CFLAGS $USER_CFLAGS \
  -Wl,-Tuser/user.ld \
  -Iuser -I. \
  -o user/terminal.elf \
  user/terminal.c user/lib.c user/servers.c user/start.c
$OBJCOPY -O binary user/terminal.elf user/terminal.bin

# Сборка сервреа ввода
$CC $CFLAGS $USER_CFLAGS \
  -Wl,-Tuser/user.ld \
  -Iuser -I. \
  -o user/uinput.elf \
  user/uinput.c user/lib.c user/servers.c user/start.c
$OBJCOPY -O binary user/uinput.elf user/uinput.bin

# Сборка сервреа шелла
$CC $CFLAGS $USER_CFLAGS \
  -Wl,-Tuser/user.ld \
  -Iuser -I. \
  -o user/shell.elf \
  user/shell.c user/lib.c user/servers.c user/start.c
$OBJCOPY -O binary user/shell.elf user/shell.bin

# Сборка сервреа UVM
$CC $CFLAGS $USER_CFLAGS \
  -Wl,-Tuser/user.ld \
  -Iuser -I. \
  -o user/UPM.elf \
  user/UPM.c user/lib.c user/servers.c user/start.c
$OBJCOPY -O binary user/UPM.elf user/UPM.bin

# Подготовка tar диска с бинарными файлами disk.tar
mkdir -p disk
cp user/*.bin disk/
(cd disk && tar cf ../user/disk.tar --format=ustar ./*)
rm -rf disk
rm -f user/*.bin
rm -f user/*.elf

# Сборка ядра
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
  main.c common.c HAL.c kalloc.c vm.c timer.c entry.S trap_handle.c \
  PLIC.c process_manager.c trampoline.S kernelvec.S switch_context.S \
  ipc.c bootstrap.c

# Запуск QEMU
$QEMU -machine virt -bios default -nographic \
  -serial mon:stdio --no-reboot \
  -kernel kernel.elf
#-d int,exec,cpu -D qemu.log # \
