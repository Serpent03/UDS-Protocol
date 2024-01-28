sources = main.c UDS.c CAN.c queue.c
cc = gcc
ccflags = -Wall

main: $(sources)
	$(cc) -o main $(sources) $(ccflags)

clean:
	rm main
