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
#include <math.h>
#include <algorithm>

void decode();
void print();
int twosCompToDecimal(char *binary, int significantBits);
std::string bCond(int val);
void handleLabel(int location, int currentPos);
void insertLabels();
int numLabel = 0;

std::vector<std::string> machine;
std::vector<std::string> assembly;
std::vector<int> labelIndices;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Incorrect amount of arguments\n" << std::endl;
        return -1;
    }

    struct stat results;

    if (stat(argv[1], &results) == 0)
        std::cout << "The size of the file is " << results.st_size << " bytes\n" << std::endl;
    else
        std::cout << "an error has occured" << std::endl;

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
    insertLabels();
    print();

    return 0;
}

void decode()
{
    for(int i = 0; i < machine.size(); i++)
    {
        std::string binary = machine.at(i);
        std::string opCode = binary.substr(0, 11);
        int value = stoi(opCode, 0, 2);


        if(value <= 159)
            std::cout << "Error: invalid instruction" << std::endl;
        else if(value >= 160 && value <= 191) // B // jake
        {
            std::string toAdd = "B";
            std::string addressString = binary.substr(6, 26);
            int address = twosCompToDecimal((char*) addressString.c_str(), 26);
            
            toAdd = toAdd + " " + std::to_string(address);
            assembly.push_back(toAdd);
            handleLabel(address, assembly.size() - 1);
        }
        else if(value >= 672 && value <= 679) // B.cond // jake
        {
            std::string toAdd = "B.";
            std::string addressString = binary.substr(8, 19);
            std::string rtString = binary.substr(27, 5);
            int address = twosCompToDecimal((char*) addressString.c_str(), 19);
            int rt = stoi(rtString, 0, 2);
            
            toAdd = toAdd + bCond(rt) + " " + std::to_string(address);
            assembly.push_back(toAdd);
            handleLabel(address, assembly.size() - 1);
        }
        else if(value >= 1424 && value <= 1425) // ORRI // (should work, won't print, machine code might be wrong)
        {
            std::string toAdd = "ORRI";
            std::string immediateString = binary.substr(10, 12);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int immediate = twosCompToDecimal((char*) immediateString.c_str(), 12);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", #" + std::to_string(immediate);
            assembly.push_back(toAdd);
        }
        else if(value >= 1680 && value <= 1681) // EORI // (should work, won't print, machine code might be wrong)
        {
            std::string toAdd = "EORI";
            std::string immediateString = binary.substr(10, 12);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int immediate = twosCompToDecimal((char*) immediateString.c_str(), 12);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", #" + std::to_string(immediate);
            assembly.push_back(toAdd);
        }
        else if(value == 1104) // AND //
        {
            std::string toAdd = "AND";
            std::string rmString = binary.substr(11, 5);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int rm = stoi(rmString, 0, 2);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", X" + std::to_string(rm);
            assembly.push_back(toAdd);
        }
        else if(value == 1112) // ADD //
        {
            std::string toAdd = "ADD";
            std::string rmString = binary.substr(11, 5);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int rm = stoi(rmString, 0, 2);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", X" + std::to_string(rm);
            assembly.push_back(toAdd);
        }
        else if(value >= 1160 && value <= 1161) // ADDI //
        {
            std::string toAdd = "ADDI";
            std::string immediateString = binary.substr(10, 12);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int immediate = twosCompToDecimal((char*) immediateString.c_str(), 12);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", #" + std::to_string(immediate);
            assembly.push_back(toAdd);
        }
        else if(value >= 1168 && value <= 1169) // ANDI //
        {
            std::string toAdd = "ANDI";
            std::string immediateString = binary.substr(10, 12);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int immediate = twosCompToDecimal((char*) immediateString.c_str(), 12);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", #" + std::to_string(immediate);
            assembly.push_back(toAdd);
        }
        else if(value >= 1184 && value <= 1215) // BL // jake
        {
            std::string toAdd = "BL";
            std::string addressString = binary.substr(6, 26);
            int address = twosCompToDecimal((char*) addressString.c_str(), 26);
            
            toAdd = toAdd + " " + std::to_string(address);
            assembly.push_back(toAdd);
            handleLabel(address, assembly.size() - 1);
        }
        else if(value == 1360) // ORR //
        {
            std::string toAdd = "ORR";
            std::string rmString = binary.substr(11, 5);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int rm = stoi(rmString, 0, 2);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", X" + std::to_string(rm);
            assembly.push_back(toAdd);
        }
        else if(value >= 1440 && value <= 1447) // CBZ // jake
        {
            std::string toAdd = "CBZ X";
            std::string addressString = binary.substr(8, 19);
            std::string rtString = binary.substr(27, 5);
            int address = twosCompToDecimal((char*) addressString.c_str(), 19);
            int rt = stoi(rtString, 0, 2);
            
            toAdd = toAdd + std::to_string(rt) + ", " + std::to_string(address);
            assembly.push_back(toAdd);
            handleLabel(address, assembly.size() - 1);
        }
        else if(value >= 1448 && value <= 1455) // CBNZ // jake
        {
            std::string toAdd = "CBNZ X";
            std::string addressString = binary.substr(8, 19);
            std::string rtString = binary.substr(27, 5);
            int address = twosCompToDecimal((char*) addressString.c_str(), 19);
            int rt = stoi(rtString, 0, 2);
            
            toAdd = toAdd + std::to_string(rt) + ", " + std::to_string(address);
            assembly.push_back(toAdd);
            handleLabel(address, assembly.size() - 1);
        }
        else if(value == 1616) // EOR //
        {
            std::string toAdd = "EOR";
            std::string rmString = binary.substr(11, 5);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int rm = stoi(rmString, 0, 2);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", X" + std::to_string(rm);
            assembly.push_back(toAdd);
        }
        else if(value == 1624) // SUB //
        {
            std::string toAdd = "SUB";
            std::string rmString = binary.substr(11, 5);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int rm = stoi(rmString, 0, 2);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", X" + std::to_string(rm);
            assembly.push_back(toAdd);
        }
        else if(value >= 1672 && value <= 1673) // SUBI //
        {
            std::string toAdd = "SUBI";
            std::string immediateString = binary.substr(10, 12);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int immediate = twosCompToDecimal((char*) immediateString.c_str(), 12);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", #" + std::to_string(immediate);
            assembly.push_back(toAdd);
        }
        else if(value == 1690) // LSR //
        {
            std::string toAdd = "LSR";
            std::string shamt = binary.substr(16, 6);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int s = stoi(shamt, 0, 2);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", #" + std::to_string(s);
            assembly.push_back(toAdd);
        }
        else if(value == 1691) // LSL //
        {
            std::string toAdd = "LSL";
            std::string shamt = binary.substr(16, 6);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int s = stoi(shamt, 0, 2);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", #" + std::to_string(s);
            assembly.push_back(toAdd);
        }
        else if(value == 1712) // BR // Jake
        {
            std::string toAdd = "BR";
            std::string rmString = binary.substr(11, 5);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int rm = stoi(rmString, 0, 2);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);
            
            toAdd = toAdd + " X" + std::to_string(rn);
            assembly.push_back(toAdd);
        }
        else if(value == 1880) // SUBS //
        {
            std::string toAdd = "SUBS";
            std::string rmString = binary.substr(11, 5);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int rm = stoi(rmString, 0, 2);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", X" + std::to_string(rm);
            assembly.push_back(toAdd);
        }
        else if(value >= 1928 && value <= 1929) // SUBIS //
        {
            std::string toAdd = "SUBIS";
            std::string immediateString = binary.substr(10, 12);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int immediate = twosCompToDecimal((char*) immediateString.c_str(), 12);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", #" + std::to_string(immediate);
            assembly.push_back(toAdd);
        }
        else if(value == 1984) // STUR // jake (double check?)
        {
            std::string toAdd = "STUR";
            std::string immediateString = binary.substr(10, 10);
            std::string rnString = binary.substr(22, 5);
            std::string rtString = binary.substr(27, 5);
            int immediate = stoi(immediateString, 0, 2);
            int rn = stoi(rnString, 0, 2);
            int rt = stoi(rtString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rt) + ", [X" + std::to_string(rn) + ", #" + std::to_string(immediate) + "]";
            assembly.push_back(toAdd);
        }
        else if(value == 1986) // LDUR // jake (double check?)
        {
            std::string toAdd = "LDUR";
            std::string immediateString = binary.substr(10, 10);
            std::string rnString = binary.substr(22, 5);
            std::string rtString = binary.substr(27, 5);
            int immediate = stoi(immediateString, 0, 2);
            int rn = stoi(rnString, 0, 2);
            int rt = stoi(rtString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rt) + ", [X" + std::to_string(rn) + ", #" + std::to_string(immediate) + "]";
            assembly.push_back(toAdd);
        }
        else if(value == 2044) // PRNL //
        {
            std::string toAdd = "PRNL";
            assembly.push_back(toAdd);
        }
        else if(value == 2045) // PRNT //
        {
            std::string toAdd = "PRNT";
            std::string rdString = binary.substr(27, 5);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd);
            assembly.push_back(toAdd);
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
        else if(value == 1240) // MUL //
        {
            std::string toAdd = "MUL";
            std::string rmString = binary.substr(11, 5);
            std::string rnString = binary.substr(22, 5);
            std::string rdString = binary.substr(27, 5);
            int rm = stoi(rmString, 0, 2);
            int rn = stoi(rnString, 0, 2);
            int rd = stoi(rdString, 0, 2);

            toAdd = toAdd + " X" + std::to_string(rd) + ", X" + std::to_string(rn) + ", X" + std::to_string(rm);
            assembly.push_back(toAdd);
        }
    }
}

