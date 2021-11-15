#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <endian.h>
#include <cstring>
#include <bitset>

void decode(uint32_t instruction);

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("incorrect amount of arguments\n");
        return -1;
    }
    
    struct stat results;
        
    if (stat(argv[1], &results) == 0)
        printf("The size of the file is %d bytes\n", results.st_size);
    else
        printf("an error has occured");
    
    struct stat buf;
    
    int fd;
    uint32_t *program;
    
    fd = open(argv[1], O_RDONLY);
    fstat(fd, &buf);
    program = (uint32_t*) mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    for (int i = 0; i < (buf.st_size / 4); i++) {
        program[i] = be32toh(program[i]);
        std::string str = std::bitset<32>(program[i]).to_string();
        std::cout << str << std::endl;
        decode(program[i]);
    }

    return 0;
}

void decode(uint32_t instruction)
{
    printf("made it here\n");
}
