#include <iostream>

#include "make_block_storage.h"

using namespace std;

struct Page {
    char data[4096];
};

int main() {
    auto storage = MakeBlockStorage<Page>("/home/davi/storage_file");
    uint32_t block_size;
    storage->GetBlockSize(block_size);
    cout << "Block Size: " << block_size << endl;
    if (storage->ClearAndAlloc(1) == kFileError) {
        cout << "File Exists!" << endl;
    }
    return 0;
}
