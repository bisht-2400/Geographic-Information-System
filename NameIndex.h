#include  <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

#ifndef GIS_NAMEINDEX_H
#define GIS_NAMEINDEX_H


using namespace std;
template <typename S>
ostream& operator<<(ostream& os,
                    const vector<S>& vector)
{
    for (const auto& element : vector) {
        os << element << ", ";
    }
    return os;
}
class NameIndex{
private:
    vector<string>hashtable;
    int count=0;
    int max_probe_len;

    static vector<string> extract(const string& str){
        vector<string> args;
        string word;
        stringstream ss(str);
        while (!ss.eof()) {
            getline(ss, word, '|');
            args.push_back(word);
        }
        return args;
    }

    bool isFull(){
        if ((count/hashtable.size())*100 >= 70){
            return true;
        }
        return false;
    }

    bool contains(const string& str){
        for(const auto& a:hashtable){
            if (a == str){
                return true;
            }
        }
        return false;
    }

public:

    NameIndex(){
        hashtable.resize(1024);
    }

    static unsigned int hash(string str) {
        unsigned int hash = 0;
        unsigned int x;
        unsigned int i;
        unsigned int len = str.length();
        for (i = 0; i < len; i++)
        {
            hash = (hash << 4) + (str[i]);
            if ((x = hash & 0xF0000000) != 0)
            {
                hash ^= (x >> 24);
            }
            hash &= ~x;
        }
        return hash;
    }
    int insert(const string& state, const string& abbr, int offset){
        if (isFull()){
            rehash();
        }
        if (contains(state+"|"+abbr+"|"+ to_string(offset))){
            return 0;
        }
        unsigned int pos = hash(state+abbr)%hashtable.size();
        int collision = 0;
        if (hashtable[pos].empty()){
            count++;
            hashtable[pos] = state+ "|" + abbr + "|" + to_string(offset);
        } else{
            for (int i = 0; i < hashtable.size(); ++i) {
                pos = pos+((i*i+i)/2);
                collision=i+1;
                if (hashtable[pos].empty()){
                    count++;
                    hashtable[pos] = state + "|" + abbr + "|" + to_string(offset);
                    break;
                }
            }
        }
        max_probe_len = collision;
        return collision;
    }

    int find(const string& state, const string& abbr){
        size_t pos = hash(state+abbr)%hashtable.size();
        size_t i = 0 ;
        int offset = -1;
        for (int j = 0; j < max_probe_len; ++j) {
            if (extract(hashtable[pos])[0] == state and extract(hashtable[pos])[1] == abbr){
                offset=stoi(extract(hashtable[pos])[2]);
                break;
            }
            i++;
            pos += ((i*i+i)/2);
        }
        return offset;
    }

    void rehash(){
        size_t old_capacity = hashtable.size();
        size_t new_cap = old_capacity * 2;
        vector<string> new_hashtable(new_cap);
        for (int i = 0; i < old_capacity; ++i) {
            if (hashtable[i].empty()) {
                continue;
            } else {
                string state, abbr, offset;
                vector<string> ext;
                ext = extract(hashtable[i]);
                state = ext[0];
                abbr = ext[1];
                offset = ext[2];
                size_t pos = hash(state + abbr) % new_cap;
                if (new_hashtable[pos].empty()) {
                    new_hashtable[pos] = state + "|" + abbr + "|" + offset;
                } else {
                    for (int j = 0; j < new_hashtable.size(); ++j) {
                        pos = pos + ((j * j + j) / 2);
                        if (new_hashtable[pos].empty()) {
                            new_hashtable[pos] = state + "|" + abbr + "|" + offset;
                            break;
                        }
                    }
                }
                hashtable = new_hashtable;
            }
        }
    }

    string str(){
        string str2;
        string str ="Format of display is \n"
                    "Slot number: data record\n"
                    "Current table size is "+ to_string(hashtable.size())+ "\n"
                    "Number of elements in table is "+ to_string(count)+"\n";
        for (int i = 0; i < hashtable.size(); ++i) {
            if (!hashtable[i].empty()){
            vector<string>ex = extract(hashtable[i]);
            str2 += to_string(i) + ": [" + ex[0]+ ":" + ex[1] + ", [" + ex[2]+"]]" + "\n";}
        }
        return str+"\n"+str2;
    }
};

#endif //GIS_NAMEINDEX_H