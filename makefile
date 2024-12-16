# Set paths
BIN_DIR = bin
SRC_DIR = sources

# Source files
ASM_BOOTLOADER = $(SRC_DIR)/bootloader.asm
ASM_KERNEL_ENTRY = $(SRC_DIR)/kernel_entry.asm
ASM_EMULFIX = $(SRC_DIR)/emulfix.asm
C_SOURCES = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/**/*.c)  # All .c files in sources and subdirectories
HEADERS = $(wildcard $(SRC_DIR)/*.h $(SRC_DIR)/**/*.h)  # All .h files in sources and subdirectories

# Object files generated from each .c source file
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(C_SOURCES))

# Output files
BOOT_BIN = $(BIN_DIR)/boot.bin
KERNEL_ENTRY_O = $(BIN_DIR)/kernel_entry.o
FULL_KERNEL_BIN = $(BIN_DIR)/full_kernel.bin
OS_BIN = $(BIN_DIR)/OS.bin
EMULFIX_BIN = $(BIN_DIR)/zeroes.bin

# Compiler and assembler settings
CC = i386-elf-gcc
LD = i386-elf-ld
NASM = nasm
QEMU = qemu-system-x86_64
CFLAGS = -ffreestanding -m32 -no-pie -g -c
LDFLAGS = -Ttext 0x1000 --oformat binary

# Default build target
all: $(OS_BIN)

# Assemble bootloader
$(BOOT_BIN): $(ASM_BOOTLOADER)
	$(NASM) $< -f bin -o $@

# Assemble kernel entry
$(KERNEL_ENTRY_O): $(ASM_KERNEL_ENTRY)
	$(NASM) $< -f elf -o $@

# Compile each .c file into an object file in the bin directory
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

# Assemble emulfix (zeroes filler)
$(EMULFIX_BIN): $(ASM_EMULFIX)
	$(NASM) $< -f bin -o $@

# Link all object files to create the full kernel binary
$(FULL_KERNEL_BIN): $(KERNEL_ENTRY_O) $(OBJ_FILES)
	$(LD) -o $@ $(LDFLAGS) $^

# Combine bootloader, kernel, and emulfix into the final OS binary
$(OS_BIN): $(BOOT_BIN) $(FULL_KERNEL_BIN) $(EMULFIX_BIN)
	cat $^ > $@

# Run the OS in QEMU
run: $(OS_BIN)
	$(QEMU) -drive format=raw,file=$(OS_BIN),index=0,if=floppy -m 128M

# Run the OS in QEMU in fullscreen
runf: $(OS_BIN)
	$(QEMU) -k -full-screen -drive format=raw,file=$(OS_BIN),index=0,if=floppy -m 128M

# Clean all build artifacts
clean:
	rm -rf $(BIN_DIR)/*.bin $(BIN_DIR)/*.o $(OS_BIN)

install:
	sudo apt-get update
	sudo apt-get install -y \
		qemu-utils \
		qemu-system-x86 \
		qemu-system-gui \
		binutils-multiarch \
		nasm
