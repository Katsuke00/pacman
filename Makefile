include config.mk

.SUFFIXES:
.SUFFIXES: .o .c

BIN=src/pacman
SRC=$(BIN:=.c)
OBJ=$(BIN:=.o)

all: $(BIN)

$(BIN): $(@:=.o)
$(OBJ): config.mk

.o:
	$(CC) $(LDFLAGS) -o $@ $< $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

clean:
	rm -f $(BIN) $(OBJ)

.PHONY:
	all clean
