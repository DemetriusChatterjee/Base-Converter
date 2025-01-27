#include <stdio.h>
#include <string.h>
#include <stdint.h>
/*
    project01.c
    Author: Demetrius Chatterjee
    Date: 2025-01-25
    Description: This program converts an unsigned integer to a binary string
                converts a hex string to a binary string
                converts a binary string to an unsigned integer
                converts a hex string to an unsigned integer
                converts an unsigned integer to a hex string
                converts a binary string to a hex string
*/

unsigned int hex_to_uint(const char *hex) { //converts a hex string to an unsigned integer using bitwise operations
    unsigned int value = 0;
    while (*hex) {
        value <<= 4;
        if (*hex >= '0' && *hex <= '9') {
            value += *hex - '0';
        } else if (*hex >= 'A' && *hex <= 'F') {
            value += *hex - 'A' + 10;
        } else if (*hex >= 'a' && *hex <= 'f') {
            value += *hex - 'a' + 10;
        }
        hex++;
    }
    return value;
}

unsigned int binary_to_uint(const char *binary) { //converts a binary string to an unsigned integer using bitwise operations
    unsigned int value = 0;
    while (*binary) {
        if (*binary != '0' && *binary != '1') {
            return -1;  // Invalid binary
        }
        value = (value << 1) | (*binary - '0');
        binary++;
    }
    return value;
}

uint32_t string_to_int(const char *str, int base) { //converts a string to an unsigned integer using bitwise operations
    uint32_t retval = 0;
    uint32_t placeval = 1;
    int len = strlen(str);
    
    for (int i = len - 1; i >= 0; i--) {
        int digit;
        // Calculate integer value of character
        if (str[i] >= '0' && str[i] <= '9') {
            digit = str[i] - '0';
        } else if (str[i] >= 'A' && str[i] <= 'F') {
            digit = str[i] - 'A' + 10;
        } else if (str[i] >= 'a' && str[i] <= 'f') {
            digit = str[i] - 'a' + 10;
        } else {
            return UINT32_MAX; // Error case in case of invalid input
        }
        
        if (digit >= base) {
            return UINT32_MAX; // Error case in case of invalid input
        }
        
        retval += digit * placeval;
        placeval *= base;
    }
    return retval;
}

void int_to_string(uint32_t value, char *str, int base) {
    char buffer[33] = {0}; // Max 32 binary digits + null terminator
    int idx = 0;
    
    // Handle special case of value = 0
    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    // Convert to string representation
    while (value > 0) {
        uint32_t rem = value % base;
        char digit;
        
        if (rem < 10) {
            digit = rem + '0';
        } else {
            digit = (rem - 10) + 'a';  // Changed 'A' to 'a' for lowercase hex
        }
        
        buffer[idx++] = digit;
        value /= base;
    }
    
    // Copy buffer in reverse order
    for (int i = 0; i < idx; i++) {
        str[i] = buffer[idx - 1 - i];
    }
    str[idx] = '\0';
}

int main(int argc, char *argv[])
{
    if (argc != 4) {  // Changed from 3 to 4 since we need one more argument
        printf("Usage: %s <number> -o <base>\n", argv[0]);
        return 1;
    }

    char *input = argv[1];
    int output_base;
    uint32_t value = 0;
    
    if (argv[2][0] != '-' || argv[2][1] != 'o') {
        printf("Invalid format. Use -o <base>\n"); //prints invalid format message
        return 1;
    }
    
    // Convert output base from the third argument
    output_base = string_to_int(argv[3], 10);
    if (output_base == UINT32_MAX) {
        printf("Invalid output base\n");
        return 1;
    }

    // Process input based on prefix
    if (strncmp(input, "0b", 2) == 0) {
        value = string_to_int(input + 2, 2);
        if (value == UINT32_MAX) {
            printf("Bad input\n");
            return 1;
        }
    } else if (strncmp(input, "0x", 2) == 0) {
        value = string_to_int(input + 2, 16);
        if (value == UINT32_MAX) {  // Add error checking for hex input
            printf("Bad input\n");
            return 1;
        }
    } else {
        value = string_to_int(input, 10);
        if (value == UINT32_MAX) {
            printf("Bad input\n");
            return 1;
        }
    }
    
    // Output in requested format
    char result[33];  // Max 32 binary digits + null terminator
    
    switch (output_base) {
        case 2:
            printf("0b");
            int_to_string(value, result, 2);
            printf("%s\n", result);
            break;
            
        case 10:
            int_to_string(value, result, 10);
            printf("%s\n", result);
            break;
            
        case 16:
            printf("0x");
            int_to_string(value, result, 16);
            printf("%s\n", result);
            break;
            
        default:
            printf("Bad error\n");
            return 1;
    }

    return 0;
}
