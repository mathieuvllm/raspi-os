CC = arm-none-eabi-gcc
CPU = cortex-a7
CFLAGS = -mcpu=$(CPU) -ffreestanding -std=gnu23 -g
CSRCFLAGS = -O2 -Wall -Wextra
LFLAGS = -ffreestanding -O2 -nostdlib

KER_SRC = ../src/kernel
KER_HEAD = ../include
COMMON_SRC = ../src/common
OBJ_DIR = objects
KERSOURCES = $(wildcard $(KER_SRC)/*.c)
COMMONSOURCES = $(wildcard $(COMMON_SRC)/*.c)
ASMSOURCES = $(wildcard $(KER_SRC)/*.S)
OBJECTS = $(patsubst $(KER_SRC)/%.c, $(OBJ_DIR)/%.o, $(KERSOURCES))
OBJECTS += $(patsubst $(COMMON_SRC)/%.c, $(OBJ_DIR)/%.o, $(COMMONSOURCES))
OBJECTS += $(patsubst $(KER_SRC)/%.S, $(OBJ_DIR)/%.o, $(ASMSOURCES))

ELF_NAME = kernel.elf
IMG_NAME = kernel.img
OBJCOPY = arm-none-eabi-objcopy
LOAD_ADDR ?= 0x8000

build: $(IMG_NAME)

$(ELF_NAME): $(OBJECTS) $(HEADERS) linker.ld
	$(CC) -T linker.ld -Wl,--defsym=LOAD_ADDR=$(LOAD_ADDR) -o $@ $(LFLAGS) $(OBJECTS)

$(IMG_NAME): $(ELF_NAME)
	$(OBJCOPY) $< -O binary $@

$(OBJ_DIR)/%.o: $(KER_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -I$(KER_HEAD) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KER_SRC)/%.S
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -c $< -o $@

$(OBJ_DIR)/%.o: $(COMMON_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -I$(KER_HEAD) -c $< -o $@ $(CSRCFLAGS)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(ELF_NAME) $(IMG_NAME)

run: LOAD_ADDR = 0x10000
run: clean build
	qemu-system-arm -m 1024 -M raspi2b -serial stdio -kernel $(IMG_NAME)

debug: LOAD_ADDR = 0x10000
debug: clean build
	qemu-system-arm -m 1024 -M raspi2b -serial stdio -kernel $(IMG_NAME) -s -S

.PHONY: build clean run debug
