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
git clone https://github.com/Serpent03/UDS-Protocol.git
```

## Overview

`ISOTP_CAN.h` Holds all the references for functions required to send a CAN packet(8 bytes).

`UDS.h` Holds all references for functions and objects required to build a UDS packet.

`queue.h` Holds all references for a circular queue.

`common.h` Holds common definitions and references.

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

## TODOs

- Implement timing controls.
- Implement robust error handling.
- Flesh out the `parse()` function.
