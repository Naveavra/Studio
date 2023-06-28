#include "Studio.h"
#include <iostream>
#include <fstream>
#include <sstream>



using namespace std;

Studio* backup = nullptr;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "usage: studio <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    Studio studio(configurationFile);
    //Studio studio("C:/Users/mikid/Desktop/ExampleInput.txt");
    studio.start();
    if (backup != nullptr) {
        delete backup;
        backup = nullptr;
    }
    ifstream file(configurationFile);
    stringstream buffer;

    buffer << file.rdbuf(); 

    buffer.str();


    return 0;
}