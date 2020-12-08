#include "procs.h"

Process::Process(GlobalDescriptorTable *gdt, void entrypoint()) {
  cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));
  cpustate->eax = 0;
  cpustate->ebx = 0;
  cpustate->ecx = 0;
  cpustate->edx = 0;

  cpustate->esi = 0;
  cpustate->edi = 0;
  cpustate->ebp = 0;

  /*cpustate->gs = 0;
  cpustate->fs = 0;
  cpustate->es = 0;
  cpustate->ds = 0;*/

  //cpustate->error = 0;

  cpustate->eip = (uint32_t)entrypoint;
  cpustate->cs = gdt->CodeSegmentSelector();
  cpustate->eflags = 0x202;
  //cpustate->esp = 0;
  //cpustate->ss = 0;
}

ProcessManager::ProcessManager() {
  numProcesses = 0;
  currentProcess = -1;
}

ProcessManager::~ProcessManager() {}

bool ProcessManager::AddProcess(Process* process) {
  if (numProcesses > 255)
    return false;

  processes[numProcesses++] = process;
  return true;
}

CPUState* ProcessManager::Schedule(CPUState* cpustate) {
  assert(numProcesses > 0, "No running processes!");

  // Update CPU state of current process
  if (currentProcess >= 0)
    processes[currentProcess]->cpustate = cpustate;

  // Switch to next process
  ++currentProcess %= numProcesses;

  return processes[currentProcess]->cpustate;
}
