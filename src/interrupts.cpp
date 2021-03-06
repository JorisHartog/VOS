#include "interrupts.h"

void printf(const char* str);

InterruptHandler::InterruptHandler(
    uint8_t interrupt,
    InterruptManager* interruptManager
) {
  this->interrupt = interrupt;
  this->interruptManager = interruptManager;
  interruptManager->handlers[interrupt] = this;
}

InterruptHandler::~InterruptHandler() {
  if (interruptManager->handlers[interrupt] == this)
    interruptManager->handlers[interrupt] = 0;
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp) {
  return esp;
}

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager* InterruptManager::ActiveInterruptManager = 0;

void InterruptManager::SetInterruptDescriptorTableEntry(
    uint8_t interrupt,
    uint16_t CodeSegment,
    void (*handler)(),
    uint8_t DescriptorPrivilegeLevel,
    uint8_t DescriptorType
) {
  const uint8_t IDT_DESC_PRESENT = 0x80;

  interruptDescriptorTable[interrupt].handlerAddressLowBits =
    ((uint32_t)handler) & 0xFFFF;
  interruptDescriptorTable[interrupt].handlerAddressHighBits =
    (((uint32_t)handler) >> 16) & 0xFFFF;
  interruptDescriptorTable[interrupt].gdt_codeSegmentSelector = CodeSegment;
  interruptDescriptorTable[interrupt].access =
    IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel&3) << 5);
  interruptDescriptorTable[interrupt].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
: picPrimaryCommand(0x20),
  picPrimaryData(0x21),
  picSecondaryCommand(0xA0),
  picSecondaryData(0xA1)
{
  uint32_t CodeSegment = gdt->CodeSegmentSelector();
  const uint8_t IDT_INTERRUPT_GATE = 0xE;

  for (uint16_t i = 0; i < 256; i++) {
    handlers[i] = 0;
    SetInterruptDescriptorTableEntry(i, CodeSegment, &InterruptIgnore, 0,
        IDT_INTERRUPT_GATE);
  }

  SetInterruptDescriptorTableEntry(0x20, CodeSegment,
      &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
  SetInterruptDescriptorTableEntry(0x21, CodeSegment,
      &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

  // Let pics know they need to offset their interrupt number to avoid collision
  picPrimaryCommand.Write(0x11);
  picSecondaryCommand.Write(0x11);

  picPrimaryData.Write(0x20);
  picSecondaryData.Write(0x28);

  // Tell primary and secondary pic which role they have
  picPrimaryData.Write(0x04);
  picSecondaryData.Write(0x02);

  picPrimaryData.Write(0x01);
  picSecondaryData.Write(0x01);

  picPrimaryData.Write(0x00);
  picSecondaryData.Write(0x00);

  InterruptDescriptorTablePointer idt;
  idt.size = 256 * sizeof(GateDescriptor) - 1;
  idt.base = (uint32_t)interruptDescriptorTable;
  asm volatile("lidt %0" : : "m" (idt));
}

InterruptManager::~InterruptManager() {}

void InterruptManager::Activate() {
  if (ActiveInterruptManager != 0)
    ActiveInterruptManager->Deactivate();
  ActiveInterruptManager = this;
  asm("sti");
}

void InterruptManager::Deactivate() {
  ActiveInterruptManager = 0;
  asm("cli");
}

uint32_t InterruptManager::HandleInterrupt(
    uint8_t interrupt,
    uint32_t esp
) {
  if (ActiveInterruptManager != 0) {
    return ActiveInterruptManager->DoHandleInterrupt(interrupt, esp);
  }

  return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(
    uint8_t interrupt,
    uint32_t esp
) {
  if (handlers[interrupt] != 0) {
    esp = handlers[interrupt]->HandleInterrupt(esp);
  } else if (interrupt != 0x20) {
    // Unhandled interrupt which is not a timer interrupt
    char* foo = "INTERRUPT 0x00\n";
    char* hex = "0123456789ABCDEF";

    foo[12] = hex[(interrupt >> 4) & 0xF];
    foo[13] = hex[interrupt & 0xF];
    printf(foo);
  }

  if (0x20 <= interrupt && interrupt < 0x30) {
    // Acknowledge hardware interrupt
    picPrimaryCommand.Write(0x20);
    if (0x28 <= interrupt) {
      // Interrupt came from secundary PIC
      picSecondaryCommand.Write(0x20);
    }
  }

  return esp;
}
