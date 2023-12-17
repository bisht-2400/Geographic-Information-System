#ifndef GIS_LOGGER_H
#define GIS_LOGGER_H

#include <fstream>
#include <string>

using namespace std;

class Logger{
    private:
        string file = "logs.txt";


    public:
        int init(){
            ofstream MyFile(file);
            MyFile << "Course Project for COMP 8042\n"
                      "Student Name: Aadi Bisht, Student Id: A01053490\n"
                      "Begin of GIS Program log:\n"
                      "dbFile:\tdb.txt\n"
                      "script:\tscript01.txt\n"
                      "log:\tlog01.txt\n";
            int good;
            good=MyFile.good();
            MyFile.close();
            return good;
        }

        void write(const string& string1){
            ofstream MyFile(file, std::ios_base::app);
            MyFile << string1;
            MyFile.close();
        }

        void set_file(string str){
            file = std::move(str);
        }

        void close(){
            ofstream MyFile(file, std::ios_base::app);
            MyFile << "------------------------------------------------------------------------------------------\n"
                      "Terminating execution of commands.\n"
                      "------------------------------------------------------------------------------------------\n";
            MyFile.close();
        }

};
#endif //GIS_LOGGER_H
