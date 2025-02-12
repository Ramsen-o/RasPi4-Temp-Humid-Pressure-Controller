ghc: ghc.o ghcontrol.o led2472g.o hts221.o lps25h.o
	gcc -g -o ghc ghc.o ghcontrol.o led2472g.o hts221.o lps25h.o -li2c -lcurses
ghc.o: ghc.c ghcontrol.h
	gcc -g -c ghc.c
ghcontrol.o: ghcontrol.c ghcontrol.h lps25h.h hts221.h led2472g.h
	gcc -g -c ghcontrol.c
led2472g.o: led2472g.c led2472g.h
	gcc -g -c led2472g.c
hts221.o: hts221.c hts221.h
	gcc -g -c hts221.c
lps25h.o: lps25h.c lps25h.h
	gcc -g -c lps25h.c
.PHONY: clean
clean:
	rm -f *.o ghc


