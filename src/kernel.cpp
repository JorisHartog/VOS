#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"

void printf(const char* str) {
  static uint16_t* VideoMemory = (uint16_t*)0xb8000;
  static uint8_t x=0, y=0;

  for (int i = 0; str[i] != '\0'; i++) {
    // Copy each character to the video memory, but retain the high byte which
    // contains color info.
    switch(str[i]) {
      case '\n':
        x = 0;
        y++;
        break;
      default:
        VideoMemory[y*80 + x] = (VideoMemory[i] & 0xFF00) | str[i];
        x++;
    }
  }

  if (x >= 80) {
    x = 0;
    y++;
  }

  if (y >= 25) {
    for (y = 1; y < 25; y++)
      for (x = 0; x < 80; x++)
        VideoMemory[(y-1)*80 + x] = VideoMemory[y*80 + x];

    for (x = 0; x < 80; x++)
      VideoMemory[24*80 + x] = (VideoMemory[24*80 + x] & 0xFF00) | ' ';

    x = 0;
    y = 24;
  }
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber) {
  printf("                     .-'''-.       \n");
  printf("                    '   _    \\     \n");
  printf(" .----.     .----./   /` '.   \\    \n");
  printf("  \\    \\   /    /.   |     \\  '    \n");
  printf("   '   '. /'   / |   '      |  '   \n");
  printf("   |    |'    /  \\    \\     / /    \n");
  printf("   |    ||    |   `.   ` ..' / _   \n");
  printf("   '.   `'   .'      '-...-'`.' |  \n");
  printf("    \\        /              .   | /\n");
  printf("     \\      /             .'.'| |//\n");
  printf("      '----'            .'.'.-'  / \n");
  printf("                        .'   \\_.'  \n");
  printf("Written by Joris Hartog\n");

  GlobalDescriptorTable gdt;
  InterruptManager interrupts(&gdt);

  KeyboardDriver keyboard(&interrupts);

  interrupts.Activate();

  while (1);
}
