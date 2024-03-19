sources = main.c BUS/*.c ISOTP/*.c QUEUE/*.c SERVICES/*.c SESSION/*.c UDS/*.c UTILS/*.c
services = SERVICES/**/*.c
test_sources = */*.c **/**/*.c
cc = gcc
ccflags = -Wall

main: $(sources)
	$(cc) -o main $(sources) $(services) $(ccflags)
	$(cc) -o test $(test_sources) $(ccflags)

clean:
	@ rm main ; rm test ; rm *.bin ; rm massif.out*

debug:
	@ od -An -vtx1 -w10 debug.bin

mem_test: main
	valgrind --tool=massif --stacks=yes --time-unit=ms --massif-out-file=massif.out.rx ./main 0x731 0x731 0x000 0 &
	valgrind --tool=massif --stacks=yes --time-unit=ms --massif-out-file=massif.out.tx ./main 0x739 0x739 0x731 1
	ms_print massif.out.rx
	ms_print massif.out.tx

push:
	git push uds-dev master
	git push origin master
