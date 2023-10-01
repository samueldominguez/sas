CC=gcc
CFLAGS=-c -O3 -Wno-packed-bitfield-compat -fcommon
LDFLAGS=
SOURCES=sas.c output.c label.c instruction.c directive.c sof.c lex.yy.c yacc.tab.c
Y_SRC=yacc.y
L_SRC=lex.l
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sas

all: $(Y_SRC) $(L_SRC) $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@$(CC) -o $@ $(OBJECTS) $(LDFLAGS)
	@echo "LINK	$@"

.c.o:
	@$(CC) $(CFLAGS) $< -o $@
	@echo "CC	$<"

%.tab.c %.tab.h: %.y
	@bison -d $<
	@echo "BISON	$<"

%.yy.c: %.l yacc.y
	@flex $<
	@echo "FLEX	$<"

clean:
	rm -f lex.yy.c
	rm -f yacc.tab.c
	rm -f yacc.tab.h
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)
