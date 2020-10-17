#include "crc32.h"

CRC32::CRC32()
{

}

unsigned int CRC32::calculate_CRC32 (unsigned char *pStart, unsigned int uSize)
{
    #define INIT  0xffffffffL
    #define XOROT 0xffffffffL

    unsigned int uCRCValue;
    unsigned char *pData;

    uCRCValue = INIT;
    pData = pStart;

    while (uSize --)
    {
        uCRCValue = crc32c_table[(uCRCValue ^ *pData++) & 0xFFL] ^ (uCRCValue >> 8);
    }

    return uCRCValue ^ XOROT;
}
