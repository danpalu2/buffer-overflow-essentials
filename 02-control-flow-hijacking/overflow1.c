#include <stdio.h>
#include <stdlib.h>

/*
  Compilation without stack protections, with debug symbols, 
  and disabling PIE (Position Independent Executable) to have fixed addresses:
  gcc -g -fno-stack-protector -no-pie overflow1.c -o overflow1
*/

void success() {
    printf("CONGRATULATIONS! You successfully hijacked the control flow!\n");
    printf("Flag: flag{ret2win_success_achieved}\n");
    exit(0);
}

void login() {
    char buffer[64];
    
    printf("Enter password: ");
    // VULNERABILITY: gets() allows writing past the 64-byte buffer,
    // corrupting the saved RBP and the saved RIP (Return Address)
    gets(buffer);
    
    printf("Access Denied.\n");
}

int main(int argc, char** argv) {
    printf("=== Secure Login System v1.0 ===\n");
    login();
    return 0;
}