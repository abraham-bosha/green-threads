# ============================================================================
# Toolchain
# ============================================================================

CC      := clang
AR      := llvm-ar
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

SRC_DIR           := src
TEST_DIR          := tests
EXAMPLE_DIR       := examples
BENCHMARK_DIR     := benchmarks

PUBLIC_INC_DIR       := include
PRIVATE_INC_DIR      := internal
TEST_SUPPORT_INC_DIR := tests/support/include


# ============================================================================
# Compiler Flags
# ============================================================================

COMMON_CFLAGS := \
	-std=c23 \
	-Wall \
	-Wextra \
	-Werror \
	-Wpedantic

DEBUG_CFLAGS := \
	-O0 \
	-g3 \
	-DDEBUG \
    -DGT_LOG_ENABLED \
    -DGT_LOG_LEVEL=GT_LOG_LEVEL_TRACE \
	-fno-omit-frame-pointer

RELEASE_CFLAGS := \
    -O2  \
    -DNDEBUG \
    -DGT_LOG_ENABLED \
    -DGT_LOG_LEVEL=GT_LOG_LEVEL_INFO \
    -flto

BENCHMARK_CFLAGS :=  \
    -O3  \
    -DNGT_LOG_ENABLED

# ============================================================================
# Preprocessor Flags & Include Policies
# ============================================================================

CPPFLAGS_PUBLIC := \
    -I$(PUBLIC_INC_DIR)

CPPFLAGS_PRIVATE := \
    -I$(PRIVATE_INC_DIR)

CPPFLAGS_TEST_SUPPORT := \
    -I$(TEST_SUPPORT_INC_DIR)

# Runtime library (src/)
CPPFLAGS_RUNTIME := \
    $(CPPFLAGS_PUBLIC) \
    $(CPPFLAGS_PRIVATE)

# Examples
CPPFLAGS_EXAMPLE := \
    $(CPPFLAGS_PUBLIC)

# Benchmarks
CPPFLAGS_BENCHMARK := \
    $(CPPFLAGS_PUBLIC)

# Unit tests
CPPFLAGS_UNIT := \
    $(CPPFLAGS_PUBLIC) \
    $(CPPFLAGS_PRIVATE) \
    $(CPPFLAGS_TEST_SUPPORT)

# Integration / Stress / Regression tests
CPPFLAGS_PUBLIC_TEST := \
    $(CPPFLAGS_PUBLIC) \
    $(CPPFLAGS_TEST_SUPPORT)


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

# Track the active profile state inside a hidden file in the build directory
PROFILE_TRACKER := $(BUILD_DIR)/.active_profile

# Force a clean sweep if the requested profile doesn't match the last built profile
ifneq ($(shell cat $(PROFILE_TRACKER) 2>/dev/null),$(PROFILE))
    $(shell rm -rf $(BUILD_DIR) && mkdir -p $(BUILD_DIR) && echo $(PROFILE) > $(PROFILE_TRACKER))
endif


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
# Linker Flags
# ============================================================================

LDFLAGS := $(PROFILE_CFLAGS)

LDLIBS  :=


# ============================================================================
# Runtime Library Sources
# ============================================================================

RUNTIME_SRCS := \
	$(shell find $(SRC_DIR) -name '*.c' 2>/dev/null)

RUNTIME_OBJS := \
	$(RUNTIME_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/runtime/%.o)

RUNTIME_DEPS := \
	$(RUNTIME_SRCS:$(SRC_DIR)/%.c=$(DEP_DIR)/runtime/%.d)


# ============================================================================
# Runtime Object Compilation
# ============================================================================

$(OBJ_DIR)/runtime/%.o: $(SRC_DIR)/%.c
	$(Q)$(MKDIR) $(dir $@)
	$(Q)$(MKDIR) $(dir $(DEP_DIR)/runtime/$*.d)

	$(Q)$(CC) \
		$(CPPFLAGS_RUNTIME) \
		$(COMMON_CFLAGS) \
		$(PROFILE_CFLAGS) \
		-MMD -MP -MF $(DEP_DIR)/runtime/$*.d \
		-c $< \
		-o $@


# ============================================================================
# Runtime Library
# ============================================================================

GT_LIB_NAME   := libgt.a
GT_LIB        := $(LIB_DIR)/$(GT_LIB_NAME)

$(GT_LIB): $(RUNTIME_OBJS)
	$(Q)$(MKDIR) $(dir $@)
	@echo "[AR] $@"

	$(Q)$(AR) $(ARFLAGS) $@ $^


