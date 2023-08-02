#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <cstring>

template <class T>
void valueToRaw(char **raw, T val) {
  if (raw == nullptr)
    printf("Expect raw not to be nullptr\n");

  size_t cnt = sizeof(T) / sizeof(uint8_t);
  uint64_t tmp_val = static_cast<uint64_t>(val);
  for (size_t i = 0; i < cnt; i++) {
    (*raw)[i] = (tmp_val & 0xff);
    tmp_val >>= 8;
  }
  *raw += cnt;
}

template <class T>
T rawToValue(char **raw) {
  if (raw == nullptr)
    printf("Expect raw not to be nullptr\n");

  size_t cnt = sizeof(T) / sizeof(uint8_t);
  T tmp = 0;

  for (size_t i = 0; i < cnt; i++) {
    size_t shift = i * 8;
    size_t val = (unsigned char) (*raw)[i];
    tmp = (val << shift) | tmp;
  }
  *raw += cnt;
  return tmp;
}

int main() {
    char* ptr;

    uint32_t size = 1000;
    ptr = (char*)malloc(size);


    char *raw = ptr;
    uint32_t num1 = 12;
    uint64_t num2 = 81600;
    uint32_t num3 = 220;
    uint64_t num4 = 1;
    int num5 = -110;

    valueToRaw(&raw, num1);
    valueToRaw(&raw, num2);
    valueToRaw(&raw, num3);
    valueToRaw(&raw, num4);
    valueToRaw(&raw, num5);
     
    raw = ptr;
    printf("num1 = %u\n", rawToValue<uint32_t>(&raw)); 
    printf("num2 = %lu\n", rawToValue<uint64_t>(&raw)); 
    printf("num3 = %u\n", rawToValue<uint32_t>(&raw)); 
    printf("num4 = %lu\n", rawToValue<uint64_t>(&raw)); 
    printf("num5 = %d\n", rawToValue<int>(&raw)); 


    free(ptr);

    return 0;
}

