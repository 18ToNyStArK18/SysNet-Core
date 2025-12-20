# xv6 Kernel Modifications

This directory contains modified source code for the **xv6 operating system** (RISC-V version). The kernel has been enhanced with new system calls and advanced CPU scheduling algorithms to improve process management and fairness.

## 🖥️ System Calls Implemented

### `getreadcount`
A system call that tracks the total number of bytes read by the `read()` syscall across all processes since system boot.

- **Usage:** `int count = getreadcount();`
- **User Program:** `readcount.c` demonstrates this functionality.

## ⏱️ Scheduling Algorithms

The default Round-Robin scheduler has been replaced/augmented with the following policies, selectable at compile time.

### 1. FCFS (First-Come, First-Served)
- **Logic:** Non-preemptive. Selects the process with the earliest creation time.
- **Implementation:** Modified `struct proc` to track creation time.

### 2. CFS (Completely Fair Scheduler)
- **Logic:** Models a "virtual runtime" (`vruntime`) for each process. The process with the lowest `vruntime` is selected.
- **Features:**
  - **Process Weights:** Supports priorities (nice values) which decay the `vruntime` accumulation rate.
  - **Time Slices:** Dynamic time slices based on target latency and number of runnable processes.

### 3. MLFQ (Multi-Level Feedback Queue) - *Bonus*
- **Logic:** Preemptive priority scheduling with 4 priority queues.
- **Features:**
  - **Aging:** Prevents starvation by boosting lower-priority processes periodically.
  - **Feedback:** Processes using their full time slice move down in priority; I/O bound processes stay high.

## 🛠️ Build & Run

To compile and run QEMU with a specific scheduler, use the `SCHEDULER` flag.

### Run FCFS
```bash
make clean
make qemu SCHEDULER=FCFS
```

### Run CFS
```bash
make clean
make qemu SCHEDULER=CFS
```

### Run Default (Round Robin)
```bash
make clean
make qemu
```


## 🧪 Testing

User-space test programs included:
- **`readcount`**: Validates the `getreadcount` system call.
- **`schedulertest`**: Benchmarks scheduler performance with CPU-bound and I/O-bound workloads.

## 📋 Requirements

- RISC-V toolchain (riscv64-unknown-elf-gcc)
- QEMU with RISC-V support (qemu-system-riscv64)
- Make utility

## 🔍 Key Files Modified

- `kernel/proc.c`: Scheduler implementations
- `kernel/syscall.c`: System call registration
- `kernel/sysproc.c`: `getreadcount` implementation
- `kernel/proc.h`: Process structure modifications
- `user/readcount.c`: Test program for new syscall
- `user/schedulertest.c`: Scheduler performance benchmarking
