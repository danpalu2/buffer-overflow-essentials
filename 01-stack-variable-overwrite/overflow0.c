#include <stdio.h>

/*  
    Compilation without stack protections and with debug symbols:
    gcc -g -fno-stack-protector overflow0.c -o overflow0
*/

int main(int argc, char** argv)
{
    char not_overflow;
    int privileges = 0;
    char buffer[64];

    printf("Enter some text: \n");

    not_overflow = 'Y';

    // VULNERABILITY: gets() does not check input length against buffer[64]
    gets(buffer);

    if (not_overflow == 'Y') {
        printf("Can't overflow Me\nThis is the content of the buffer:\n%s\n", buffer);
    }
    else {
        // If not_overflow is overwritten with any value other than 'Y', privileges are granted
        privileges = 1;
    }

    if (privileges == 1) {
        printf("Your secret password is overflow90\n");
    }
    
    return 0;
}