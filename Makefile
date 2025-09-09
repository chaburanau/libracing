ifeq ($(OS),Windows_NT)
	SHELL := pwsh.exe
else
	SHELL := pwsh
endif

.SHELLFLAGS := -NoProfile -Command 

TARGET := build
COMPILER := clang
BUILDS_PATH := .\builds
EXAMPLES_PATH := .\examples

BUILD_FLAGS = -std=c99 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror -Wcast-align -Wstrict-prototypes
REMOVE_FLAGS = -ErrorAction Ignore

$(TARGET):
	@ rm -r $(OUT_PATH) $(REMOVE_FLAGS); $$null;
	@ $(COMPILER) $(BUILD_FLAGS) -o $(OUT_PATH) $(ENTRY_PATH)

clean:
	@ rm -r $(BUILDS_PATH)
	@ mkdir $(BUILDS_PATH)

build_ac: ENTRY_PATH = $(EXAMPLES_PATH)\ac.c
build_ac: OUT_PATH = $(BUILDS_PATH)\ac.exe
build_ac: $(TARGET)

build_acc: ENTRY_PATH = $(EXAMPLES_PATH)\acc.c
build_acc: OUT_PATH = $(BUILDS_PATH)\acc.exe
build_acc: $(TARGET)

build_ace: ENTRY_PATH = $(EXAMPLES_PATH)\ace.c
build_ace: OUT_PATH = $(BUILDS_PATH)\ace.exe
build_ace: $(TARGET)

build_iracing: ENTRY_PATH = $(EXAMPLES_PATH)\iracing.c
build_iracing: OUT_PATH = $(BUILDS_PATH)\iracing.exe
build_iracing: $(TARGET)

build_le_mans: ENTRY_PATH = $(EXAMPLES_PATH)\lmu.c
build_le_mans: OUT_PATH = $(BUILDS_PATH)\lmu.exe
build_le_mans: $(TARGET)
