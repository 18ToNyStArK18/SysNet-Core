# SysNet Core: Systems & Networking Suite

**SysNet Core** is a comprehensive systems programming repository comprised of three distinct projects: a fully functional POSIX-compliant shell, a reliable transport layer protocol implementation over UDP, and kernel-level scheduling modifications for the xv6 operating system.

## 📂 Project Structure

```text
SysNet-Core/
├── Custom-Shell/               # POSIX-Compliant Command Line Interpreter
├── Reliable-UDP-Protocol/          # S.H.A.M. Reliable Transport Protocol
└── xv6-Scheduler-Implementation/                 # Modified xv6 Kernel
```

## 🚀 Components

### 1. Custom Shell (C-Shell)

A UNIX shell built from scratch capable of process management, I/O redirection, and signal handling. It features a custom Context-Free Grammar (CFG) parser.

**Key Features:** Redirection (`<`, `>>`), Piping (`|`), Background processes (`&`), and job control (`fg`, `bg`).

### 2. S.H.A.M. Networking Protocol

An implementation of S.H.A.M. (Sequenced Header for Asynchronous Messaging). This module simulates TCP-like reliability over unreliable UDP.

**Key Features:** 3-way handshake, Sliding Window flow control, Retransmission Timeouts (RTO), and MD5 data verification.

### 3. xv6 Kernel Modifications

Deep modifications to the educational xv6 operating system to implement advanced scheduling algorithms and system calls.

**Key Features:** getreadcount syscall, FCFS Scheduler, Completely Fair Scheduler (CFS), and Multi-Level Feedback Queue (MLFQ).

## 🛠️ Tech Stack

- **Languages:** C (C99 Standard), RISC-V Assembly
- **Environment:** Linux (Ubuntu/Debian)
- **Tools:** GCC, Make, QEMU, GDB, Wireshark

## 📜 License

Distributed under the MIT License. See LICENSE for more information.
