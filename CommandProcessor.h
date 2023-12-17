#include <iostream>
#include <cstring>
#include <sstream>
#include <utility>
#include <vector>
#include <iterator>
#include <limits>
#include "Logger.h"
#include "NameIndex.h"
#include "GISRecord.h"
#include "BufferPool.h"

#ifndef GIS_COMMANDPROCESSOR_H
#define GIS_COMMANDPROCESSOR_H


using namespace std;


enum Command{
    world,
    import,
    debug,
    quit,
    what_is_at,
    what_is,
    what_is_in,
    comment,
};


class CommandProcessor{
    private:
        vector<string> flags;
        Command cmd;
        string db_file;
        vector<DMS> world_limit;
        NameIndex nameIndex;
        int cmd_track=1;
        BufferPool bufferPool;

    vector<string> get_flags(const string& str){
            stringstream ss(str);
            string word;
            while (!ss.eof()) {
                getline(ss, word, '\t');
                flags.push_back(word);
            }
            return flags;
        };

        Command get_command(){
            string cmd_str = flags.front();
            Command command = convert(cmd_str);
            if (command!=comment)
                flags.erase(flags.begin());
            return command;
        }

        static Command convert(const std::string& str)
        {
            if(str == "world") return world;
            else if(str == "import") return import;
            else if(str == "debug") return debug;
            else if(str == "quit") return quit;
            else if(str == "what_is_at") return what_is_at;
            else if(str == "what_is") return what_is;
            else if(str == "what_is_in") return what_is_in;
            else return comment;
        }

        static vector<string> get_args(const string& str){
            vector<string> args;
            string word;
            stringstream ss(str);
                while (!ss.eof()) {
                    getline(ss, word, '|');
                    args.push_back(word);
                }
            return args;
        }

public:
        void set_db(string db){
            db_file=std::move(db);
            ofstream MyFile(db_file);
            MyFile.close();
        }

        void write_db(const string& string1){
            ofstream MyFile(db_file, std::ios_base::app);
            MyFile << string1 << '\n';
            MyFile.close();
        }

        void give_string(string& str){
            get_flags(str);
            cmd = get_command();
            process();
        }

        void reset(){
            flags.clear();
        }


        void import_command() {
            int offset_counter=0,insert_counter=0, max_probe_seq=0;
            ifstream MyReadFile(flags[0]);
            string myText;
            MyReadFile.ignore(320, '\n');
            while (getline (MyReadFile, myText)) {
                GISRecord gisRecord = GISRecord(get_args(myText));
                if (contains(world_limit, gisRecord.prim_long_dms, gisRecord.primary_lat_dms)){
                    write_db(myText);
                    int probe_seq = nameIndex.insert(gisRecord.feature_name, gisRecord.state_alpha, offset_counter);
                    if (probe_seq>max_probe_seq){
                        max_probe_seq = probe_seq;
                    }
                    offset_counter++;
                    insert_counter++;}
            }
            Logger logger;
            logger.write("Imported Features by name: " + to_string(insert_counter)+ '\n'+"Longest probe sequence:\t"+
                                                                                         to_string(max_probe_seq)+"\nImported Locations:"+
                                                                                                                  to_string(insert_counter));
        }

        void world_command(){
            for (const auto& w: flags) {
                DMS dms = create_DMS(w);
                world_limit.push_back(dms);
//                cout << "DMS: " << dms.degrees << dms.minutes << dms.seconds << dms.direction << endl;
            }
        }

        void what_is_command(){
            int offset = nameIndex.find(flags[0], flags[1]);
            if (offset == -1){
//                cout << "Couldn't find a match for " << flags[0] << " " << flags[1] << endl;
                return;
            }
            string s = read_line(offset);
            bufferPool.insert(GISRecord(get_args(s)));
//            cout << bufferPool.count << endl;
        }

        string read_line(int offset){
            string s;
            ifstream file(db_file);
            for (int i = 0; i < offset; ++i) {
                file.ignore(320, '\n');
            }
            getline(file, s);
            file.close();
            return s;
        }

        void process(){
            Logger logger;
            switch (cmd) {
                case world:
                   world_command();
                    cmd_track++;
                    logger.write("Command "+ to_string(cmd_track)+": ");
                    logger.write("\n------------------------------------------------------------------------------------------\n");
                    break;
                case import:
                    cmd_track++;
                    import_command();
                    logger.write("Command "+ to_string(cmd_track)+": ");
                    logger.write("\n------------------------------------------------------------------------------------------\n");
                    world_limit.clear();
                    break;
                case debug:
                    cmd_track++;
                    logger.write("Command "+ to_string(cmd_track)+": ");
                    if (flags[0] == "quad"){
                        logger.write("'Quad' command not implemented.\\n\\n");
                    } else if(flags[0] == "hash"){
                        logger.write(nameIndex.str());
                    } else if (flags[0] == "pool"){
                        logger.write(bufferPool.str());
                    }
                    logger.write("\n------------------------------------------------------------------------------------------\n");
                    break;
                case quit:
                    logger.write("Command "+ to_string(cmd_track)+": " +"\n");
                    this->reset();
                    cmd_track++;
                    break;
                case what_is_at:{
                    logger.write("Command "+ to_string(cmd_track)+": " +"\n");
                    logger.write("'What is at' command not implemented.\n\n");
                    cmd_track++;
                    break;
                }
                case what_is:
                    logger.write("Command "+ to_string(cmd_track)+": " +"\n");
                    what_is_command();
                    logger.write("\n------------------------------------------------------------------------------------------\n");
                    cmd_track++;
                    break;
                case what_is_in:{
                    logger.write("Command "+ to_string(cmd_track)+": " +"\n");
                    logger.write("'What is in' command not implemented.\n\n");
                    cmd_track++;
                    break;
                }
                case comment:
                    std::string s;
                    for (const auto &piece : flags){ s += (string) piece + "\n";}
                    logger.write(s);
                    break;
            }
        }
};
#endif //GIS_COMMANDPROCESSOR_H
