#ifndef __CRC32_H__
#define __CRC32_H__

#include <stdio.h>
#include <stdlib.h>

int Crc32_ComputeFromBuf(char *buf, size_t bufLen, unsigned long *outCrc32);

#endif /* __CRC32_H__ */
