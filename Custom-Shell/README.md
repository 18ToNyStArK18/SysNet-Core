# Custom C-Shell

A robust, POSIX-compliant command-line interpreter implemented in C. This shell features a recursive descent parser based on a custom Context-Free Grammar (CFG) to handle complex command chains, piping, and redirection.
## 📂 Project Structure
```text
Custom-Shell/
├── include/
│   └── include.h               # All function declarations and headers
├── src/
│   ├── main.c                  # Entry point and main shell loop
│   ├── Parsing.c               # CFG-based recursive descent parser
│   ├── execute.c               # Command execution engine
│   ├── handlers.c              # Signal handling (Ctrl-C, Ctrl-Z, Ctrl-D)
│   ├── redirection.c           # I/O redirection and piping logic
│   ├── jobs.c                  # Background/foreground job management
│   ├── hop.c                   # Directory navigation built-in
│   ├── reveal.c                # Directory listing built-in
│   ├── log.c                   # Command history management
│   ├── activities.c            # Process status listing
│   └── ping.c                  # Signal sending to processes
├── Makefile                    # Build configuration
├── README.md                   # This file
└── shell.out                   # Compiled executable (after make)
```

## ✨ Features

### Core Functionality
- **Prompt:** Displays `<Username@SystemName:current_path>`. Auto-detects home directory (`~`).
- **Input Parsing:** Handles complex command groups using `&` (background) and `;` (sequential).
- **Process Management:** Supports foreground and background execution with job tracking.

### I/O & Redirection
- **Input Redirection:** `<` (Read from file).
- **Output Redirection:** `>` (Overwrite) and `>>` (Append).
- **Piping:** `|` (Connect stdout of one command to stdin of another).
- **Combined:** Supports chains like `cat file.txt | grep "data" > out.txt &`.

### Built-in Commands (Intrinsics)
- `hop`: Changes directory (supports `.`, `..`, `~`, `-`).
- `reveal`: Lists directory contents. Flags: `-a` (all/hidden), `-l` (long format).
- `log`: Maintains persistent command history (`purge` to clear, `execute <index>` to run).
- `activities`: Lists running and stopped processes spawned by the shell.
- `ping <pid> <sig>`: Sends signals to processes.
- `fg <pid>` / `bg <pid>`: Manages background and stopped jobs.

### Signal Handling
- **Ctrl-C:** Interrupts foreground process.
- **Ctrl-D:** Logout/Exit.

## 🔨 Build & Run

```bash
# Compile the shell
make

# Run the shell
./shell.out
```

## 📝 Usage Examples

### Directory Navigation
```bash
<user@sys:~> hop test_dir
<user@sys:~/test_dir> hop ..
```

### Piping and Redirection
```bash
<user@sys:~> cat file.txt | grep "hello" > result.txt
```

### Background Processing
```bash
<user@sys:~> sleep 10 &
[1] 1234
<user@sys:~> activities
[1234] : sleep - Running
```

## 🏗️ Architecture

The shell is structured around a recursive descent parser that converts user input into an Abstract Syntax Tree (AST) for execution. The parser handles operator precedence and constructs pipelines dynamically.

## 📋 Requirements

- GCC 7.0 or higher
- Linux kernel 4.0+
- Make utility
