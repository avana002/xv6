#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
//#include "spinlock.c"

int
sys_fork(void)
{
  return fork();
}

void
sys_exit(int status)
{
  exit(status);
  return;  // not reached
}

int
sys_wait(int* status)
{ 
  char* p;
  argptr(0,&p,4);
  int* s = (int*)p;
  return wait(s);
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_waitpid(int pid, int* status, int options)
{
  char* c;
  argptr(1,&c,4);
  int* s = (int*)c;
  int p;
  int o;
  argint(0,&p);
  argint(2,&o);
  return waitpid(p, s, o);
}

void sys_changepr(int pid, int priority)
{
  changepr(pid, priority);
  return; 
}
