#
# Compiler flags
#
CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wextra

#
# Project files
#
SRCS = main.c master.c worker.c queue.c
DEPS = job.h  master.h worker.h queue.h
LIBS = -lm
OBJS = $(SRCS:.c=.o)
EXE  = primes

#
# Debug build settings
#
DBGDIR = debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0 -DDEBUG

#
# Release build settings
#
RELDIR = release
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCFLAGS = -O3 -DNDEBUG

.PHONY: all clean debug prep release remake

# Default build
all: prep release

#
# Debug rules
#
debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $(DBGEXE) $^ $(LIBS)

$(DBGDIR)/%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $< $(LIBS)

#
# Release rules
#
release: $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $(RELEXE) $^ $(LIBS)

$(RELDIR)/%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) $(RELCFLAGS) -o $@ $< $(LIBS)

#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)