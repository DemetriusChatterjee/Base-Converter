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

/**
 * Converts a hexadecimal string to an unsigned integer
 * @param hex The input hexadecimal string
 * @return The converted unsigned integer value
 */
unsigned int hex_to_uint(const char *hex) {
    unsigned int value = 0;
    while (*hex) {
        value <<= 4;  // Shift left by 4 bits (multiply by 16)
        if (*hex >= '0' && *hex <= '9') {
            value += *hex - '0';  // Convert decimal digits
        } else if (*hex >= 'A' && *hex <= 'F') {
            value += *hex - 'A' + 10;  // Convert uppercase hex
        } else if (*hex >= 'a' && *hex <= 'f') {
            value += *hex - 'a' + 10;  // Convert lowercase hex
        }
        hex++;
    }
    return value;
}

/**
 * Converts a binary string to an unsigned integer
 * @param binary The input binary string
 * @return The converted unsigned integer value, or -1 if invalid
 */
unsigned int binary_to_uint(const char *binary) {
    unsigned int value = 0;
    while (*binary) {
        if (*binary != '0' && *binary != '1') {
            return -1;  // Return error for invalid binary digits
        }
        value = (value << 1) | (*binary - '0');  // Shift left and add new bit
        binary++;
    }
    return value;
}

/**
 * Converts a string to an unsigned integer with specified base
 * @param str The input string
 * @param base The numeric base (2 for binary, 10 for decimal, 16 for hex)
 * @return The converted uint32_t value, or UINT32_MAX on error
 */
uint32_t string_to_int(const char *str, int base) {
    uint32_t retval = 0;
    uint32_t placeval = 1;
    int len = strlen(str);
    
    for (int i = len - 1; i >= 0; i--) {
        int digit;
        // Convert character to numeric value
        if (str[i] >= '0' && str[i] <= '9') {
            digit = str[i] - '0';
        } else if (str[i] >= 'A' && str[i] <= 'F') {
            digit = str[i] - 'A' + 10;
        } else if (str[i] >= 'a' && str[i] <= 'f') {
            digit = str[i] - 'a' + 10;
        } else {
            return UINT32_MAX;  // Invalid character
        }
        
        // Check if digit is valid for the given base
        if (digit >= base) {
            return UINT32_MAX;
        }
        
        retval += digit * placeval;
        placeval *= base;
    }
    return retval;
}

/**
 * Converts an unsigned integer to a string representation in specified base
 * @param value The input unsigned integer
 * @param str The output string buffer
 * @param base The numeric base (2 for binary, 10 for decimal, 16 for hex)
 */
void int_to_string(uint32_t value, char *str, int base) {
    char buffer[33] = {0};  // Temporary buffer for reverse string construction
    int idx = 0;
    
    // Special case: value is zero
    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    // Convert number to string, building it in reverse
    while (value > 0) {
        uint32_t rem = value % base;
        char digit;
        
        if (rem < 10) {
            digit = rem + '0';  // Convert to decimal digit
        } else {
            digit = (rem - 10) + 'a';  // Convert to lowercase hex digit
        }
        
        buffer[idx++] = digit;
        value /= base;
    }
    
    // Reverse the string into the output buffer
    for (int i = 0; i < idx; i++) {
        str[i] = buffer[idx - 1 - i];
    }
    str[idx] = '\0';  // Null terminate the string
}

int main(int argc, char *argv[]) {
    // Validate command line arguments
    if (argc != 4) {
        printf("Usage: %s <number> -o <base>\n", argv[0]);
        return 1;
    }

    char *input = argv[1];
    int output_base;
    uint32_t value = 0;
    
    // Verify the -o flag format
    if (argv[2][0] != '-' || argv[2][1] != 'o') {
        printf("Invalid format. Use -o <base>\n");
        return 1;
    }
    
    // Parse the output base
    output_base = string_to_int(argv[3], 10);
    if (output_base == UINT32_MAX) {
        printf("Invalid output base\n");
        return 1;
    }

    // Process input based on prefix (0b for binary, 0x for hex, or decimal)
    if (strncmp(input, "0b", 2) == 0) {
        // Binary input (0b prefix)
        value = string_to_int(input + 2, 2);
        if (value == UINT32_MAX) {
            printf("Bad input\n");
            return 1;
        }
    } else if (strncmp(input, "0x", 2) == 0) {
        // Hexadecimal input (0x prefix)
        value = string_to_int(input + 2, 16);
        if (value == UINT32_MAX) {
            printf("Bad input\n");
            return 1;
        }
    } else {
        // Decimal input (no prefix)
        value = string_to_int(input, 10);
        if (value == UINT32_MAX) {
            printf("Bad input\n");
            return 1;
        }
    }
    
    // Convert and output the result in requested format
    char result[33];  // Buffer for result string
    
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
