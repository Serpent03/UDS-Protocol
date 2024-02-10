sources = main.c UDS/UDS.c ISOTP/ISOTP_CAN.c SERVICER/timing.c SERVICER/servicer.c QUEUE/queue.c
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
