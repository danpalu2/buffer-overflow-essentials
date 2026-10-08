# Buffer Overflow Essentials

## Overview
**Buffer Overflow Essentials** is a structured, hands-on educational repository designed to teach the fundamentals of binary exploitation, memory corruption vulnerabilities, and low-level system security. 

Each module in this repository focuses on a specific vulnerability class, complete with vulnerable source code, exploit generation scripts, compilation instructions, and comprehensive technical documentation.

---

## Repository Structure

```text
buffer-overflow-essentials/
├── README.md                      # Root documentation (this file)
├── 01-variable-overwrite/         # Module 01: Stack variable overwriting via gets()
└── 02-control-flow-hijacking/     # Module 02: Ret2Win & control flow hijacking (RIP overwrite)
```

---

## Available Modules

| Module | Topic | Description | Status |
| :--- | :--- | :--- | :--- |
| **[01-variable-overwrite](./01-variable-overwrite/)** | Stack Variable Overwrite | Overwriting adjacent stack variables using an unsafe `gets()` call to alter program logic and bypass access checks. | ✅ Completed |
| **[02-control-flow-hijacking](./02-control-flow-hijacking/)** | Ret2Win / RIP Overwrite | Corrupting the Saved Return Pointer (RIP) to redirect execution flow into an uncalled target function. | ✅ Completed |

---

## General Requirements & Environment
* **Operating System:** Linux (Ubuntu / Debian recommended)
* **Compiler:** `gcc`
* **Debugger:** GNU Debugger (`gdb`)
* **Scripting Language:** Python 3

### Standard Compilation Flags
To ensure predictable stack layouts and repeatable lab results across all modules, binaries are typically compiled disabling stack protections and Position Independent Executables (PIE):
```bash
gcc -g -fno-stack-protector -no-pie source.c -o binary
```

---

## Disclaimer
*This repository is created strictly for educational purposes, academic research, and cybersecurity training. Do not execute or test these exploits against systems without explicit, written authorization.*
