# UDS Protocol in C

Implementation of the UDS Protocol in C.

## Pre-Requisites
- CC/GCC
- GNU make

On linux systems, run the command:
```bash 
sudo apt install make
``` 

## Installation

Clone the repository onto your local machine using git:

```bash
git clone https://github.com/PupilMesh/UDSDev.git
```

To run the implementation, do the following:
- Run `make`
- Run `./main <ADDRESSING RANGE START> <ADDRESSING RANGE END> <TRANSMIT ADDRESS> <TRANSMITTER STATE>`
- Run `make clean` to clean up `bus.bin`, where all the communication takes place.

Addressing range starts from 0x000 to 0x7FF. Either type it in with the `0x` specifier, or without it. The transmitter state lets the program know if it will either transmit or only receive from other sources(as of now). An example command looks like:

`./main 0x731 0x739 0x733 0` where the program will only active when the CAN bus brings messages with arbitration ID/addresses from 0x731 to 0x739, transmits only at 0x733, and will only receive messages and not pre-emptively respond.

#### Testing
To test memory usage using valgrind, run `make mem_test`. The results are printed to console.

## Overview

`/ISOTP` Holds all definitions for an ISOTP packet.

`/UDS` Holds all definitions for functions and objects required to build a UDS packet.

`/QUEUE` Holds all definitions for a circular queue.

`common.h` Holds common definitions and references.

`/SESSION` Holds definitions for the main loop functions.

`/SERVICES` Holds definitions for the processing being done on the HAL/application layer from received UDS packets.

`/UTILS` Holds definitions for common utility features.

## Program Flow

For transmission, the program works as follows:
-  Taking an input to generate a UDS packet, via the `generate_UDS_packet()` function.
- Calling the `send_ISOTP_frames()` function on the generated UDS packet.
- A "buffer" queue is populated with all the data to be sent into the I/O, which is, for now, simulated with FILE I/O.
- CAN frames for either single, first or consecutive frames are developed based on the UDS packet length.

For receiving data, the program workflow is:
- Make a UDS_Packet structure, and pass it to the `receive_ISOTP_frames()` function by reference.
- Data coming in continuously from the I/O(simulated via FILE I/O) decides the length of the UDS packet through various FCI codes specified in the CAN-TP/ISO-TP frame.
- The received UDS packet can be parsed through the `parse()` function, which can then generate a reply to transmitted back.

The overall program flow is:
- In `main.c`, the `Server_Init()` function initializes the server loop.
- `Server_Main()` is called, where the `servicer()` is called every `CLOCK_CYCLE` seconds.
- The `servicer()` deals with all transmissions and packet processing, calling functions defined above.

## TODOs

- Implement actual GPIO operations.
- Flesh out the `parse()` function.
- Remove dependency on OS specific libs like <time.h>
