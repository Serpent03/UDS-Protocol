sources = main.c UDS.c ISOTP_CAN.c timing.c servicer.c queue.c
cc = gcc
ccflags = -Wall

main: $(sources)
	$(cc) -o main $(sources) $(ccflags)

clean:
	rm main
	rm GPO.bin
	rm GPI.bin

push:
	git push uds-dev master
	git push origin master
