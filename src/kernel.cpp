#include "types.h"
#include "gdt.h"

void printf(char* str) {
  uint16_t* VideoMemory = (uint16_t*)0xb8000;

  for (int i = 0; str[i] != '\0'; i++) {
    // Copy each character to the video memory, but retain the high byte which
    // contains color info.
    VideoMemory[i] = (VideoMemory[i] & 0xff00) | str[i];
  }
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber) {
  printf("Hi there!\n");

  GlobalDescriptorTable gdt;

  while (1);
}
