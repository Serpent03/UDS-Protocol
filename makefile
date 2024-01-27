sources = main.c UDS.c
cc = gcc
ccflags = -Wall

main: $(sources)
	$(cc) -o main $(sources) $(ccflags)

clean:
	rm main
