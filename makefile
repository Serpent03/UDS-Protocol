sources = main.c */*.c 
cc = gcc
ccflags = -Wall

main: $(sources)
	$(cc) -o main $(sources) $(ccflags)

clean:
	@ rm main ; rm *.bin ; rm massif.out.*

debug:
	@ xxd -c 10 -g 1 debug.bin

mem_test: main
	valgrind --tool=massif --stacks=yes --time-unit=ms --massif-out-file=massif.out.rx ./main 0x731 0x731 0x000 0 &
	valgrind --tool=massif --stacks=yes --time-unit=ms --massif-out-file=massif.out.tx ./main 0x739 0x739 0x731 1
	ms_print massif.out.rx
	ms_print massif.out.tx

push:
	git push uds-dev master
	git push origin master