# ============================================================================
# Test Support Library Sources
# ============================================================================

TEST_SUPPORT_SRCS := \
	$(shell find $(TEST_DIR)/support/src -name '*.c' 2>/dev/null)

TEST_SUPPORT_OBJS := \
	$(TEST_SUPPORT_SRCS:$(TEST_DIR)/support/src/%.c=$(OBJ_DIR)/tests/support/%.o)

TEST_SUPPORT_DEPS := \
	$(TEST_SUPPORT_SRCS:$(TEST_DIR)/support/src/%.c=$(DEP_DIR)/tests/support/%.d)


# ============================================================================
# Test Support Object Compilation
# ============================================================================

$(OBJ_DIR)/tests/support/%.o: $(TEST_DIR)/support/src/%.c
	$(Q)$(MKDIR) $(dir $@)
	$(Q)$(MKDIR) $(dir $(DEP_DIR)/tests/support/$*.d)

	$(Q)$(CC) \
		$(CPPFLAGS_TEST_SUPPORT) \
		$(COMMON_CFLAGS) \
		$(PROFILE_CFLAGS) \
		-MMD -MP -MF $(DEP_DIR)/tests/support/$*.d \
		-c $< \
		-o $@


# ============================================================================
# Test Support Library
# ============================================================================

TEST_SUPPORT_LIB_NAME := libtestsupport.a
TEST_SUPPORT_LIB      := $(LIB_DIR)/$(TEST_SUPPORT_LIB_NAME)

$(TEST_SUPPORT_LIB): $(TEST_SUPPORT_OBJS)
	$(Q)$(MKDIR) $(dir $@)
	@echo "[AR] $@"

	$(Q)$(AR) $(ARFLAGS) $@ $^


# ============================================================================
# Unit Test Sources
# ============================================================================

UNIT_TEST_SRCS := \
	$(shell find $(TEST_DIR)/unit -name '*.c' 2>/dev/null)

UNIT_TEST_DEPS := \
	$(UNIT_TEST_SRCS:$(TEST_DIR)/unit/%.c=$(DEP_DIR)/tests/unit/%.d)

UNIT_TEST_BINS := \
	$(UNIT_TEST_SRCS:$(TEST_DIR)/unit/%.c=$(BIN_DIR)/tests/unit/%)


# ============================================================================
# Unit Test Object Compilation
# ============================================================================

$(OBJ_DIR)/tests/unit/%.o: $(TEST_DIR)/unit/%.c
	$(Q)$(MKDIR) $(dir $@)
	$(Q)$(MKDIR) $(dir $(DEP_DIR)/tests/unit/$*.d)

	$(Q)$(CC) \
		$(CPPFLAGS_UNIT)  \
		$(COMMON_CFLAGS)  \
		$(PROFILE_CFLAGS) \
		-MMD -MP -MF $(DEP_DIR)/tests/unit/$*.d \
		-c $< \
		-o $@


# ============================================================================
# Unit Tests Linking
# ============================================================================

$(BIN_DIR)/tests/unit/%: $(OBJ_DIR)/tests/unit/%.o $(GT_LIB) $(TEST_SUPPORT_LIB)
	$(Q)$(MKDIR) $(dir $@)
	@echo "[LD] $@"

	$(Q)$(CC) \
		$(LDFLAGS) \
		$< \
		$(GT_LIB) \
		$(TEST_SUPPORT_LIB) \
		$(LDLIBS) \
		-o $@


# ============================================================================
# Example Sources
# ============================================================================

EXAMPLE_SRCS := \
	$(shell find $(EXAMPLE_DIR) -name 'main.c' 2>/dev/null)

EXAMPLE_DEPS := \
	$(EXAMPLE_SRCS:$(EXAMPLE_DIR)/%/main.c=$(DEP_DIR)/examples/%.d)

EXAMPLE_BINS := \
	$(EXAMPLE_SRCS:$(EXAMPLE_DIR)/%/main.c=$(BIN_DIR)/examples/%)


# ============================================================================
# Example Object Compilation
# ============================================================================

$(OBJ_DIR)/examples/%.o: $(EXAMPLE_DIR)/%/main.c
	$(Q)$(MKDIR) $(dir $@)
	$(Q)$(MKDIR) $(dir $(DEP_DIR)/examples/$*.d)

	$(Q)$(CC) \
		$(CPPFLAGS_EXAMPLE) \
		$(COMMON_CFLAGS) \
		$(PROFILE_CFLAGS) \
		-MMD -MP -MF $(DEP_DIR)/examples/$*.d \
		-c $< \
		-o $@


