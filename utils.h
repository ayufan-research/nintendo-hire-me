void PrintHex(const char *name, const u8 *inp, int n) {
	printf("u8 %s[] = {\n", name);

	for(int j = 0; j < n; j++) {
		if (j % 8 == 0)
			printf("\t");
		printf("0x%02x, ", inp[j]);
		if (j % 8 == 7)
			printf("\n");
	}

	printf("};\n");
}

void PrintHex32(const char *name, const u32 *inp, int n) {
	printf("u32 %s[] = {\n", name);

	for(int j = 0; j < n; j++) {
		if (j % 8 == 0)
			printf("\t");
		printf("0x%08x, ", inp[j]);
		if (j % 8 == 7)
			printf("\n");
	}

	printf("};\n");
}

u8 CalcDiff(u8 outp[32], u32 diff) {
	u8 inp = 0;

	for(u8 k = 0; k < 32; k++) {
		inp ^= outp[k] * ((diff >> k) & 1);
	}

	return inp;
}
