sources = main.c */*.c 
cc = gcc
ccflags = -Wall

main: $(sources)
	$(cc) -o main $(sources) $(ccflags)

clean:
	rm main ; rm *.bin

debug:
	@ xxd -c 10 -g 1 debug.bin

push:
	git push uds-dev master
	git push origin master
