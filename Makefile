CC=gcc
CFLAGS=-c -g -Wno-packed-bitfield-compat
LDFLAGS=-lfl -ly -g
SOURCES=sas.c output.c label.c instruction.c directive.c sof.c lex.yy.c yacc.tab.c
Y_SRC=yacc.y
L_SRC=lex.l
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sas

all: $(Y_SRC) $(L_SRC) $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

%.tab.c %.tab.h: %.y
	bison -d $<

%.yy.c: %.l yacc.y
	flex $<

clean:
	rm -f lex.yy.c
	rm -f yacc.tab.c
	rm -f yacc.tab.h
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)
