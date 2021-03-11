LCLIDIR =../../include
RMTIDIR =../../../build/lirc-0.9.4c/debian/tmp/usr/include

CC=g++
LCLCFLAGS=$(LCLIDIR:%=-I%)
RMTCFLAGS=$(RMTIDIR:%=-I%)
CFLAGS=$(LCLCFLAGS) $(RMTCFLAGS)

ODIR=obj
LDIR =../lib
_dummy := $(shell mkdir -p obj)

LIBS=-lcurl 

_DEPS = ircc.h
DEPS = $(patsubst %,$(LCLIDIR)/%,$(_DEPS))

_OBJ = main.o ../../src/ircc.o ../../src/base64.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

../ircc_test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 