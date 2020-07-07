#include <iostream>
#include <fstream>

#include "Ext4.hpp"
#include "byte_buffer2.hpp"
using namespace std;

int main(){
    ifstream in("../data/ext4.bin", ios::binary);


    //uint8_t data[0x1000];
    //if (in.is_open()){
    //    in.seekg(0x47480);
    //    in.read((char*)data, 0x1000);
    //} 
    //else {
    //    cout << "error" << '\n';
    //}
    
    // Superblock *sb = new Superblock(data, 0);
    // cout << hex << "inodecount : " << sb->iNodeCnt << endl;
    // cout << hex << "reserved block : " << sb->reservedBlkCnt << endl;
    // cout << hex << "magic : " << sb->magicSignature << endl;
    // cout << hex << "inode size : " << sb->iNodeSize << endl;
    // for(int i=0; i < 16; i++)
    //     printf("%x", sb->UUID[i]);
    // cout << endl;
    // cout << hex << "volume name : " << sb->volumeName << endl;
    
     if (in.is_open())
     {
         Ext4* ext4 = new Ext4(&in, 0L, false);

         cout << "EXT4 root node size:" << ext4->m_root_node->m_size << endl;
         cout << "EXT4 root node is dir?:" << ext4->m_root_node->is_directory() << endl;
         cout << "EXT4 block size:" << ext4->m_block_size << endl;
         cout << "EXT4 iNode size:" << ext4->m_inode_size << endl;

         ext4->build_filesystem();
         ext4->show_rst();
         cout << "build filesystem done" << endl;
     }
     else
     {
         cout << "i don't know" << endl;
     }
    

    //사라지지 말라
    int a;
    cin >> a;

    return 0;
}
