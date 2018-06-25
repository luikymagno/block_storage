#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <string>

#include "block_storage.h"

using namespace std;

struct Register {
    int idade;
    char nome[30];
    char cpf[10];
};

struct MyBlockType {
    Register data[93];
    char reserved[4];
};

Register MakeRandomRegister() {
    Register reg;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> age_rand(1,100);
    reg.idade = age_rand(generator);
}

int main() {

    BlockStorage<MyBlockType> storage("/home/davi/storage_file");

    size_t blksz = 0;
    storage.GetBlockSize(blksz);

    cout << "Block Size: " << blksz << endl;

    Block<MyBlockType> block(blksz);

    for(int i=0; i<1000; i++) {
        if (storage.WriteBlock(i, block) != kNoError) {
            cout << "write error!" << endl;
            break;
        }

//        if (storage.ReadBlock(0, block) != kNoError) {
//            cout << "read error!" << endl;
//            break;
//        }

        cout << "i = " << i << endl;
    }
    return 0;
}
