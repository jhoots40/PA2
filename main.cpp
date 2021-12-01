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
#include <vector>

void decode();
void print();

std::vector<std::string> machine;
std::vector<std::string> assembly;

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
        machine.push_back(str);
    }
    decode();
    print();

    return 0;
}

void decode()
{
    for(int i = 0; i < machine.size(); i++)
    {
        std::string binary = machine.at(i);
        std::cout << binary << std::endl;
        std::string opCode = binary.substr(0, 11);
        int value = stoi(opCode, 0, 2);


        if(value <= 159)
            std::cout << "Error: invalid instruction" << std::endl;
        else if(value >= 160 && value <= 191) // B // jake
        {

        }
        else if(value >= 672 && value <= 679) // B.cond // jake
        {

        }
        else if(value >= 712 && value <= 713) // ORRI //
        {

        }
        else if(value >= 840 && value <= 841) // EORI //
        {

        }
        else if(value == 1104) // AND //
        {

        }
        else if(value == 1112) // ADD //
        {

        }
        else if(value >= 1160 && value <= 1161) // ADDI //
        {
            std::string toAdd = "ADDI";
            std::string immediateString = binary.substr(10, 12);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int immediate = stoi(immediateString, 0, 2);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", #" + std::to_string(immediate);
            assembly.push_back(toAdd);
        }
        else if(value >= 1168 && value <= 1169) // ANDI //
        {

        }
        else if(value >= 1184 && value <= 1215) // BL // jake
        {

        }
        else if(value == 1360) // ORR //
        {

        }
        else if(value >= 1440 && value <= 1447) // CBZ // jake
        {

        }
        else if(value >= 1448 && value <= 1455) // CBNZ // jake
        {

        }
        else if(value == 1616) // EOR //
        {

        }
        else if(value == 1624) // SUB //
        {

        }
        else if(value >= 1672 && value <= 1673) // SUBI //
        {
            std::string toAdd = "SUBI";
            std::string immediateString = binary.substr(10, 12);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int immediate = stoi(immediateString, 0, 2);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", #" + std::to_string(immediate);
            assembly.push_back(toAdd);
        }
        else if(value == 1690) // LSR //
        {

        }
        else if(value == 1691) // LSL //
        {

        }
        else if(value == 1712) // BR // Jake
        {

        }
        else if(value == 1880) // SUBS //
        {

        }
        else if(value >= 1928 && value <= 1929) // SUBIS //
        {

        }
        else if(value == 1984) // STUR // jake
        {

        }
        else if(value == 1986) // LDUR // jake
        {

        }
        else if(value == 2044) // PRNL //
        {
            std::string toAdd = "PRNL";
            assembly.push_back(toAdd);
        }
        else if(value == 2045) // PRNT //
        {

        }
        else if(value == 2046) // DUMP //
        {
            std::string toAdd = "DUMP";
            assembly.push_back(toAdd);
        }
        else if(value == 2047) // HALT //
        {
            std::string toAdd = "HALT";
            assembly.push_back(toAdd);
        }
        //TODO
        /*else if(value == ??) // MUL //
        {

        }
        */
    }
}

void print()
{
    for(int i = 0; i < assembly.size(); i++)
    {
        std::cout << assembly.at(i) << std::endl;
    }
}

int binTwosComplementToSignedDecimal(char binary[],int significantBits)
{
    int power = pow(2,significantBits-1);
    int sum = 0;
    int i;

    for (i=0; i<significantBits; ++i)
    {
        if ( i==0 && binary[i]!='0')
        {
            sum = power * -1;
        }
        else
        {
            sum += (binary[i]-'0')*power;//The -0 is needed
        }
        power /= 2;
    }

    return sum;
}
