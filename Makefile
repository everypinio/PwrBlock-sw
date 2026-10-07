.PHONY: all build clean config flash config_deps_graph lint

# Build system tasks
BUILD_DIR := build
CMAKE_PRESET = debug-tmc

build: config BUILD_DIR
	cmake --build $(BUILD_DIR) --preset $(CMAKE_PRESET)

config: BUILD_DIR
	cmake --preset $(CMAKE_PRESET)

config_deps_graph: BUILD_DIR
	cmake --preset $(CMAKE_PRESET) --graphviz=$(BUILD_DIR)/graph/deps.dot && \
	dot -Tpng -o $(BUILD_DIR)/graph/deps.png $(BUILD_DIR)/graph/deps.dot

clean:
	rm -rf $(BUILD_DIR)/*

BUILD_DIR:
	mkdir -p $(BUILD_DIR)

# Code style tasks
cube_after_gen: cube_remove_extras format

format:
	find ./src -iname '*.h' -o -iname '*.c' | clang-format --style=file -i --files=/dev/stdin

cube_remove_extras:
	rm -rf src/bsp/periph_setup/cmake src/bsp/periph_setup/CMakePresets.json src/bsp/periph_setup/STM32G0B1*.ld

# List of source files to lint limited to src directory
LINT_FILES_LIST := $(shell find src/ -name "*.c")

# lint: build
# 	clang-tidy $(LINT_FILES_LIST)
# 	-p ./build # build dir, shold ontain compile_commands.json
# 	-header-filter=.* # hide warnings from system headers
# 	--extra-arg=-I/opt/gcc-arm-none-eabi/arm-none-eabi/include/ # include path of gcc stdlib headers
# 	--extra-arg=-ferror-limit=0  # Ulimited error count
lint: build
	clang-tidy $(LINT_FILES_LIST) \
	-p ./build \
	-header-filter=.* \
	--extra-arg=-I/opt/gcc-arm-none-eabi/arm-none-eabi/include/ \
	--extra-arg=-ferror-limit=0 \

# Device loading tasks
flash:
	openocd -f interface/stlink.cfg -f target/stm32g0x.cfg \
	-c 'program ./build/power_block.elf verify exit reset'
