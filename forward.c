
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "utils.h"

#define N 32

void ForwardRound(u8 inp[32], u8 outp[32]) {
	u8 temp[32];

	for(u8 j = 0; j < 32; j++) {
		temp[j] = confusion[inp[j]];
	}

	for(u8 j = 0; j < 32; j++) {
		outp[j] = CalcDiff(temp, diffusion[j]);
	}
}

void ForwardFinal(u8 inp[32], u8 outp[16]) {
	for(u8 i = 0; i < 16; i++) {
		outp[i] = confusion[inp[i * 2]] ^ confusion[inp[i * 2 + 1] + 256];
	}
}

void Forward(u8 inp[32], u8 outp[16]) {
	for(u32 i = 0; i < 256; i++) {
		char name[256];
		sprintf(name, "round_%d", i);
		PrintHex(name, inp, 32);
		ForwardRound(inp, inp);
	}

	PrintHex("before_final", inp, 32);
	ForwardFinal(inp, outp);
}

int main(int argc, char* argv[])
{
	u8 target[] = "Hire me!!!!!!!!";
	u8 output[16];

	u8 resolved[] = {
		0xb7, 0xbc, 0x56, 0xe9, 0x6d, 0xf9, 0x83, 0xc1, 
		0xa3, 0xfe, 0x02, 0x05, 0x2d, 0x2c, 0x23, 0xa4, 
		0x66, 0xca, 0x6c, 0xb2, 0x0e, 0x2b, 0x3c, 0xe1, 
		0xee, 0x04, 0x35, 0x67, 0xb9, 0x21, 0xf2, 0xdc, 
	};

	Forward(resolved, output);

  printf("%*s\n", (int)sizeof(output), output);
	
	return memcmp(output, target, sizeof(output)); // => contact jobs(at)nerd.nintendo.com
}
