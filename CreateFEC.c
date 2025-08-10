#include "CreateFEC.h"
#include "string.h"
#include <inttypes.h>
#include <stdbool.h>

//RTP
//     0                   1                   2                   3
//     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |V=2|P|X|  CC   |M|     PT      |       sequence number         |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |                           timestamp                           |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |           synchronization source (SSRC) identifier            |
//    +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//    |            contributing source (CSRC) identifiers             |
//    |                             ....                              |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

//FEC
    //     0                   1                   2                   3
    //     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //    |0|0| P|X|  CC  |M| PT recovery |         length recovery       |
    //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //    |                          TS recovery                          |
    //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //    |   SSRCCount   |                    reserved                   |
    //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //    |                             SSRC_i                            |
    //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //    |           SN base_i           |k|          Mask [0-14]        |
    //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //    |k|                   Mask [15-45] (optional)                   |
    //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //    |k|                                                             |
    //    +-+                   Mask [46-108] (optional)                  |
    //    |                                                               |
    //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //    |                     ... next in SSRC_i ...                    |
    //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void PrintPacket(BinaryFile file, int start_byte, int len_bytes) {
    if (start_byte < 0 || len_bytes <= 0 || 
        start_byte + len_bytes > (int)file.size) {
        printf("Invalid range\n");
        return;
    }

    for (int i = 0; i < len_bytes; i++) {
        uint8_t byte = file.buffer[start_byte + i];
        for (int bit = 7; bit >= 0; bit--) {
            printf("%d", (byte >> bit) & 1);
        }
        printf(" "); // space between bytes
    }
    printf("\n");
}

const int payload_offset = 28;
const int SN_Base_i_offset = payload_offset + 16;

void FindMediaPacketForFEC(BinaryFile fec){
    uint16_t SN_Base = ((uint16_t)fec.buffer[SN_Base_i_offset] << 8) |
                        (uint16_t)fec.buffer[SN_Base_i_offset + 1];

    printf("%02X ", fec.buffer[SN_Base_i_offset]);
    printf("%02X ", fec.buffer[SN_Base_i_offset+1]);
    printf("%" PRIu16 "\n", SN_Base);

    uint16_t LowestSeq = UINT16_MAX;

    uint16_t HihestSeq = 0;
    for (size_t i = 0; i < 499; i++)
    {
        uint8_t filename[512];
        snprintf(filename, sizeof(filename), "%sp%ld.bin", dir, i);
        BinaryFile data = read_binary_file(filename);
        uint16_t SequenceNum = ((uint16_t)data.buffer[2] << 8) |
                        (uint16_t)data.buffer[2 + 1];
        // SequenceNum = swap_uint16(SequenceNum);
        if(SequenceNum == SN_Base){
            printf("Found Mediapacket in %s\n SEQ: %"PRIu16"\n",filename,SequenceNum);
            // break;
        }
        // if(SequenceNum<LowestSeq){
        //     LowestSeq = SequenceNum;
        // }
        // printf("Seq: %" PRIu16 "\n", SequenceNum);
        // if(SequenceNum < HihestSeq){
        //     printf("Gotha");
        // }
        if(SequenceNum > HihestSeq){
            HihestSeq = SequenceNum;
        }
    }
    printf("HihestSeq: %" PRIu16 "\n", HihestSeq);
}

void GetAllMediaPacketsProtectedByFEC(BinaryFile fec, int no_of_media){
    int first_k_bit_offset = payload_offset + 20;
    int no_of_k = 0;
    size_t kFlexfecPacketMaskSizes[] = {2, 6, 14};
    bool k_bit0 = (fec.buffer[first_k_bit_offset + 0] & 0x80) != 0;

    size_t packet_mask_size;
    if (k_bit0) {
        // The first K-bit is set, and the packet mask is thus only 2 bytes long.
        // We have now read the entire FEC header, and the rest of the packet
        // is payload.
        packet_mask_size = kFlexfecPacketMaskSizes[0];
    } else {
    }
}

void GenerateFECPacket(){

}