
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "utils.h"
#include "conf_r.h"
#include "test_data.h"

#define N 32

int BackwardRoundConf(u8 inp[32], u8 outp[32], int j, int (*fn)(u8 outp[32], void *data), void *data) {
	for(; j < 32; j++) {
		int *idx = conf_r[inp[j]];
		if (idx[0] < 0) {
			//printf("fatal error: missing round conf %d at %d\n", inp[j], j);
			return 0;
		}

		for (int k = 1; idx[k] >= 0; k++) {
			outp[j] = idx[k];

			int ret = BackwardRoundConf(inp, outp, j+1, fn, data);
			if (ret) {
				return ret;
			}
		}

		outp[j] = idx[0];
	}

	return fn ? fn(outp, data) : 1;
}

int BackwardRound(u8 inp[32], u8 outp[32], int (*fn)(u8 outp[32], void *data), void *data) {
	u8 temp[32];

	for(u8 j = 0; j < 32; j++) {
		temp[j] = CalcDiff(inp, diffusion[j]);
	}

	return BackwardRoundConf(temp, outp, 0, fn, data);
}

int BackwardFinalConf(u8 inp[16], u8 outp[32], int j, int (*fn)(u8 outp[32], void *data), void *data) {
	for(; j < 16; j++) {
		int *idx = conf_64k_r[inp[j]];
		if (idx[0] < 0) {
			printf("fatal error: missing final conf %d, at %d\n", inp[j], j);
			return 0;
		}

		for (int k = 1; idx[k] >= 0; k++) {
			outp[j*2] = idx[k]%256;
			outp[j*2+1] = idx[k]/256;
			int ret = BackwardFinalConf(inp, outp, j+1, fn, data);
			if (ret) {
				return ret;
			}
		}

		outp[j*2] = idx[0]%256;
		outp[j*2+1] = idx[0]/256;
	}

	return fn ? fn(outp, data) : 1;
}

int BackwardFinal(u8 inp[16], u8 outp[32], int (*fn)(u8 outp[32], void *data), void *data) {
	return BackwardFinalConf(inp, outp, 0, fn, data);
}

struct data_type {
	u8 *intp;
	int round;
};

int FnReverseRoundForward(u8 outp[32], struct data_type *data) {
	struct data_type data2 = {
		.intp = data->intp,
		.round = data->round-1
	};

	if (data2.round > 0) {
		u8 temp[32] = {0};
		memcpy(temp, outp, 32);
		int ret = BackwardRound(outp, temp, FnReverseRoundForward, &data2);
		if (ret) {
			memcpy(outp, temp, 32);
		}
		return ret;
	}

	return 1;
}

int FnReverseFinalForward(u8 outp[32], void *r0) {
	struct data_type data = {.round = 256};

	u8 temp[32] = {0};

	int ret = BackwardRound(outp, temp, FnReverseRoundForward, &data);
	if (ret) {
		memcpy(outp, temp, 32);
	}
	return ret;
}

int main(int argc, char* argv[])
{
	u8 resolved[32] = {0};
	u8 target[] = "Hire me!!!!!!!!";

	int ret = BackwardFinal(target, resolved, FnReverseFinalForward, NULL);
	printf("reverse final=%d\n", ret);
	if (ret) {
		PrintHex("resolved", resolved, 32);
	}
}
