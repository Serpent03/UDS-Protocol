sources = main.c */*.c 
cc = gcc
ccflags = -Wall

main: $(sources)
	$(cc) -o main $(sources) $(ccflags)

clean:
	rm main
	rm bus.bin

push:
	git push uds-dev master
	git push origin master
