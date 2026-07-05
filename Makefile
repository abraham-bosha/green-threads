# ============================================================================
# Toolchain
# ============================================================================

CC      := clang
AR      := ar
ARFLAGS := rcs
RM      := rm -rf
MKDIR   := mkdir -p


# ============================================================================
# Build Verbosity Control 
# ============================================================================

V ?= 0 

ifeq ($(V),1)
    Q := 
else 
    Q := @
endif


# ============================================================================
# Project Layout
# ============================================================================

SRC_DIR     := src
INC_DIR     := include
TEST_DIR    := tests
EXAMPLE_DIR := examples
BENCH_DIR   := benchmarks

PUBLIC_INC_DIR  := include
PRIVATE_INC_DIR := internal


# ============================================================================
# Compiler Flags
# ============================================================================

COMMON_CFLAGS := \
	-std=c23 \
	-Wall \
	-Wextra \
	-Werror \
	-Wpedantic \
	-MMD \
	-MP

DEBUG_CFLAGS := \
	-O0 \
	-g3 \
	-DDEBUG \
	-fno-omit-frame-pointer

RELEASE_CFLAGS := \
    -O2  \
    -DNDEBUG  \
    -flto

LDFLAGS :=

LDLIBS  :=


# ============================================================================
# Include Flags
# ============================================================================

CPPFLAGS_PUBLIC  := -I$(PUBLIC_INC_DIR)
CPPFLAGS_PRIVATE := -I$(PRIVATE_INC_DIR)

CPPFLAGS_SRC := \
	$(CPPFLAGS_PUBLIC) \
	$(CPPFLAGS_PRIVATE)

CPPFLAGS_TEST := \
	$(CPPFLAGS_PUBLIC) \
	$(CPPFLAGS_PRIVATE)

CPPFLAGS_EXAMPLE := \
	$(CPPFLAGS_PUBLIC)

CPPFLAGS_BENCH := \
	$(CPPFLAGS_PUBLIC)


# ============================================================================
# Build Layout
# ============================================================================

BUILD_DIR := build

PROFILE ?= debug

PROFILE_DIR := $(BUILD_DIR)/$(PROFILE)

OBJ_DIR := $(PROFILE_DIR)/obj
DEP_DIR := $(PROFILE_DIR)/dep
LIB_DIR := $(PROFILE_DIR)/lib
BIN_DIR := $(PROFILE_DIR)/bin


# ============================================================================
# Profile Selection
# ============================================================================

ifeq ($(PROFILE),debug)
    PROFILE_CFLAGS := $(DEBUG_CFLAGS)
else ifeq ($(PROFILE),release)
    PROFILE_CFLAGS := $(RELEASE_CFLAGS)
else
    $(error Unknown PROFILE '$(PROFILE)')
endif


# ============================================================================
# Source Discovery
# ============================================================================

SRCS := $(shell find $(SRC_DIR) -name '*.c' 2>/dev/null)

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

DEPS := $(SRCS:$(SRC_DIR)/%.c=$(DEP_DIR)/%.d)


# ============================================================================
# Object Compilation Rule
# ============================================================================

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@$(MKDIR) $(dir $(DEP_DIR)/$*)
	$(Q)$(CC) \
		$(CPPFLAGS_SRC) \
		$(COMMON_CFLAGS) \
		$(PROFILE_CFLAGS) \
		-MF $(DEP_DIR)/$*.d \
		-c $< \
		-o $@


# ============================================================================
# Static Library
# ============================================================================

LIB_NAME   := libgt.a
LIB_TARGET := $(LIB_DIR)/$(LIB_NAME)

$(LIB_TARGET): $(OBJS)
	@$(MKDIR) $(dir $@)
	@echo "[AR] Creating static library: $@"
	@$(AR) $(ARFLAGS) $@ $^


# ============================================================================
# Tests
# ============================================================================

TEST_SRCS := $(shell find $(TEST_DIR) -name '*.c' 2>/dev/null)

TEST_BINS := $(TEST_SRCS:$(TEST_DIR)/%.c=$(BIN_DIR)/tests/%)

$(TEST_BINS): $(TEST_SRCS) $(LIB_TARGET)

$(BIN_DIR)/tests/%: $(TEST_DIR)/%.c $(LIB_TARGET)
	@$(MKDIR) $(dir $@)
	@echo "[TEST] Building $@"
	$(Q)$(CC) \
		$(CPPFLAGS_TEST) \
		$(COMMON_CFLAGS) \
		$(PROFILE_CFLAGS) \
		$< \
		$(LIB_TARGET) \
		-o $@


# ============================================================================
# Examples
# ============================================================================

EXAMPLE_SRCS := $(shell find $(EXAMPLE_DIR) -name 'main.c' 2>/dev/null)

EXAMPLE_BINS := $(EXAMPLE_SRCS:$(EXAMPLE_DIR)/%.c=$(BIN_DIR)/examples/%)

$(EXAMPLE_BINS): $(LIB_TARGET)

