EXEC=tristramd
SRC= \
	$(wildcard src/*.c) \
	$(wildcard src/bnetd/*.c) \
	$(wildcard src/d2gs/*.c) \
	$(wildcard src/realm/*.c)

OBJ= $(SRC:.c=.o)

TESTSRC= \
	tests/test_char.c \
	src/account.c  src/bnftp.c  src/file.c  src/hashing.c src/net.c  src/packet.c  src/protocol.c  src/selector.c  src/sql.c  src/sql_schema.c  src/tristram.c  src/utils.c src/character.c \
	$(wildcard src/bnetd/*.c) \
	$(wildcard src/d2gs/*.c) \
	$(wildcard src/realm/*.c)

TESTOBJ = $(TESTSRC:.c=.o)

CFLAGS=-Isrc `pkg-config --cflags eina libconfig` `mysql_config --cflags` -g -ggdb3 -W -Wall -Wextra
LDFLAGS=`pkg-config --libs eina libconfig` `mysql_config --libs`


all: $(EXEC)

tristramd: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)
	@echo "[LD] Compiling $@"

test: $(TESTOBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)
	@echo "[LD] Compiling $@"

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< 
	@echo "[CC] Compiling $<"

.PHONY: clean mrproper

clean:
	@rm -rf *.o \
		*~ etc/*~ \
		tests/*~ tests/*.o \
		src/*.o *~ src/*~ \
		src/bnetd/*.o src/bnetd/*~ \
		src/realm/*.o src/realm/*~ \
		src/d2gs/*.o src/d2gs/*~

mrproper: clean
	@rm -rf test
	@rm -rf $(EXEC)
