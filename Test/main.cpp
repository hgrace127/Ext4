#include <iostream>
#include <fstream>
#include "develop/include/byte_buffer2.hpp"
#include "develop/include/endian_swap.hpp"
#include "../src/Superblock.cpp"
#include <cstring>

using namespace std;

int main(){
    ifstream in("../data/ext4.bin", ios::binary);
    
    uint8_t data[1024];
    if(in.is_open()){
        in.seekg(0x400);
        in.read((char*)data, 0x400);
        cout << hex << data << '\n';
    }else {
        cout << "error" << '\n';
    }
    
    Superblock *sb = new Superblock(data, 0);
    cout << "11" << '\n';
    // cout << sb->iNodeCnt << '\n';
    // cout << sb->magicSignature << '\n';
    // cout << sb->iNodeSize << '\n';
    // cout << sb->blkGroupNo << '\n';

    return 0;
}