void print()
{
    for(int i = 0; i < assembly.size(); i++)
    {
        std::cout << assembly.at(i) << std::endl;
    }
}

int twosCompToDecimal(char *binary, int significantBits)
{
    int power = pow(2,significantBits-1);
    int sum = 0;

    for(int i = 0; i < significantBits; i++)
    {
        if(i == 0 && binary[i] != '0')
        {
            sum = power * -1;
        }
        else
        {
            sum += (binary[i] - '0') * power;
        }
        power /= 2;
    }

    return sum;
}

std::string bCond(int val)
{
    if(val == 0)
    {
        return "EQ";
    }
    else if(val == 1)
    {
        return "NE";
    }
    else if(val == 2)
    {
        return "HS";
    }
    else if(val == 3)
    {
        return "LO";
    }
    else if(val == 4)
    {
        return "MI";
    }
    else if(val == 5)
    {
        return "PL";
    }
    else if(val == 6)
    {
        return "VS";
    }
    else if(val == 7)
    {
        return "VC";
    }
    else if(val == 8)
    {
        return "HI";
    }
    else if(val == 9)
    {
        return "LS";
    }
    else if(val == 10)
    {
        return "GE";
    }
    else if(val == 11)
    {
        return "LT";
    }
    else if(val == 12)
    {
        return "GT";
    }
    else if(val == 13)
    {
        return "LE";
    }
    return "Error";
}

void handleLabel(int location, int currentPos)
{
    int position = currentPos + location;
    if(position > 0)
        position = position - 1;
    
    bool unique = true;
    for(int i = 0; i < labelIndices.size(); i++)
    {
        if(labelIndices.at(i) == position)
            unique = false;
    }
    
    if(unique)
    {
        labelIndices.push_back(position);
    }
    
}

void insertLabels()
{
    std::sort (labelIndices.begin(), labelIndices.end());
    for(int i = 0; i < labelIndices.size(); i++)
    {
        std::vector<std::string>::iterator it;
        it = assembly.begin();
        
        ++numLabel;
        for(int j = 0; j <= labelIndices.at(i) + i; j++)
        {
            if(labelIndices.at(i) == 0)
                break;
            it++;
        }
        std::string toInsert = "\nLabel " + std::to_string(numLabel) + ":";
        it = assembly.insert(it, toInsert);
    }
}
