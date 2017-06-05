#include "util.h"

void itoa_r(int n, char *buff_end) {
  while (n > 0) {
    *buff_end = (char) n % 10 + '0';
    buff_end--;
    n /= 10;
  }
}
