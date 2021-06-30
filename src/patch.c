/***************************************************************************
 *                                                                         *
 *          ###########   ###########   ##########    ##########           *
 *         ############  ############  ############  ############          *
 *         ##            ##            ##   ##   ##  ##        ##          *
 *         ##            ##            ##   ##   ##  ##        ##          *
 *         ###########   ####  ######  ##   ##   ##  ##    ######          *
 *          ###########  ####  #       ##   ##   ##  ##    #    #          *
 *                   ##  ##    ######  ##   ##   ##  ##    #    #          *
 *                   ##  ##    #       ##   ##   ##  ##    #    #          *
 *         ############  ##### ######  ##   ##   ##  ##### ######          *
 *         ###########    ###########  ##   ##   ##   ##########           *
 *                                                                         *
 *            S E C U R E   M O B I L E   N E T W O R K I N G              *
 *                                                                         *
 * This file is part of NexMon.                                            *
 *                                                                         *
 * Copyright (c) 2018 NexMon Team                                          *
 *                                                                         *
 * NexMon is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * NexMon is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with NexMon. If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                         *
 **************************************************************************/

#pragma NEXMON targetregion "patch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>


/*Allows to write a specific function to the BLE HCI commands table */
#define REGISTER_HCI_FUNCTION(opcode,parameters,name) 	__attribute__((at((0xF20A0 + (opcode << 3) - 8), "", CHIP_VER_BCM4375B1_BT, FW_VER_S20))) \
							GenericPatch4(callback_ ## name,name) \
							__attribute__((at((0xF20A0 + (opcode << 3)) - 4, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20))) \
							GenericPatch4(params_  ## name,parameters)


/*
RX mode: 
	- 0x00 (disabled, normal BLE mode)
	- 0x01: Zigbee
	- 0x02: Mosart packet (CRC check)
	- 0x03: Mosart keylogger
	- 0x04: Mosart packet (CRC check + address filtering)
	- 0x05: Enhanced ShockBurst address
	- 0x06: Enhanced ShockBurst payload
	- */
__attribute__((at(0x00210500, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint32_t rx_enabled = 0x00000000;

/*
Frequency offset for RX mode
*/
__attribute__((at(0x00210504, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint32_t rx_frequency = 0x00000000;

/*
TX mode:
	0x00: disabled
	0x01: Zigbee
	0x02/0x03/0x04:  Mosart
	0x05/0x06: Enhanced ShockBurst
*/
__attribute__((at(0x00210508, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint32_t tx_enabled = 0x00000000;

/*
Frequency offset for TX mode
*/
__attribute__((at(0x0021050c, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint32_t tx_frequency = 0x00000000;

/*
Position of the beginning of the mosart frame in the buffer
*/
__attribute__((at(0x00210510, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint8_t start_mosart_frame = 0xFF;

/*
Buffer allowing to send HCI commands from patches
*/
__attribute__((at(0x00210514, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint8_t fake_hci_buffer[40] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

/*
Buffer used to store the demodulator output / modulator input
*/
__attribute__((at(0x21053c, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint8_t raw_buffer[255] =  {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

/*
Buffer used to store the decoded/encoded version of a frame
*/
__attribute__((at(0x21063b, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint8_t buffer_decoded[50] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

/*
Size of the decoded/encoded version of a frame
*/
__attribute__((at(0x210670, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint32_t buffer_decoded_size = 0x00000000;

/*
Flag indicating if a frame has been received
*/
__attribute__((at(0x210678, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint32_t rx_done = 0x00000000;

/*
Correspondance table linking Zigbee symbols to the corresponding MSK representation
*/
__attribute__((at(0x21067c, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint8_t SYMBOL_TO_CHIP_MAPPING[16][4] = {
					{0x03,0xf7,0x3a,0x1b},
					{0x39,0x70,0xaf,0x33},
					{0x9a,0x03,0xf7,0x3a},
					{0xb2,0x39,0x70,0x2f},
					{0x3b,0x9b,0x03,0x77},
					{0xae,0xb3,0x39,0x70},
					{0xf7,0x3a,0x9b,0x03},
					{0x70,0xaf,0xb3,0x39},
					{0xfc,0x08,0xc5,0x64},
					{0xc6,0x8f,0x50,0x4c},
					{0x65,0xfc,0x08,0x45},
					{0x4d,0xc6,0x8f,0x50},
					{0xc4,0x64,0xfc,0x08},
					{0x51,0x4c,0xc6,0x0f},
					{0x08,0xc5,0x64,0x7c},
					{0x8f,0x50,0x4c,0x46},
					};

/*
Preamble (used by Enhanced ShockBurst and Mosart RX mode to filter by address)
*/
__attribute__((at(0x2106c0, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint32_t preamble = 0x00000000;

/*
Boolean indicating if the dongle frames must be transmitted to Host (Mosart only)
*/
__attribute__((at(0x2106c4, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint32_t dongle_mode = 0x00000000;

/*
Integer used to store the first two bytes of the received packet (not in the demodulator output)
*/
__attribute__((at(0x2106c8, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint32_t rx_header = 0x00000000;

/*
Buffer used to check the presence of the patches by the Android app
*/
__attribute__((at(0x2106cc, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint8_t patch_installed[11] = {0x52,0x61,0x64,0x69,0x6f,0x53,0x70,0x6c,0x6f,0x69,0x74};

/*
Function allowing to return the hamming distance between two buffers
*/
__attribute__((at(0x00218700, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
int hamming(uint8_t *demod_buffer,uint8_t *pattern) {
	int count = 0;
	for (int i=0;i<4;i++) {
		for (int j=0;j<8;j++) {
			if (((pattern[i] >> (7-j)) & 0x01) != (((demod_buffer[i] & (i==0 && j==0 ? 0x7F : 0xFF)) >> (7-j)) & 0x01)) {
				count++;
			}
		}
	}
	return count;
}

/*
Function allowing to shift a buffer by a given number of bits
*/
__attribute__((at(0x00218800, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
void shift_buffer(uint8_t *demod_buffer,int size) {
	for (int i=0;i<size;i++) {
		if (i != 0) {
			demod_buffer[i-1]=((demod_buffer[i] & 0x80) >> 7) | demod_buffer[i-1];
		}
		demod_buffer[i] = demod_buffer[i] << 1;
	}
}

/*
Function allowing to convert a channel to the corresponding frequency (finalFreq = 2402MHz + returnedFreq)
*/
__attribute__((at(0x00218900, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
int channelToFrequency(int channel) {
	return 3 + 5 * (channel - 11);
}


/*
Function allowing to convert a GFSK frame to the corresponding Zigbee frame (see WazaBee attack: https://hal.archives-ouvertes.fr/hal-03193299/)
*/
__attribute__((at(0x00218A00, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
void decode(uint8_t *output_buffer, uint32_t *length, uint8_t * packet) {

	for (int i=0;i<50;i++) output_buffer[i] = 0;
	// index of the current Zigbee symbol
	uint32_t index = 0;
	// indicator of current 4 bits position (1 = 4 MSB, 0 = 4 LSB)
	uint32_t part = 1;
	// indicator of start frame delimiter
	uint32_t sfd = 0;
	// Hamming distance
	uint32_t hamming_dist = 0;
	// Thresold Hamming distance
	uint32_t hamming_thresold = 8;

	// Align the buffer with the SFD
	output_buffer[0] |= (0x0F & 0x07);
	hamming_dist = 32;
	while (hamming_dist > hamming_thresold) {
		hamming_dist = hamming(packet,SYMBOL_TO_CHIP_MAPPING[0]);
		if (hamming_dist > hamming_thresold) {
			shift_buffer(packet,255);
		}
	}

	hamming_dist = 0;
	while (hamming_dist <= hamming_thresold) {
		int symbol = -1;
		// Compute the hamming distance for every zigbee symbol
		for (int i=0;i<16;i++) {
			hamming_dist = hamming(packet,SYMBOL_TO_CHIP_MAPPING[i]);
			if (hamming_dist <= hamming_thresold) {
				symbol = i;
				break;
			}
		}

		// If a zigbee symbol has been found ...
		if (symbol != -1) {
			// If it matches the SFD next symbol, start the frame decoding
			if (sfd == 0 && symbol == 10) {
				sfd = 1;
			}
			
			// If we are in the frame decoding state ...
			if (sfd == 1) {
				// Fill the output buffer with the selected symbol
				output_buffer[index] |= (symbol & 0x0F) << 4*part;

				// Select the next 4 bits free space in the output buffer
				part = part == 1 ? 0 : 1;
				if (part == 0) index++;
			}
			// Shift the buffer (31 bits shift)
			for (int i=0;i<32;i++) shift_buffer(packet,255);
		}
	}

	// Export of the effective length of the Zigbee frame
	*length = index;
}

/*
Function allowing to convert a little endian byte to big endian
*/
__attribute__((at(0x00218C00, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint8_t swap_bits(uint8_t byte) {
	return (uint8_t)(((byte & 0x01) << 7) |
			((byte & 0x02) << 5) |
			((byte & 0x04) << 3) |
			((byte & 0x08) << 1) |
			((byte & 0x10) >> 1) |
			((byte & 0x20) >> 3) |
			((byte & 0x40) >> 5) |
			((byte & 0x80) >> 7));	
}

/*
Function allowing to save the RX header in rx_header variable
*/
__attribute__((at(0x00218D00, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
__attribute__((optimize("O0")))
__attribute__((naked))
void get_rx_header() {
	__asm__(
	"ldr r3,[r3]\n\t"
	"ldr r2,=rx_header\n\t"
	"str r3,[r2]\n\t"
	"lsls r2,r3,#0x19\n\t"
	"b 0x79fac\n\t"
	);
}

/* Redirect the flow of scanTaskRxHeaderDone to get_rx_header */
__attribute__((at(0x79FA8, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
BPatch(rx_header_patch, get_rx_header);

/* 
Function allowing to calculate a Mosart CRC
*/
__attribute__((at(0x00218E00, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
unsigned short crc16(uint8_t *ptr, int count)
{
   int  crc;
   uint8_t i;
   crc = 0;
   while (--count >= 0)
   {
      crc = crc ^ (int) *ptr++ << 8;
      i = 8;
      do
      {
         if (crc & 0x8000)
            crc = crc << 1 ^ 0x1021;
         else
            crc = crc << 1;
      } while(--i);
   }
   return (crc);
}

/*
Function allowing to check if a Mosart frame includes a valid CRC
*/
__attribute__((at(0x00218F00, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
int check_frame(uint8_t *frame,uint8_t size,uint8_t *start) {
	int valid = 0;
	*start = 0;
	for (int i=0;i<size-2;i++) {
		if (crc16(frame+i,size-2-i) == ((frame[size-1]<<8) | frame[size-2])) {
			valid = 1;
			*start = i;
			break;
		}
		if (dongle_mode == 1 && frame[i] == 0x11 && frame[i+1] == 0x22) {
			valid = 1;
			*start = i;
		}
	}
	return valid;
}

/*
Function allowing to decode a Mosart frame (keeps only one bits over two + performs dewhitening + identifies postamble position) 
*/
__attribute__((at(0x00219000, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
void decode_mosart(uint8_t *output_buffer, uint32_t *length, uint8_t * packet) {
	uint8_t max = 50;
	uint8_t index = 0;
	while (index < max) {
		output_buffer[index/2] = swap_bits(((packet[index] & 0x40) >> 3) |
				   ((packet[index] & 0x10) >> 2) |
				   ((packet[index] & 0x04) >> 1) |
				   ((packet[index] & 0x01)) |
				   ((packet[index+1] & 0x40) << 1) |
				   ((packet[index+1] & 0x10) << 2) |
				   ((packet[index+1] & 0x04) << 3) |
				   ((packet[index+1] & 0x01) << 4)) ^ 0x5a;
		if (output_buffer[index/2] == 0xa5) {
			break;
		}
		index += 2;
	}
	*length = index/2;
}


/*
Function allowing to encode a Mosart frame (duplicates every bit + performs whitening )
*/
__attribute__((at(0x00219100, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
void encode_mosart(uint8_t *output_buffer,uint8_t *data, uint8_t size) {

	for (int i=0;i<size;i++) {
		uint8_t current = swap_bits(data[i] ^ 0x5A);
		output_buffer[i*2] = swap_bits((((current & 0x01) >> 0) << 7) |
				      (((current & 0x01) >> 0) << 6) |
				      (((current & 0x02) >> 1) << 5) |
      				      (((current & 0x02) >> 1) << 4) |
      				      (((current & 0x04) >> 2) << 3) |
    				      (((current & 0x04) >> 2) << 2) |
      				      (((current & 0x08) >> 3) << 1) |
      				      (((current & 0x08) >> 3) << 0));      				      ;
		output_buffer[i*2+1] = swap_bits((((current & 0x10) >> 4) << 7) |
				      (((current & 0x10) >> 4) << 6) |
				      (((current & 0x20) >> 5) << 5) |
      				      (((current & 0x20) >> 5) << 4) |
      				      (((current & 0x40) >> 6) << 3) |
    				      (((current & 0x40) >> 6) << 2) |
      				      (((current & 0x80) >> 7) << 1) |
      				      (((current & 0x80) >> 7) << 0)); 
	}
}

/* 
This function is not used anymore
*/
__attribute__((at(0x00219200, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
int find_esb_address(uint8_t *buffer, uint8_t size) {
	uint8_t patterna = preamble & 0xFF;
	uint8_t patternb = (preamble & 0xFF00) >> 8;
	uint8_t patternc = (preamble & 0xFF0000) >> 16;
	int found = 0;
	if (patterna != 0 && patternb != 0 && patternc != 0) {
		for (int i=0;i<8*size-24;i++) {
			shift_buffer(buffer,size);
			if (buffer[2] == patterna && buffer[3] == patternb && buffer[4] == patternc) {
				found = 1;
				break;
			}
		}
	}
	return found;
}

/*
This function extracts the payload of an Enhanced ShockBurst frame 
*/
__attribute__((at(0x00219300, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
uint8_t extract_payload_esb(uint8_t *buffer, uint8_t *dest) {
	uint8_t size = buffer[0] >> 2;
	if (size > 0 && size < 30) {
		for (int i=0;i<size;i++) {
			dest[i] = 0xFF & (buffer[1+i] << 1 | buffer[1+i] >> 7);
		}	
	}
	else {
		size = 0;
	}
	return size;
}

/*
Main RX callback : handles every RX mode, applies the different operations allowing to decode / convert the received frames, tranmits to the Host
*/
__attribute__((at(0x00210700, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
__attribute__((optimize("O0")))
__attribute__((naked))
void rx_callback(int* func) {
   	__asm__("push {r4-r11,lr}\n\t");
	if (rx_enabled != 0) {

		if ((*((int*)0x318bac) & 256) && (rx_done == 0)){
			memsetbt(raw_buffer,0,255);
			memcpybt_8(raw_buffer,(void*)(0x372400),255);

			if (rx_enabled == 1) {
				decode(buffer_decoded,&buffer_decoded_size,raw_buffer);
				if (buffer_decoded_size > 0 && buffer_decoded_size < 60) {
					char* buffer = bthci_event_AllocateEventAndFillHeader(buffer_decoded_size+2+2, 0xff, buffer_decoded_size+2);
					buffer[10]  =(uint8_t)rx_enabled;
					buffer[11]  =(uint8_t)rx_frequency;
					memcpybt(&buffer[12],buffer_decoded,buffer_decoded_size);
					// Forge HCI response and send it to host
					bthci_event_AttemptToEnqueueEventToTransport(buffer);
				}
			}
			else if (rx_enabled == 2 ) {
				decode_mosart(buffer_decoded,&buffer_decoded_size,raw_buffer);

				if (check_frame(buffer_decoded,buffer_decoded_size,&start_mosart_frame)) {
					char* buffer = bthci_event_AllocateEventAndFillHeader(buffer_decoded_size-start_mosart_frame+4+2+2, 0xff, buffer_decoded_size-start_mosart_frame+4+2);
					buffer[10]  =(uint8_t)rx_enabled;
					buffer[11]  =(uint8_t)rx_frequency;
					memcpybt(&buffer[12],buffer_decoded+start_mosart_frame-4,buffer_decoded_size-start_mosart_frame+4);
					// Forge HCI response and send it to host
					bthci_event_AttemptToEnqueueEventToTransport(buffer);		
				}
			}
			else if (rx_enabled == 3) {
				decode_mosart(buffer_decoded,&buffer_decoded_size,raw_buffer+1);
				char* buffer = bthci_event_AllocateEventAndFillHeader(1+2+2, 0xff, 1+2);
				buffer[10]  =(uint8_t)rx_enabled;
				buffer[11]  =(uint8_t)rx_frequency;			
				memcpybt(&buffer[12],buffer_decoded,1);
				// Forge HCI response and send it to host
				bthci_event_AttemptToEnqueueEventToTransport(buffer);
				
			}
			else if (rx_enabled == 4) {
				decode_mosart(buffer_decoded,&buffer_decoded_size,raw_buffer);

				if (check_frame(buffer_decoded,buffer_decoded_size,&start_mosart_frame)) {
					if ((buffer_decoded[start_mosart_frame-4+0] == (preamble & 0xFF) && buffer_decoded[start_mosart_frame-4+1] == ((preamble & 0xFF00) >> 8) && buffer_decoded[start_mosart_frame-4+2] == ((preamble & 0xFF0000) >> 16) && buffer_decoded[start_mosart_frame-4+3] == ((preamble & 0xFF000000) >> 24))) {
						char* buffer = bthci_event_AllocateEventAndFillHeader(buffer_decoded_size-start_mosart_frame+4+2+2, 0xff, buffer_decoded_size-start_mosart_frame+4+2);
						buffer[10]  =(uint8_t)rx_enabled;
						buffer[11]  =(uint8_t)rx_frequency;
						memcpybt(&buffer[12],buffer_decoded+start_mosart_frame-4,buffer_decoded_size-start_mosart_frame+4);
						// Forge HCI response and send it to host
						bthci_event_AttemptToEnqueueEventToTransport(buffer);		
					}
				}
			}
			else if (rx_enabled == 5) {
				for (int i=0;i<200;i++) raw_buffer[i] = swap_bits(raw_buffer[i]);

				char* buffer = bthci_event_AllocateEventAndFillHeader(5+2+2, 0xff,5+2);
				buffer[10]  =(uint8_t)rx_enabled;
				buffer[11]  =(uint8_t)rx_frequency;
				buffer[12] = swap_bits((rx_header & 0xFF));
				buffer[13] = swap_bits((rx_header & 0xFF00) >> 8);
				memcpybt(&buffer[14],raw_buffer,3);	
				// Forge HCI response and send it to host
				bthci_event_AttemptToEnqueueEventToTransport(buffer);								
			}
			else if (rx_enabled == 6) {
				for (int i=0;i<200;i++) raw_buffer[i] = swap_bits(raw_buffer[i]);

				buffer_decoded_size = extract_payload_esb(raw_buffer,buffer_decoded);

				char* buffer = bthci_event_AllocateEventAndFillHeader(buffer_decoded_size+2+2, 0xff,buffer_decoded_size+2);
				buffer[10]  =(uint8_t)rx_enabled;
				buffer[11]  =(uint8_t)rx_frequency;
				
				
				memcpybt(&buffer[12],buffer_decoded,buffer_decoded_size);	
				// Forge HCI response and send it to host
				bthci_event_AttemptToEnqueueEventToTransport(buffer);								
			}
			rx_done = 1;
		}
		else {
			rx_done = 0; 
		}
	}
   	__asm__("pop {r4-r11,lr}\n\t"
		"push.w {r4-r8,lr}\n\t"
		"b 0x7C890\n\t"
	);
}
/*
Redirects the flow of extendedScanTaskRxDone to rx_callback
*/
__attribute__((at(0x7C88C, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
BPatch(rx_callback_patch, rx_callback);

/*
Function used to disable the whitening / dewhitening if a RX or a TX is selected
*/
__attribute__((at(0x00210c00, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
__attribute__((optimize("O0")))
__attribute__((naked))
void disable_whitening(int* func) {
	if (rx_enabled != 0 || tx_enabled != 0) {
	   	__asm__("ldr r3,=0x00000000\n\t");
	}
	else {
	   	__asm__("ldr r3,=0x00203150\n\t"
			"ldr r3,[r3]\n\t");
		

	}
   	__asm__("b 0x715B8\n\t");
}
/* 
Redirect the flow of bcsulp_setupWhitening to disable_whitening
*/
__attribute__((at(0x715B4, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
BPatch(disable_whitening_patch, disable_whitening);

/*
Function used to configure the RX preamble and LE 2M physical layer in RX mode according to the selected protocol
*/
__attribute__((at(0x00210d00, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
__attribute__((optimize("O0")))
__attribute__((naked))

void set_preamble(int* func) {
	if (rx_enabled != 0) {
		if (rx_enabled == 1) {
		   	__asm__("ldr r0,=0x9b3af703\n\t"
		   		"b.w 0x90020\n\t");
   		}
   		else if (rx_enabled == 2 || rx_enabled == 4) {
	   		__asm__("ldr r0,=0x33333333\n\t"
		   		"b.w 0x90020\n\t");
   		}
   		else if (rx_enabled == 5) {
   			__asm__("ldr r0,=0x55000000\n\t"
		   		"b.w 0x90020\n\t");
   		}
   		else if (rx_enabled == 3  || rx_enabled == 6) {
	   		__asm__("ldr r0,=preamble\n\t" //3f cc 0c 0c // marche pour les calviers mais pas aligné : 0x300fcf0c
	   			"ldr r0,[r0]\n\t"
		   		"b.w 0x90020\n\t");
   		}   		
	}
	else {
	   	__asm__("cbz r0,le1m_mode\n\t"
			"mov r0,r1\n\t"
			"b.w 0x90020\n\t"
			"le1m_mode:\n\t"
			"b.w 0x90084\n\t");
		

	}
}
/*
Redirects the flow of le2m_setupAdvChannelFor2M to set_preamble
*/
__attribute__((at(0x9007C, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
BPatch(set_preamble_patch, set_preamble);

/*
Function allowing to select the RX frequency
*/
__attribute__((at(0x00210e00, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
__attribute__((optimize("O0")))
__attribute__((naked))

void set_rx_frequency(int* func) {
	__asm__("push {r0-r3}\n\t");
	if (rx_enabled != 0) {
	   	__asm__("ldr r0,=rx_frequency\n\t"
			"ldr r0,[r0]\n\t"
			"ldr r1,=0x3186a4\n\t"
			"orr.w r0,r0,#0x80\n\t"
			"str r0,[r1]\n\t"
			"pop {r0-r3}\n\t"
			"b 0x7A1BA\n\t"
		);
	}
	else {
		__asm__("pop {r0-r3}\n\t"
			"b 0x7A196\n\t");
	}
}
/*
Redirects the flow of _scanTaskLcuCmdCommon to set_rx_frequency
*/
__attribute__((at(0x7A192, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
BPatch(set_rx_frequency_patch, set_rx_frequency);



/*
Function allowing to enable and configure RX mode 
*/
__attribute__((at(0x00210f00, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
void bthci_cmd_start_rx(uint8_t *hci_cmd_buffer) {
	char* buffer = bthci_event_AllocateEventAndFillHeader(4+2, 0xFF, 4);
	
	// save Zigbee RX mode
	rx_enabled = hci_cmd_buffer[12];

	// save Zigbee channel
	// TODO: I should check the interval and trigger an error if needed
	rx_frequency = (rx_enabled == 0x01 ? channelToFrequency(hci_cmd_buffer[13]) : hci_cmd_buffer[13]-2); 

	// Reset fake HCI buffer
	for (int i=0;i<40;i++) fake_hci_buffer[i] = 0;
	fake_hci_buffer[8] =  0x01;
	fake_hci_buffer[9] =  0x42;
	fake_hci_buffer[10] =  0x20;
	fake_hci_buffer[11] =  0x06;	
	fake_hci_buffer[12] = 0x00;
	fake_hci_buffer[13] = 0x00;
	fake_hci_buffer[14] = 0x00;
	fake_hci_buffer[15] = 0x00;
	fake_hci_buffer[16] = 0x00;
	fake_hci_buffer[17] = 0x00;
	bthci_cmd_ble_HandleSet_Extended_Scan_Enable(fake_hci_buffer);
	if (rx_enabled != 0) {
		// Reset fake HCI buffer
		for (int i=0;i<40;i++) fake_hci_buffer[i] = 0;
		fake_hci_buffer[8] =  0x01;
		fake_hci_buffer[9] =  0x41;
		fake_hci_buffer[10] =  0x20;
		fake_hci_buffer[11] =  0x08;		
		fake_hci_buffer[12] = 0x01;
		fake_hci_buffer[13] = 0x00;
		fake_hci_buffer[14] = 0x01;
		fake_hci_buffer[15] = 0x01;
		fake_hci_buffer[16] = 0x99;
		fake_hci_buffer[17] = 0x19;
		fake_hci_buffer[18] = 0x99;
		fake_hci_buffer[19] = 0x19;
		bthci_cmd_ble_HandleSet_Extended_Scan_Parameters(fake_hci_buffer);
		
		// Configure fake HCI buffer to enable or disable LE scan

		
		for (int i=0;i<40;i++) fake_hci_buffer[i] = 0;
		fake_hci_buffer[8] =  0x01;
		fake_hci_buffer[9] =  0x42;
		fake_hci_buffer[10] =  0x20;
		fake_hci_buffer[11] =  0x06;		
		fake_hci_buffer[12] = 0x01;
		fake_hci_buffer[13] = 0x00;
		fake_hci_buffer[14] = 0x00;
		fake_hci_buffer[15] = 0x00;
		fake_hci_buffer[16] = 0x00;
		fake_hci_buffer[17] = 0x00;

		bthci_cmd_ble_HandleSet_Extended_Scan_Enable(fake_hci_buffer);
	}
	if (rx_enabled == 2) {
		dongle_mode = (hci_cmd_buffer[14] == 0 ? 0x00 : 0x01);
	}
	if (rx_enabled == 3) {
		memcpybt(raw_buffer,&hci_cmd_buffer[14],4);
		raw_buffer[4] = 0x70;
		encode_mosart(buffer_decoded,raw_buffer,5);
		preamble = (buffer_decoded[8] << 24) | (buffer_decoded[7] << 16) | (buffer_decoded[6] << 8) | (buffer_decoded[5]);
	}
	else if (rx_enabled == 4) {
		dongle_mode = 0x00;
		preamble = (hci_cmd_buffer[17] << 24) | (hci_cmd_buffer[16] << 16) | ((hci_cmd_buffer[15]) << 8) | hci_cmd_buffer[14];
	}
	else if (rx_enabled == 5) {
		/*
		dongle_mode = (hci_cmd_buffer[14] == 0 ? 0x00 : 0x01);
		preamble =  (hci_cmd_buffer[17] << 16)|(hci_cmd_buffer[16] << 8)| hci_cmd_buffer[15];
		*/
	}
	else if (rx_enabled == 6) {
		memcpybt(raw_buffer,&hci_cmd_buffer[14],5);
		preamble = ((swap_bits(raw_buffer[2]) << 24) |(swap_bits(raw_buffer[1]) << 16) | (swap_bits(raw_buffer[0]) << 8) | (swap_bits(0xAA)));
	}
	// Forge HCI response and send it to host
	buffer[10] = 0x52;
	buffer[11] = 0x58;
	buffer[12] = (uint8_t)rx_enabled;
	buffer[13] = (uint8_t)rx_frequency;
	bthci_event_AttemptToEnqueueEventToTransport(buffer);
}


/* 
bthci_cmd_start_rx is registered as a BLE HCI address (opcode = 0x2060)
Adds the bthci_cmd_start_rx address to the BLE HCI commands table 
*/
REGISTER_HCI_FUNCTION(0x60,0x00080000,bthci_cmd_start_rx);

__attribute__((at(0x214400, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
__attribute__((optimize("O0")))
__attribute__((naked))

/*
This function allows to select the LE 2M physical layer if TX mode is enabled.
*/
void set_tx_le2m(int* func) {

		__asm__(
		"ldr r0,=tx_enabled\n\t"
		"ldr r0,[r0]\n\t"
		"cmp r0,#0\n\t"
		"beq normal_behaviour_txle2m\n\t"
		"ldr r0,=0x9b3af703\n\t"
		"bl 0x90020\n\t"
		"ldr r3,=0x00000000\n\t"
		"ldr r2,=0x000279a5\n\t"
		"str r3,[r2]\n\t"
		"b 0x74DAC\n\t"
		"normal_behaviour_txle2m:\n\t"
		"bl 0x9007C\n\t"
		"b 0x74DAC\n\t");
	
}
/*
Redirects the flow of extendedAdvTaskProgHw to set_tx_le2M
*/
__attribute__((at(0x74DA8, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
BPatch(set_tx_le2m_patch, set_tx_le2m);

/*
Function setting the TX frequency. 
*/
__attribute__((at(0x214500, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
__attribute__((optimize("O0")))
__attribute__((naked))
void set_tx_frequency(int* func) {
	__asm__(
	"ldr r0,=tx_enabled\n\t"
	"ldr r0,[r0]\n\t"
	"cmp r0,#0\n\t"
	"beq normal_behaviour_txfreq\n\t"
	"ldr r2,=tx_frequency\n\t"
	"ldr r2,[r2]\n\t"
	"b 0x74F96\n\t"
	"normal_behaviour_txfreq:\n\t"
	"ldr.w r2,[r4,#0x288]\n\t"
	"b 0x74F96\n\t");
	
}

/*
Redirects the flow of extendedAdvTaskProgHW to set_tx_frequency
*/
__attribute__((at(0x74F92, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
BPatch(set_tx_frequency_patch, set_tx_frequency);

/*
Function allowing to enable and configure TX mode.
*/
__attribute__((at(0x214600, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
__attribute__((optimize("O0")))
void bthci_cmd_start_tx(uint8_t *hci_cmd_buffer) {
	
	tx_enabled = 0x01;
	tx_frequency = (hci_cmd_buffer[12] == 0x01 ? channelToFrequency(hci_cmd_buffer[13]) : hci_cmd_buffer[13]-2);


	buffer_decoded_size = hci_cmd_buffer[14];
	if (hci_cmd_buffer[12] == 0x01) {

		for (int i=0;i<4;i++) {
			buffer_decoded[i] = 0;
		}
		// Save decoded packet in RAM
		memcpybt(&buffer_decoded[4],&hci_cmd_buffer[15],buffer_decoded_size);
		
		// Generate GMSK representation of Zigbee packet
		for (int i=0;i<255;i++) raw_buffer[i] = 0;
		for (int i=0;i<buffer_decoded_size+4;i++) {
			uint8_t msb = buffer_decoded[i] >> 4;
			uint8_t lsb = buffer_decoded[i] & 0x0F;

			raw_buffer[i*8+0] = (SYMBOL_TO_CHIP_MAPPING[lsb][0]);
			raw_buffer[i*8+1] = (SYMBOL_TO_CHIP_MAPPING[lsb][1]);
			raw_buffer[i*8+2] = (SYMBOL_TO_CHIP_MAPPING[lsb][2]);
			raw_buffer[i*8+3] = (SYMBOL_TO_CHIP_MAPPING[lsb][3]);

			raw_buffer[i*8+4] = (SYMBOL_TO_CHIP_MAPPING[msb][0]);
			raw_buffer[i*8+5] = (SYMBOL_TO_CHIP_MAPPING[msb][1]);
			raw_buffer[i*8+6] = (SYMBOL_TO_CHIP_MAPPING[msb][2]);
			raw_buffer[i*8+7] = (SYMBOL_TO_CHIP_MAPPING[msb][3]);
		}
	}
	else if (hci_cmd_buffer[12] == 0x02 || hci_cmd_buffer[12] == 0x03 || hci_cmd_buffer[12] == 0x04 ) {
		memcpybt(buffer_decoded,&hci_cmd_buffer[15],buffer_decoded_size);
		encode_mosart(raw_buffer,buffer_decoded,buffer_decoded_size);
	}
	else if (hci_cmd_buffer[12] == 0x05 || hci_cmd_buffer[12] == 0x06 ) {
		memcpybt(buffer_decoded,&hci_cmd_buffer[15],buffer_decoded_size);
		for (int i=0;i<buffer_decoded_size;i++) raw_buffer[i] = swap_bits(buffer_decoded[i]);
	}	
	*((int*)(0x232138)) = 0x000005d0;
	*((int*)(0x232140)) = 0x00000005;
	*((int*)(0x232144)) = 0x00000010;

	// Copy packet as adv data
	if (hci_cmd_buffer[12] == 0x01) {
		memcpybt((void*)(0x232ec0),raw_buffer,(buffer_decoded_size+4)*8);
		*((int*)(0x2332c0)) = (buffer_decoded_size+4)*8;
	}
	else if (hci_cmd_buffer[12] < 0x05) {
		memcpybt((void*)(0x232ec0),raw_buffer,buffer_decoded_size*2);
		*((int*)(0x2332c0)) = buffer_decoded_size*2;	
	}
	else {
		memcpybt((void*)(0x232ec0),raw_buffer,buffer_decoded_size);
		*((int*)(0x2332c0)) = buffer_decoded_size;	
	
	}
	for (int i=0;i<40;i++) fake_hci_buffer[i] = 0;
		
	// Configure fake HCI buffer to enable LE adv
	fake_hci_buffer[8] =  0x01;
	fake_hci_buffer[9] =  0x39;
	fake_hci_buffer[10] =  0x20;
	fake_hci_buffer[11] =  0x06;
	fake_hci_buffer[12] =  0x01;
	fake_hci_buffer[13] =  0x01;
	fake_hci_buffer[14] =  0x00;
	fake_hci_buffer[15] =  0x00;
	fake_hci_buffer[16] =  0x00;
	fake_hci_buffer[17] =  0x00;	
	bthci_cmd_ble_HandleSet_Extended_Advertising_Enable(fake_hci_buffer);

}


/*
bthci_cmd_start_tx is registered as a BLE HCI address (opcode = 0x2061)
Adds the bthci_cmd_start_tx address to the BLE HCI commands table 
*/
REGISTER_HCI_FUNCTION(0x61,0x00080000,bthci_cmd_start_tx);

__attribute__((at(0x214F00, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
__attribute__((optimize("O0")))
__attribute__((naked))

/*
This function is called when a packet has been transmitted.
It transmits a notification to Host indicating that the transmission was successful and disables the advertising mode.
*/
void stop_tx(int* func) {
	__asm__("push {r0-r11,lr}\n\t");
	if (tx_enabled) {

		char* buffer = bthci_event_AllocateEventAndFillHeader(4+2, 0xff, 4);
		// Forge HCI response and send it to host
		for (int i=0;i<40;i++) fake_hci_buffer[i] = 0;
			
		// Configure fake HCI buffer to disable LE adv
		fake_hci_buffer[8] =  0x01;
		fake_hci_buffer[9] =  0x3C;
		fake_hci_buffer[10] =  0x20;
		fake_hci_buffer[11] =  0x01;			
		fake_hci_buffer[12] =  0x00;		
		bthci_cmd_ble_HandleRemove_Advertising_Set(fake_hci_buffer);
		tx_enabled = 0x0;
		

		buffer[10] = 0x54;
		buffer[11] = 0x58;
		buffer[12] = 0x01;
		buffer[13] = (uint8_t)tx_frequency;
		bthci_event_AttemptToEnqueueEventToTransport(buffer);
	}
	__asm__("pop {r0-r11,lr}\n\t"
		"ldr r3,[r4]\n\t"
		"cmp r2,r3\n\t"
		"b 0x71e30\n\t");
}
/* 
Redirects the flow of the advertising process (after the packet transmission) to stop_tx
*/
__attribute__((at(0x71e2c, "", CHIP_VER_BCM4375B1_BT, FW_VER_S20)))
BPatch(stop_tx_patch, stop_tx);

