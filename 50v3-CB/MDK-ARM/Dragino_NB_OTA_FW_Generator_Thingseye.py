import os, sys
import binascii
import zlib
import struct

OriginalFirmwareName = "SN50V3-NB-GE.bin"
OutputFirmwareName = "SN50V3-NB-GE_OTA_Thingseye.bin"
Model = 0x22223333
FirmwareVersion = 0x10101010

FirmwareMagicNumber = 0xA8404166
NewFirmwareRequest = 0x01


def GetFileCRC32(file):
    with open(file, 'rb') as f:
        return zlib.crc32(f.read()) & 0xFFFFFFFF


if __name__ == "__main__":
    MergeFirmware = open("MergeFirmware.bin", "wb+")

    with open(OriginalFirmwareName, "rb") as f:
        FirmwareSize = os.path.getsize(OriginalFirmwareName)

        FirmwareCRC32 = GetFileCRC32(OriginalFirmwareName)

        pkt = struct.pack(b'<IBIIII', FirmwareMagicNumber, NewFirmwareRequest, Model, FirmwareVersion, FirmwareSize, FirmwareCRC32)

        DataLength = len(pkt)
        while DataLength < 4096:
            pkt += struct.pack(b'>B', 0x00)
            DataLength = DataLength + 1

        MergeFirmware.write(pkt)
        MergeData = f.read()
        MergeFirmware.write(MergeData)

    MergeFirmware.close()
    f.close()

    OutputFirmware = open(OutputFirmwareName, "wb+")
    with open("MergeFirmware.bin", "rb") as f:
        while True:
            if FirmwareSize // 1024:
                line_data = f.read(1024)
            else:
                line_data = f.read(FirmwareSize % 1024)

            if line_data == b'':
                break
            else:
                line_data_crc = zlib.crc32(line_data) & 0xFFFFFFFF
                pkt = struct.pack(b'<I', line_data_crc)
                OutputFirmware.write(line_data)
                OutputFirmware.write(pkt)

    f.close()
    OutputFirmware.close()
