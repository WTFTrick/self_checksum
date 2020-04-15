#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <elf.h>
#include "crc32/crc32.h"

#define CHKSUM_DEBUG 1

#if CHKSUM_DEBUG
#define DEBUG(fmt, ...) printf("%s: " fmt, __FUNCTION__, __VA_ARGS__)
#else
#define DEBUG(fmt, ...)	
#endif

#define ERROR(fmt, ...) printf("%s: %d" fmt, __FUNCTION__, __LINE__, __VA_ARGS__)

#define INFO(fmt) printf("%s: " fmt, __FUNCTION__)

#define SOURCE_NAME "source"
#define CRC32_SECTION ".crc32_checksum"
unsigned long check_sum;

int calc_check_sum()
{
    FILE* fp = NULL; 
    char* program_name;
    Elf64_Ehdr eh;
    Elf64_Shdr* sh_table;

    fp = fopen(SOURCE_NAME, "r+b");
    if (fp == NULL){
        ERROR("Error to open file %s\n", SOURCE_NAME);
        return -1;
    }

    // Search for .crc32_checksum data
    fseek(fp, 0, SEEK_SET);
    fread(&eh, 1, sizeof(Elf64_Ehdr), fp);
    sh_table = malloc(eh.e_shentsize*eh.e_shnum);
    if (!sh_table){
	ERROR("%s: malloc failed!\n", __FUNCTION__);
    }

    fseek(fp, eh.e_shoff, SEEK_SET);
    fread(sh_table, 1, eh.e_shentsize*eh.e_shnum, fp);

    int i;
    char* sh_str;
    char* buff;

    buff = malloc(sh_table[eh.e_shstrndx].sh_size);
    if (!buff){
        ERROR("%s: malloc failed!\n", __FUNCTION__);
        return -1;
    }

    fseek(fp, sh_table[eh.e_shstrndx].sh_offset, SEEK_SET);
    fread(buff, 1, sh_table[eh.e_shstrndx].sh_size, fp);
    sh_str = buff;

    for(i=0; i<eh.e_shnum; i++)
    {
        if(!strcmp(CRC32_SECTION, (sh_str + sh_table[i].sh_name)))
        {
            DEBUG("Found section\t\"%s\"\n", CRC32_SECTION);
            DEBUG("at offset\t0x%08x\n", (unsigned int)sh_table[i].sh_offset);
            DEBUG("of size\t\t0x%08x\n", (unsigned int)sh_table[i].sh_size);
            break;
        }
    }

    // Create buf, containing all from source bin, except .crc32_checksum data
    fseek(fp, 0L, SEEK_END);
    size_t file_size = ftell(fp);
    size_t buf_size = file_size - sh_table[i].sh_size + 1;
    fseek(fp, 0L, SEEK_SET);
    char *buf = malloc(buf_size);
    if (buf == NULL){
	ERROR("%s: malloc failed!\n", __FUNCTION__);
	return -1;
    }
    buf[buf_size] = 0;
    fread(buf, 1, sh_table[i].sh_offset, fp);
    fseek(fp, sh_table[i].sh_size, SEEK_CUR);
    fread(buf + sh_table[i].sh_offset, 1, file_size - (sh_table[i].sh_size + sh_table[i].sh_offset), fp);
    // Calculate crc32 for formed buffer
    Crc32_ComputeFromBuf(buf, buf_size, &check_sum);
    fseek(fp, 0L, SEEK_SET);
    DEBUG("Write to bin %s: calculated chksum %08lx\n", SOURCE_NAME, check_sum);

    if (i < eh.e_shnum) {
        char *mydata = malloc(sh_table[i].sh_size);
        fseek(fp, sh_table[i].sh_offset, SEEK_SET);
        memcpy(mydata, &check_sum, sizeof(unsigned long));
	// Write crc32 check sum in .crc32_checksum data
	fwrite(mydata, 1, sh_table[i].sh_size, fp);
    } else {
        ERROR("Section %s not found!\n", CRC32_SECTION);
	return -1;
    }
    fclose(fp);
    return 0;

}

int main()
{
    return calc_check_sum();
} 
