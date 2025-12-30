CC = gcc
CFLAGS = -Wall -Wextra -Wno-override-init -g -std=c99 -I. -I$(INCDIR)
DEPFLAGS = -MMD -MP -MF $(DEPDIR)/$*.d

SRCDIR = src
INCDIR = include
LIBDIR = lib
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
BINDIR = $(BUILDDIR)/bin
DEPDIR = $(BUILDDIR)/deps

TESTDIR = test
TESTBINDIR = $(BUILDDIR)/test-bin

UNITY_DIR = $(LIBDIR)/Unity
UNITY_SRC = $(UNITY_DIR)/src/unity.c
UNITY_INC = $(UNITY_DIR)/src

SOURCES = $(shell find $(SRCDIR) -name "*.c")
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))
DEPS = $(patsubst $(SRCDIR)/%.c,$(DEPDIR)/%.d,$(SOURCES))

TEST_SOURCES = $(shell find $(TESTDIR) -name "*.c")
TEST_OBJECTS = $(patsubst $(TESTDIR)/%.c,$(OBJDIR)/%.o,$(TEST_SOURCES))
TEST_DEPS = $(patsubst $(TESTDIR)/%.c,$(DEPDIR)/%.d,$(TEST_SOURCES))
TEST_BINARIES = $(patsubst $(TESTDIR)/%.c,$(TESTBINDIR)/%,$(TEST_SOURCES))

$(shell mkdir -p $(OBJDIR) $(BINDIR) $(DEPDIR) $(TESTBINDIR))

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEPDIR)/$*)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(TESTBINDIR)/%: $(TESTDIR)/%.c $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) | $(TESTBINDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEPDIR)/$*)
	$(CC) $(CFLAGS) -I$(UNITY_INC) $(DEPFLAGS) -o $@ $< $(UNITY_SRC) $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) $(LDFLAGS)

test-build: $(TEST_BINARIES)

test: test-build
	@echo "Running tests..."
	@for test in $(TEST_BINARIES); do \
		printf "\n"; \
		echo "Running $$test..."; \
		$$test || exit 1; \
	done
	@echo "All tests passed!"

$(OBJDIR) $(BINDIR) $(DEPDIR) $(TESTBINDIR):
	mkdir -p $@

-include $(DEPS)

clean:
	rm -rf $(BUILDDIR)

.PHONY: clean test test-build
