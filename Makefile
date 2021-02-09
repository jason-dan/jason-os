#
# Jason Dan | 260718739
# Makefile. Based on: https://stackoverflow.com/questions/1079832/how-can-i-configure-my-makefile-for-debug-and-release-builds
#

#
# Compiler flags
#
CC=gcc
CFLAGS=-I$(IDIR)

#
# Project Files
#
IDIR=./include
BDIR=./build
SDIR=./src
DEPS=$(wildcard $(IDIR)/*.h)
OBJ = $(patsubst $(SDIR)%.c,%.o,$(wildcard $(SDIR)/*.c))
EXE = mysh

#
# Debug build settings
#
DBGDIR = $(BDIR)/debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR), $(OBJ))
DBGCFLAGS = -g

#
# Release build settings
#
RELDIR = $(BDIR)/release
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR), $(OBJ))

.PHONY: all clean debug prep release remake copy

# Default build
all: prep release copy

#
# Debug rules
#
debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $(DBGEXE) $^

$(DBGDIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $<

#
# Release rules
#
release: $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(CC) $(CFLAGS) -o $(RELEXE) $^

$(RELDIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)

copy:
	cp -f $(RELEXE) $(EXE)