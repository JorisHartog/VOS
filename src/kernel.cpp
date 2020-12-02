void printf(char* str) {
  unsigned short* VideoMemory = (unsigned short*)0xb8000;

  for (int i = 0; str[i] != '\0'; i++) {
    // Copy each character to the video memory, but retain the high byte which
    // contains color info.
    VideoMemory[i] = (VideoMemory[i] & 0xff00) | str[i];
  }
}

extern "C" void kernelMain(void* multiboot_structure, unsigned int magicnumber) {
  printf("Hi there!\n");
  while (1);
}
