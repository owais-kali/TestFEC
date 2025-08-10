#pragma once
#include "healper.h"

extern const char* dir;

void PrintPacket(BinaryFile file, int start_byte, int len_bytes);
void GenerateFECPacket();
void FindMediaPacketForFEC(BinaryFile fec);