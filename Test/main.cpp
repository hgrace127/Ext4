#include <iostream>
#include <fstream>

#include "Superblock.hpp"
#include "byte_buffer2.hpp"
using namespace std;

int main(){
    ifstream in("../data/ext4.bin", ios::binary);
    uint8_t data[1024];
/*
    in.seekg(0x400);
    in.read((char*)data, 0x400);

    ByteBuffer2 bb(data, 0, 1024, true);

    for(int i=0; i<256; i++){
        cout << hex << bb.get_uint32_le() << " ";
        if(i % 4 == 0 && i != 0)
            cout << endl;
    }
*/
    
    if (in.is_open()){
        in.seekg(0x400);
        in.read((char*)data, 0x400);
        cout << hex << data << '\n';
    } 
    else {
        cout << "error" << '\n';
    }
    
    Superblock *sb = new Superblock(data, 0);

    cout << hex << "inodecount : " << sb->iNodeCnt << endl;
    cout << hex << "reserved block : " << sb->reservedBlkCnt << endl;
    cout << hex << "magic : " << sb->magicSignature << endl;
    cout << hex << "inode size : " << sb->iNodeSize << endl;
    for(int i=0; i < 16; i++)
        printf("%x", sb->UUID[i]);
    cout << endl;
    cout << hex << "volume name : " << sb->volumeName << endl;
    



    int a;
    cin >> a;

    return 0;
}
