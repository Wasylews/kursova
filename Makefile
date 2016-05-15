TOOLS_PATH = TOOLS
CC = $(TOOLS_PATH)\TASM.EXE
LD = $(TOOLS_PATH)\TLINK.EXE
RM = del
MKDIR = mkdir

CFLAGS = /z /zi
LDFLAGS = /v

BUILD_DIR = build
INCLUDE_DIR = libasm\libasm

# sometimes dosbox try create obj file with wrong path
# quick and dirty fix is always clean build folder before
all: clean main.exe


.asm.obj:
	# separate build and source folders
	@if not exist $(BUILD_DIR) $(MKDIR) $(BUILD_DIR)

	$(CC) $(CFLAGS) /i$(INCLUDE_DIR) $< $(BUILD_DIR)\$@

.obj.exe:
	$(LD) $(LDFLAGS) $(BUILD_DIR)\$<

clean:
	-@$(RM) $(BUILD_DIR)\*.obj
	-@$(RM) $(BUILD_DIR)\*.exe
	-@$(RM) $(BUILD_DIR)\*.map
