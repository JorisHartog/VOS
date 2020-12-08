#include "utils.h"

void printf(const char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}
//void printf(const char* str) {
//  static uint16_t* VideoMemory = (uint16_t*)0xb8000;
//  static uint8_t x=0, y=0;
//  const int WIDTH = 80;
//  const int HEIGHT = 25;
//
//  for (int i = 0; str[i] != '\0'; i++) {
//    // Copy each character to the video memory, but retain the high byte which
//    // contains color info.
//    switch(str[i]) {
//      case '\n':
//        x = 0;
//        y++;
//        break;
//      default:
//        VideoMemory[y*WIDTH + x] = (VideoMemory[i] & 0xFF00) | str[i];
//        x++;
//    }
//
//    if (x >= WIDTH) {
//      x = 0;
//      y++;
//    }
//
//    if (y >= HEIGHT) {
//      for (y = 1; y < HEIGHT; y++)
//        for (x = 0; x < WIDTH; x++)
//          VideoMemory[(y-1)*WIDTH + x] = VideoMemory[y*WIDTH + x];
//
//      y = HEIGHT-1;
//      for (x = 0; x < WIDTH; x++)
//        VideoMemory[y*WIDTH + x] =
//          (VideoMemory[y*WIDTH + x] & 0xFF00) | ' ';
//
//      x = 0;
//    }
//  }
//
//}

void assert(bool assertion, const char* msg) {
  if (!assertion) {
    printf("[!!!] Assertion error: ");
    printf(msg);
    asm("sti");
    while(1);
  }
}
