CC=gcc
CFLAGS=-c -g -Wno-packed-bitfield-compat
LDFLAGS=-lfl -ly -g
SOURCES=sas.c output.c label.c instruction.c directive.c ssf.c lex.c yacc.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sas

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

%.yacc.c: %.y
	bison -d $<

%.lex.c: %.l yacc.h
	flex $<

clean:
	rm -f lex.c
	rm -f yacc.c
	rm -f yacc.h
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)
