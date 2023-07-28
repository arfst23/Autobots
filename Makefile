REASON = @echo; if [ -f $@ ]; then echo "[mk $@: $?]"; else echo "[mk $@]"; fi

CFLAGS = -g

all: autobots

.c.o:
	$(REASON)
	$(COMPILE.c) $< $(OUTPUT_OPTION)

autobots: strategy.o autobots.o
	$(REASON)
	$(LINK.c) -o $@ $^ $(LDLIBS)

simulator: strategy.o simulator.o
	$(REASON)
	$(LINK.c) -o $@ $^ $(LDLIBS)

test: simulator
	simulator < test-row-1 > /dev/null
	simulator < test-row-2 > /dev/null
	simulator < test-row-3 > /dev/null
	simulator < test-row-4 > /dev/null
	simulator < test-row-5 > /dev/null
	simulator < test-row-6 > /dev/null
	simulator < test-row-7 > /dev/null
	simulator < test-row-8 > /dev/null

deps depend:
	$(REASON)
	$(CC) -MM $(CFLAGS) *.c > deps

clean:
	$(RM) *.o core err deps tags *~

tags:
	ctags --format=2 -o $@ -R .

-include deps
