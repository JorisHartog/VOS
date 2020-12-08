#include "utils.h"
#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "procs.h"
#include "keyboard.h"

void testA() {
  printf("This is testA, I'm done\n");
}

void testB() {
  while(1)
    printf("B");
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
  printf("Written by Joris Hartog\n\n");

  printf("[ * ] Setting up global descriptor table..\n");
  GlobalDescriptorTable gdt;

  printf("[ * ] Setting up process manager..\n");
  ProcessManager processmanager;
  Process A(&gdt, testA);
  //Process B(&gdt, testB);
  processmanager.AddProcess(&A);
  //processmanager.AddProcess(&B);

  printf("[ * ] Setting up interrupt manager..\n");
  InterruptManager interrupts(&gdt, &processmanager);

  printf("[ * ] Setting up drivers..\n");
  KeyboardDriver keyboard(&interrupts);

  printf("[ * ] Activating interrupts..\n");
  interrupts.Activate();

  while(1);
}
