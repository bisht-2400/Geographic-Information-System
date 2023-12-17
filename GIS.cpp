#include <iostream>
#include "CommandProcessor.h"

using namespace std;
int main(int argc, char** argv) {
    if (argc == 4){
    Logger logger;
    logger.init();
    logger.set_file(argv[3]);
    string myText;
    CommandProcessor commandProcessor;
    commandProcessor.set_db(argv[1]);
    ifstream MyReadFile(argv[2]);
    while (getline (MyReadFile, myText)) {
        commandProcessor.give_string(myText);
        commandProcessor.reset();
    }
    logger.close();}
    else cout<<"Wrong Args.\nExiting";
}
