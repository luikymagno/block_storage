#include <iostream>
#include <algorithm>

#include "block_storage.h"

using namespace std;

struct Register {
    int idade;
    char nome[30];
    char cpf[10];
};

struct Block {
    Register data[93];
    char reserved[4];
};

int main() {
    cout << "Register Size: " << sizeof(Register) << endl;

    BlockStorage<Block> storage("/home/davi/storage_file");


    uint32_t blksz = 0;
    storage.GetBlockSize(blksz);

    cout << "Block Size: " << blksz << endl;


    BlockArray<Block> blocks(blksz, 1000000);


    if (storage.WriteBlocks(0, blocks) != kNoError) {
        cout << "write error!" << endl;
    }

    if (storage.ReadBlocks(0, blocks) != kNoError) {
        cout << "read error!" << endl;
    }

    return 0;
}
