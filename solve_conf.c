#include <stdio.h>

#include "input.h"
#include "conf_r.h"

void reverse_256() {
  printf("int conf_r[][16] = {\n");
  for (int i = 0; i < 256; i++) {
    printf("\t{");
    for (int j = 0; j < 256; j++) {
      if (confusion[j] == i) {
        printf("0x%02x, ", j);
      }
    }
    printf("-1}, // 0x%02x\n", i);
  }
  printf("};\n");
}

void reverse_64k() {
  printf("int conf_64k_r[][256] = {\n");
  for (int i = 0; i < 256; i++) {
    printf("\t{");

    for (int j = 0; j < 65536; j++) {
      int low = j%256;
      int high = j/256;
      int conf = confusion[low] ^ confusion[high+256];
      if (conf == i && low > 34 && high > 34) {
        printf("0x%04x, ", j);
      }
    }
    printf("-1}, // 0x%02x\n", i);
  }
  printf("};\n");
}

int main() {
  reverse_256();
  reverse_64k();
}
