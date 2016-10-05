#include "CapreraUtilities.h"

char* int2BitString(char* buffer, int a, int buf_size) {
  if (buf_size > 0) {
    buffer[buf_size - 1] = '\0';
  
    for (int i = buf_size - 2; i >= 0; i--) {
      buffer[i] = (a & 1) + '0';
  
      a >>= 1;
    }
  }

  return buffer;
}

int bitString2Int(char* buffer, int buf_size) {
  int a = 0;
  
  if (buf_size > 0) {
    for (int i = 0; i < buf_size - 1; i++) {
      a <<= 1;
      a |= (buffer[i] & 1);
    }
  }
  
  return a;
}

