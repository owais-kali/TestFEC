#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int checkEndien();

uint16_t swap_uint16(uint16_t val);

typedef struct {
    size_t size;       
    uint8_t *buffer;   
} BinaryFile;

BinaryFile read_binary_file(const char *filename);