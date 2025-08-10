#include "healper.h"

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

BinaryFile read_binary_file(const char *filename) {
    BinaryFile bf = {0, NULL};

    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return bf;  // returns empty struct
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    bf.size = ftell(file);
    rewind(file);

    // Allocate memory
    bf.buffer = (uint8_t *)malloc(bf.size);
    if (!bf.buffer) {
        perror("Memory allocation failed");
        fclose(file);
        bf.size = 0;
        return bf;
    }

    // Read into buffer
    if (fread(bf.buffer, 1, bf.size, file) != bf.size) {
        perror("File read error");
        free(bf.buffer);
        bf.buffer = NULL;
        bf.size = 0;
    }

    fclose(file);
    return bf;
}