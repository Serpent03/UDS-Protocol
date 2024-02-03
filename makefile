sources = main.c UDS.c ISOTP_CAN.c queue.c
cc = gcc
ccflags = -Wall

main: $(sources)
	$(cc) -o main $(sources) $(ccflags)

clean:
	rm main
	rm GPO.bin
	rm GPI.bin
