CFLAGS = -Wall -std=c11 -g
INCLUDES = 
LFLAGS = 
MAIN = tmphome


all: $(MAIN)

$(MAIN): src/$(MAIN).c
	$(CC) $< $(CFLAGS) $(INCLUDES) $(LFLAGS) -o $@

format:
	astyle --fill-empty-lines --indent=tab --indent-switches --suffix=no src/$(MAIN).c

clean:
	$(RM) $(MAIN)

