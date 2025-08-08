#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const char* dir = "/home/owais/CloudGaming/packets_logs/";
const char* file = "p2.bin";

char filename[512]; 

int checkEndien(){
    uint16_t x = 0x0102;       // 2-byte value
    uint8_t* ptr = (uint8_t*)&x;

    if (ptr[0] == 0x01) {
        printf("Big-endian\n");
    } else if (ptr[0] == 0x02) {
        printf("Little-endian\n");
    } else {
        printf("Unknown endianness\n");
    }

    return 0;
}

uint16_t swap_uint16(uint16_t val) {
    return (val << 8) | (val >> 8);
}

int main() {
    checkEndien();
    // Concatenate using snprintf (safe and preferred)
    snprintf(filename, sizeof(filename), "%s%s", dir, file);
    FILE* file = fopen(filename, "rb");    // Open in binary mode

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);  // Move back to the beginning

    // Allocate memory for the char array (+1 for null terminator)
    char* buffer = (char*) malloc(file_size + 1);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    // Read the file into the buffer
    size_t bytesRead = fread(buffer, 1, file_size, file);
    buffer[bytesRead] = '\0';  // Null-terminate the string

    fclose(file);

    // Use the char array (print in this case)
    // Print bits (from most significant bit to least)
    for (int i = 7; i >= 0; i--) {
        printf("%d", (buffer[0] >> i) & 1);
    }

    __uint16_t* seq_number = 0;
    seq_number = &buffer[2];

    printf("squence_number = %u", *seq_number);
uint16_t swapped = swap_uint16(seq_number);

    printf("Swapped:  %u\n", swapped);
    // Free the buffer
    free(buffer);

    return 0;
}
