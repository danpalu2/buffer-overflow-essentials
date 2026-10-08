# Stack Variable Overwrite (Buffer Overflow Basics)

## Project Overview
This hands-on lab demonstrates the fundamental mechanics of a **Stack-based Buffer Overflow**. It illustrates how missing bounds-checking on user input allows an attacker to overwrite contiguous local variables within a function's stack frame, altering the program's conditional execution flow (`if/else`) without needing to hijack the return address (RIP/EIP).

---

## Vulnerability Analysis

### Vulnerable Code
The program allocates a 64-byte character buffer (`char buffer[64]`) and uses the deprecated, unsafe function `gets()`:

```c
char not_overflow;
int privileges = 0;
char buffer[64];

not_overflow = 'Y';
gets(buffer); // Vulnerability: no boundary check on input length
```

### Memory Layout in the Stack
When `main` executes, local variables are allocated within the Stack frame. Because the stack grows toward lower memory addresses, data written into `buffer` moves toward higher memory addresses, eventually reaching the adjacent `not_overflow` variable:

```text
[ Lower Memory Addresses (RSP) ]
+-----------------------------------+
|  buffer[64]                       |  <- Start of gets() writing
|  (64 bytes)                       |
+-----------------------------------+
|  ... (padding and alignment)      |
+-----------------------------------+
|  not_overflow ('Y' / 0x59)        |  <- Target overwrite (1 byte)
+-----------------------------------+
|  privileges (0x00000000)          |  <- Privilege variable (4 bytes)
+-----------------------------------+
|  Saved RBP                        |  <- Previous Frame Pointer
+-----------------------------------+
|  Saved RIP                        |  <- Return Address
+-----------------------------------+
[ Higher Memory Addresses ]
```

If the user input exceeds the space allocated for `buffer`, the extra bytes overwrite `not_overflow`, changing its original value (`'Y'` / `0x59`) and forcing the program into the restricted `else` branch.

---

## Requirements & Compilation

### Requirements
* Linux Operating System (e.g., Ubuntu / Debian)
* `gcc` Compiler
* GNU Debugger (`gdb`)
* Python 3

### Compilation
To reproduce the vulnerability, stack protections must be disabled using the `-fno-stack-protector` flag, and debugging symbols included with `-g`:

```bash
gcc -g -fno-stack-protector overflow0.c -o overflow0
```

> **Note**: The compiler will issue a warning regarding the unsafe `gets()` function.

---

## Dynamic Analysis with GDB

### 1. Launch & Breakpoints
Start the debugger and place breakpoints to analyze the stack state before and after input ingestion:

```text
gdb ./overflow0
(gdb) break main
(gdb) break 20
(gdb) run
```

### 2. Inspecting the Stack Pointer (RSP)
Examine memory at the stack pointer before supplying input:

```text
(gdb) x/20gx $rsp
```

Provide a controlled input (e.g., 8 bytes of `'A'` / `0x41` in ASCII) and continue execution to the next breakpoint:

```text
(gdb) continue
Enter some text:
AAAAAAAA
(gdb) x/20gx $rsp
```

In the GDB output, the byte sequence `0x4141414141414141` is visible at the buffer's starting address.

---

## Attack Reproduction (Exploit)

### Generating the Payload
Run the Python script to create an input payload with sufficient padding (between 75 and 87 bytes of `'A'`) to overwrite `not_overflow` without corrupting the saved `RIP` return address (preventing a `Segmentation Fault`):

```bash
python3 exploit.py
```

### Executing the Attack
Redirect `attack.txt` into the vulnerable application:

```bash
./overflow0 < attack.txt
```

### Output
```text
Enter some text: 
Your secret password is overflow90
```

The value of `not_overflow` was successfully overwritten, enabling the elevated privileges path (`privileges = 1`).

---

## Mitigations & Secure Coding

1. **Replace `gets()`**: Use boundary-checked functions such as `fgets()`:
   ```c
   fgets(buffer, sizeof(buffer), stdin);
   ```
2. **Enable Stack Canaries**: Compile without `-fno-stack-protector` so the compiler places a canary value before the frame pointer. If modified, the process terminates before executing arbitrary code.
3. **ASLR and DEP/NX**: Ensure Address Space Layout Randomization and Data Execution Prevention are active at the kernel level.