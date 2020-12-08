#ifndef __PROCESSMANAGER_H
#define __PROCESSMANAGER_H

#include "types.h"
#include "port.h"
#include "gdt.h"
#include "utils.h"

struct CPUState
{
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;

  uint32_t esi;
  uint32_t edi;
  uint32_t ebp;

  /*uint32_t gs;
  uint32_t fs;
  uint32_t es;
  uint32_t ds;*/

  uint32_t error;

  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
  uint32_t esp;
  uint32_t ss;
} __attribute__((packed));


class Process
{
  friend class ProcessManager;

  private:
    uint8_t stack[4096];
    CPUState* cpustate;

  public:
    Process(GlobalDescriptorTable *gdt, void entrypoint());
    ~Process();
};


class ProcessManager
{
  private:
    Process* processes[256];
    int numProcesses;
    int currentProcess;

  public:
    ProcessManager();
    ~ProcessManager();
    bool AddProcess(Process* process);
    CPUState* Schedule(CPUState* cpustate);
};

#endif
