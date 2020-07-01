#include <iostream>
#include <fstream>

#include "Superblock.hpp"

using namespace std;

int main(){
    ifstream in("../data/ext4.bin", ios::binary);
    
    uint8_t data[1024];
    if (in.is_open()){
        in.seekg(0x400);
        in.read((char*)data, 0x400);
        cout << hex << data << '\n';
    } 
    else {
        cout << "error" << '\n';
    }
    
    Superblock *sb = new Superblock(data, 0);
    
    return 0;
}