$(BIN_DIR)/examples/%: $(EXAMPLE_DIR)/%.c $(LIB_TARGET)
	@$(MKDIR) $(dir $@)
	@echo "[EXAMPLE] Building $@"
	$(Q)$(CC) \
		$(CPPFLAGS_EXAMPLE) \
		$(COMMON_CFLAGS) \
		$(PROFILE_CFLAGS) \
		$< \
		$(LIB_TARGET) \
		-o $@


# ============================================================================
# Benchmarks
# ============================================================================

BENCH_SRCS := $(shell find $(BENCH_DIR) -name 'main.c' 2>/dev/null)

BENCH_BINS := $(BENCH_SRCS:$(BENCH_DIR)/%.c=$(BIN_DIR)/benchmarks/%)

$(BENCH_BINS): $(LIB_TARGET)

$(BIN_DIR)/benchmarks/%: $(BENCH_DIR)/%.c $(LIB_TARGET)
	@$(MKDIR) $(dir $@)
	@echo "[BENCHMARKS] Building $@"
	$(Q)$(CC) \
		$(CPPFLAGS_BENCH) \
		$(COMMON_CFLAGS) \
		$(PROFILE_CFLAGS) \
		$< \
		$(LIB_TARGET) \
		-o $@


# ============================================================================
# Phony Targets
# ============================================================================

.PHONY: \
	all \
	lib \
	tests \
	examples \
	benchmarks \
	run-tests \
	run-examples \
	run-benchmarks \
	clean \
	rebuild \
	check \
	lint \
	format \
	check-env \
	help


# ============================================================================
# Default Build
# ============================================================================

all: lib tests examples benchmarks


lib: $(LIB_TARGET)


tests: $(TEST_BINS)


examples: $(EXAMPLE_BINS)


benchmarks: $(BENCH_BINS)


# ============================================================================
# Clean & Rebuild
# ============================================================================

clean:
	@echo "[CLEAN] Removing build directory"
	@$(RM) $(BUILD_DIR)


rebuild: clean all


# ============================================================================
# Run Tests
# ============================================================================

run-tests: tests
	@echo "[RUN] tests"
	@set -e; \
	for test in $(TEST_BINS); do \
		echo "-> $$test"; \
		$$test || exit 1; \
	done


# ============================================================================
# Run Examples
# ============================================================================

run-examples: examples
	@echo "[RUN] examples"
	@for example in $(EXAMPLE_BINS); do \
		echo "-> $$example"; \
		$$example; \
	done


# ============================================================================
# Run Benchmarks
# ============================================================================

run-benchmarks: benchmarks
	@echo "[RUN] benchmarks"
	@for bench in $(BENCH_BINS); do \
		echo "-> $$bench"; \
		$$bench; \
	done


# ============================================================================
# Lint
# ============================================================================

lint:
	@echo "[LINT] Running cppcheck"
	@if [ -z "$(SRCS)" ]; then \
		echo "No source files found in $(SRC_DIR)"; \
		exit 1; \
	fi
	@cppcheck \
		--enable=all \
		--error-exitcode=1 \
		--inline-suppr \
		--suppress=missingIncludeSystem \
		--suppress=unusedFunction \
		--suppress=checkersReport \
		-I$(PUBLIC_INC_DIR) \
		-I$(PRIVATE_INC_DIR) \
		$(SRCS)


# ============================================================================
# Format
# ============================================================================

format:
	@echo "[FORMAT] source tree"
	@find \
		$(SRC_DIR) $(INC_DIR) $(TEST_DIR) $(EXAMPLE_DIR) $(BENCH_DIR) \
		-type f \
		\( -name '*.c' -o -name '*.h' \) \
		-exec clang-format -i {} +


# ============================================================================
# Check Pipeline
# ============================================================================

check: lint tests run-tests


# ============================================================================
# Environment Check
# ============================================================================

check-env:
	@echo "[CHECK] build environment"

	@command -v $(CC) >/dev/null || { echo "Missing compiler: $(CC)"; exit 1; }
	@command -v $(AR) >/dev/null || { echo "Missing archiver: $(AR)"; exit 1; }
	@command -v cppcheck >/dev/null || { echo "Missing cppcheck"; exit 1; }
	@command -v clang-format >/dev/null || { echo "Missing clang-format"; exit 1; }

	@echo "Environment OK."


# ============================================================================
# Help
# ============================================================================

help:
	@echo ""
	@echo "Green Threads Build System"
	@echo ""
	@echo "Build Targets"
	@echo "  all              Build library, tests, examples, benchmarks"
	@echo "  lib              Build static library only"
	@echo "  tests            Build test executables"
	@echo "  examples         Build example executables"
	@echo "  benchmarks       Build benchmark executables"
	@echo ""
	@echo "Run Targets"
	@echo "  run-tests        Execute test suite"
	@echo "  run-examples     Run example programs"
	@echo "  run-benchmarks   Run benchmark programs"
	@echo ""
	@echo "Quality Targets"
	@echo "  check            Run lint + tests"
	@echo "  lint             Static analysis (cppcheck)"
	@echo "  format           Format source tree (clang-format)"
	@echo "  check-env        Verify toolchain dependencies"
	@echo ""
	@echo "Maintenance"
	@echo "  clean            Remove build directory"
	@echo "  rebuild          Clean and rebuild"
	@echo ""
	@echo "  help             Show this help message\n"

-include $(DEPS)
