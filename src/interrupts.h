#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"
#include "procs.h"

class InterruptManager;

class InterruptHandler {
  protected:
    uint8_t interrupt;
    InterruptManager* interruptManager;

    InterruptHandler(InterruptManager* interruptManager, uint8_t interrupt);
    ~InterruptHandler();

  public:
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

class InterruptManager {
  friend class InterruptHandler;
  protected:
    static InterruptManager* ActiveInterruptManager;
    InterruptHandler* handlers[256];
    ProcessManager* processManager;

    struct GateDescriptor {
      uint16_t handlerAddressLowBits;
      uint16_t gdt_codeSegmentSelector;
      uint8_t reserved;
      uint8_t access;
      uint16_t handlerAddressHighBits;
    } __attribute__((packed));

    static GateDescriptor interruptDescriptorTable[256];

    struct InterruptDescriptorTablePointer {
      uint16_t size;
      uint32_t base;
    } __attribute__((packed));

    static void SetInterruptDescriptorTableEntry(
        uint8_t interruptr,
        uint16_t CodeSegment,
        void (*handler)(),
        uint8_t DescriptorPrivilegeLevel,
        uint8_t DescriptorType
    );

    static void InterruptIgnore();
    static void HandleInterruptRequest0x00();
    static void HandleInterruptRequest0x01();

    Port8BitSlow picPrimaryCommand;
    Port8BitSlow picPrimaryData;
    Port8BitSlow picSecondaryCommand;
    Port8BitSlow picSecondaryData;

  public:
    InterruptManager(GlobalDescriptorTable* gdt,ProcessManager* processManager);
    ~InterruptManager();

    void Activate();
    void Deactivate();

    static uint32_t HandleInterrupt(uint8_t interrupt, uint32_t esp);
    uint32_t DoHandleInterrupt(uint8_t interrupt, uint32_t esp);
};

#endif
