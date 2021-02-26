LCLIDIR =../include
RMTIDIR =../../../build/lirc-0.9.4c/debian/tmp/usr/include

CC=g++
LCLCFLAGS=$(LCLIDIR:%=-I%)
RMTCFLAGS=$(RMTIDIR:%=-I%)
CFLAGS=$(LCLCFLAGS) $(RMTCFLAGS)

ODIR=obj
LDIR =../lib
_dummy := $(shell mkdir -p obj)

LIBS=-lwiringPi -lpthread -lm ./../../../build/lirc-0.9.4c/debian/tmp/usr/lib/arm-linux-gnueabihf/liblirc_client.so

_DEPS = benq.h comms.h controller.h dali.h easing.h ir.h led_strip.h ynca.h 
DEPS = $(patsubst %,$(LCLIDIR)/%,$(_DEPS))

_OBJ = main.o comms.o controller.o dali.o led_strip.o easing.o ir.o ynca.o benq.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

../../../fhx_manager/fhx_manager: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 