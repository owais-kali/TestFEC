#include "healper.h"

#include "CreateFEC.h"

const char* dir = "/home/owais/CloudGaming/Sandbox/packets_logs/";

int main(int argc, char const *argv[]) {
    char* file_name = argv[1];

    uint8_t filename[512];
    snprintf(filename, sizeof(filename), "%s%s", dir, file_name);
    printf("File: %s\n",filename);

    BinaryFile data = read_binary_file(filename);

    // if (data.buffer) {
    //     printf("File size: %zu bytes\n", data.size);

    //     // Example: print first 10 bytes in hex
    //     for (size_t i = 0; i < data.size && i < 10; i++) {
    //         printf("%02X ", data.buffer[i]);
    //     }
    //     printf("\n");

    //     // Free allocated memory
    //     free(data.buffer);
    // }

    // PrintPacket(data, 0, 100);
    FindMediaPacketForFEC(data);

    return 0;
}

