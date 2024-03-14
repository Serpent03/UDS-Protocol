./test e6 28 02 10 03 03 00 00 00 00
sleep 2

# the seed is transmitted by the server
# we now send the key

./test e6 28 02 27 01 00 00 00 00 00
sleep 2
./test e6 28 10 0a 27 02 01 02 03 04
sleep 0.5
./test e6 28 21 05 06 07 08 00 00 00

sleep 5

./test e6 28 02 27 03 00 00 00 00 00
sleep 2
./test e6 28 10 0a 27 04 0b 0c 0d 0e
sleep 0.5
./test e6 28 21 0f 10 11 12 00 00 00
