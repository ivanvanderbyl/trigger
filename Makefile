include Rules.make

APP = trigger
LIBS = lib/sensor.cpp

all: $(APP)

$(APP): main.cpp
	$(CC) -o bin/$(APP) main.cpp $(LIBS) $(CFLAGS)	
	
clean:
	rm -f *.o ; rm $(APP)