# ============================================================================
# Example Linking
# ============================================================================

$(BIN_DIR)/examples/%: $(OBJ_DIR)/examples/%.o $(GT_LIB)
	$(Q)$(MKDIR) $(dir $@)
	@echo "[LD] $@"

	$(Q)$(CC) \
		$(LDFLAGS) \
		$< \
		$(GT_LIB) \
		$(LDLIBS) \
		-o $@


# ============================================================================
# Phony Targets
# ============================================================================

.PHONY: \
	all \
	libs \
    unit \
	tests \
	examples \
	run-tests \
	run-examples \
	clean \
	rebuild \
	check \
	lint \
	format \
	check-env \
	help

# Prevent GNU Make from deleting intermediate .o files
.SECONDARY:

# ============================================================================
# Default Build
# ============================================================================

all: libs tests examples


libs: $(GT_LIB) $(TEST_SUPPORT_LIB)


unit: $(UNIT_TEST_BINS)


tests: $(UNIT_TEST_BINS)


examples: $(EXAMPLE_BINS)


# ============================================================================
# Clean & Rebuild
# ============================================================================

clean:
	@echo "[CLEAN] Removing build directory"
	$(Q)$(RM) $(BUILD_DIR)


rebuild: clean all


# ============================================================================
# Run Tests
# ============================================================================

run-tests: tests
	@echo "[RUN] test suite"
	$(Q)set -e; \
	for test in $(UNIT_TEST_BINS); do \
		$$test || { echo "Failure in binary: $$test"; exit 1; }; \
	done


# ============================================================================
# Run Examples
# ============================================================================

run-examples: examples
	@echo "[RUN] examples"
	$(Q)set -e; \
	for example in $(EXAMPLE_BINS); do \
		echo "-> $$example"; \
		$$example || { echo "Failure in binary: $$example"; exit 1; }; \
	done


# ============================================================================
# Lint
# ============================================================================

lint:
	@echo "[LINT] Running cppcheck"
	$(Q)cppcheck \
        --quiet   \
		--enable=all \
		--error-exitcode=1 \
		--inline-suppr \
		--suppress=missingIncludeSystem \
		--suppress=unusedFunction \
		--suppress=unmatchedSuppression \
		--suppress=checkersReport \
		--suppress=duplicateExpression:tests/* \
		--suppress=knownConditionTrueFalse:tests/* \
		-I$(PUBLIC_INC_DIR) \
		-I$(PRIVATE_INC_DIR) \
		$(RUNTIME_SRCS) \
        $(TEST_SUPPORT_SRCS) \
		$(UNIT_TEST_SRCS) \
		$(EXAMPLE_SRCS)


# ============================================================================
# Format
# ============================================================================

format:
	@echo "[FORMAT] project tree"
	$(Q)find \
		$(SRC_DIR) $(PUBLIC_INC_DIR) $(PRIVATE_INC_DIR) $(TEST_DIR) $(EXAMPLE_DIR) \
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

	$(Q)command -v $(CC) >/dev/null || { echo "Missing compiler: $(CC)"; exit 1; }
	$(Q)command -v $(AR) >/dev/null || { echo "Missing archiver: $(AR)"; exit 1; }
	$(Q)command -v cppcheck >/dev/null || { echo "Missing cppcheck"; exit 1; }
	$(Q)command -v clang-format >/dev/null || { echo "Missing clang-format"; exit 1; }

	@echo "Environment OK."


# ============================================================================
# Help
# ============================================================================

help:
	@echo ""
	@echo "Green Threads Build System"
	@echo ""
	@echo "Build Targets"
	@echo "  all              Build library, tests, examples"
	@echo "  libs             Build static libraries only"
	@echo "  unit             Build unit test executables"
	@echo "  tests            Build all test executables"
	@echo "  examples         Build example executables"
	@echo ""
	@echo "Run Targets"
	@echo "  run-tests        Execute test suite"
	@echo "  run-examples     Run example programs"
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


# ============================================================================
# Dependency Files
# ============================================================================

-include $(RUNTIME_DEPS)
-include $(TEST_SUPPORT_DEPS)
-include $(UNIT_TEST_DEPS)
-include $(EXAMPLE_DEPS)
