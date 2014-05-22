CFLAGS = -Wall -std=c11 -g
INCLUDES = 
LFLAGS = 
MAIN = tmphome


all: $(MAIN)

$(MAIN): src/$(MAIN).c
	$(CC) $< $(CFLAGS) $(INCLUDES) $(LFLAGS) -o $@

format:
	astyle --fill-empty-lines --indent=tab --indent-switches --suffix=no src/$(MAIN).c

install:
	cp $(MAIN) /bin/$(MAIN)
	chmod 755 /bin/$(MAIN)

uninstall:
	rm /bin/$(MAIN)

clean:
	$(RM) $(MAIN)

