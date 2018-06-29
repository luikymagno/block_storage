#include <iostream>
#include <algorithm>
#include <string>

#include "block_storage.h"

using namespace std;

const std::string kStorageFilePath = "./storage_file";

struct Register {
    int age;
    char name[30];
    char ssn[10];
};

struct MyBlockType {
    Register registers[93];
    char reserved[4];
};

/**
 * @brief Simple example showing block_storage usage.
 */
int main(int argc, char *argv[]) {


    try {
        BlockStorage<MyBlockType> storage(kStorageFilePath);

        Block<MyBlockType> block;

        int num_operations = std::stoi(argv[1]);
        num_operations++;

        storage.ClearFile();

        for (int i = 1; i < num_operations; i++) {
            if (storage.WriteBlock(i, block) != blkstorage::kNoError) {
                cout << "There was an error during WriteBlock()." << endl;
                break;
            }

            if (storage.ReadBlock(i, block) != blkstorage::kNoError) {
                cout << "There was an error during ReadBlock()." << endl;
                break;
            }

            cout << "Block " << i << " was written/read successfully!"
                 << endl;
        }
    }
    catch (std::logic_error &e) {
        cout << "Invalid block size." << endl;
    }

    catch (std::runtime_error &e) {
        cout << "File error." << endl;
    }

    return 0;
}
