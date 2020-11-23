#include <stdlib.h>
#include <stdio.h>

#include "input.h"
#include "utils.h"

#define BIT(i) (1<<i)

#define N 32

void PrintDiff1(u8 i, u32 diff) {
	fprintf(stderr, "Y[%d] = x", i);
	for(u8 j = 0; j < N; j++) {
		if ((diffusion[i] >> j) & 1) {
			fprintf(stderr, " %d", j);
		}
	}
	fprintf(stderr, "\n");
}

void PrintDiff() {
	for(u32 i = 0; i < N; i++) {
		PrintDiff1(i, diffusion[i]);
	}
}

void PrintRound(int k, u32 *x, u32 *y) {
	fprintf(stderr, "round %d...\n", k);
	for (u32 i = 0; i < N; i++) {
		fprintf(stderr, "X[%d] = 0x%08x; Y[%d] = 0x%08x;\n", i, x[i], i, y[i]);
	}
}

void PrintSolve() {
	u32 solve_x[N] = {0}, solve_y[N] = {0};
	u32 *X = solve_x, *Y = solve_y;

	for (u32 i = 0; i < N; i++) {
		solve_x[i] = diffusion[i];

		if (solve_x[i] & BIT(i)) {
			solve_y[i] |= BIT(i);
			solve_x[i] &= ~BIT(i);
		}
	}

	PrintRound(-1, solve_x, solve_y);

	#define SOLVE(i,j) do{ solve_x[i] ^= BIT(j); solve_x[i] ^= solve_x[j]; solve_y[i] ^= solve_y[j]; } while(0)

	for (u32 k = 0; k < 100; k++) {
		for (u32 i = 0; i < N; i++) {
			if (solve_x[i] == 0)
				continue;

			for (u32 j = 0; j < N; j++) {
				// check if we need the given bit
				if (~solve_x[i] & BIT(j))
					continue;

				// if another one depends on our bit, ignore it
				if (solve_x[j] & BIT(i))
					continue;

				fprintf(stderr, "subs y%d, with x%d\n", i, j);

				SOLVE(i, j);
			}
		}
	
		PrintRound(k, solve_x, solve_y);
	}

	SOLVE(0, 2);
	SOLVE(0, 10);
	SOLVE(2, 0);
	SOLVE(10, 0);

	PrintRound(1000, solve_x, solve_y);

	PrintHex32("diffusion_r", solve_y, N);
}

int main() {
  PrintSolve();
}
