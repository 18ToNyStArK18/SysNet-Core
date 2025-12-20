# xv6 Kernel Modifications

This directory contains modified source code for the **xv6 operating system** (RISC-V version). The kernel has been enhanced with new system calls and advanced CPU scheduling algorithms to improve process management and fairness.

## 📂 Project Structure

```text
xv6-Scheduler-Implementation/
├── my_xv6.zip                  # Complete modified xv6 source code
├── readcount.c                 # User program demonstrating getreadcount syscall
├── README.md                   # This file
├── report.md                   # Performance analysis and benchmarking results
└── xv6_modifications.patch     # Patch file containing all kernel modifications
```

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

## 🛠️ Build & Run

### Extract and Setup
```bash
# Extract the xv6 source code
unzip my_xv6.zip
cd xv6-riscv
```

### Apply Modifications (Optional)
If you want to apply the modifications manually:
```bash
# From the xv6-Scheduler-Implementation directory
patch -p1 < xv6_modifications.patch
```

### Compile and Run

To compile and run QEMU with a specific scheduler, use the `SCHEDULER` flag.

**Run FCFS:**
```bash
make clean
make qemu SCHEDULER=FCFS
```

**Run CFS:**
```bash
make clean
make qemu SCHEDULER=CFS
```

**Run Default (Round Robin):**
```bash
make clean
make qemu
```



## 🧪 Testing

User-space test programs included in the xv6 source:
- **`readcount`**: Validates the `getreadcount` system call (source provided as `readcount.c` in this directory).
- **`schedulertest`**: Benchmarks scheduler performance with CPU-bound and I/O-bound workloads.

### Running Tests
Once inside the xv6 shell (after `make qemu`):
```bash
$ readcount
$ schedulertest
```

## 📋 Requirements

- RISC-V toolchain (riscv64-unknown-elf-gcc)
- QEMU with RISC-V support (qemu-system-riscv64)
- Make utility

## 🔍 Key Files Modified

Modified files are included in `my_xv6.zip` and documented in `xv6_modifications.patch`:
- `kernel/proc.c`: Scheduler implementations (FCFS, CFS, MLFQ)
- `kernel/syscall.c`: System call registration for getreadcount
- `kernel/sysproc.c`: `getreadcount` implementation
- `kernel/proc.h`: Process structure modifications (creation time, vruntime, priority)
- `user/readcount.c`: Test program for new syscall (also provided separately)
- `user/schedulertest.c`: Scheduler performance benchmarking tool
- `Makefile`: Added scheduler compilation flags
