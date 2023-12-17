#ifndef GIS_BUFFERPOOL_H
#define GIS_BUFFERPOOL_H

#define POOL_SIZE 15

#include "GISRecord.h"
#include <vector>
#include <algorithm>
#include <numeric>


using namespace std;

class BufferPool{

private:
        bool contains(const GISRecord& gisRecord1){
            for (GISRecord record: gisRecord){
                if(record == gisRecord1) return true;
            }
            return false;
        }

    public:

    vector<GISRecord> gisRecord;
    vector<int> count;
         BufferPool(){
                gisRecord.resize(15);
                count.resize(15);
            }

        void insert(const GISRecord& gisRecord1){
            if (this->contains(gisRecord1)) { increase_count(gisRecord1);return;}
            if (gisRecord.size()<POOL_SIZE) gisRecord.push_back(gisRecord1);
            else{
            auto it = std::min_element(std::begin(count), std::end(count));
            gisRecord[std::distance(std::begin(count), it)] = gisRecord1;
            count[std::distance(std::begin(count), it)]=1;
            }
        }

        void increase_count(const GISRecord& gisRecord1){
            for (int i = 0; i < gisRecord.size(); ++i) {
                if (gisRecord[i]==gisRecord1){
                    count[i]++;
                }
            }
         }

        string str(){
            vector<int> V(15);
            string str;
            std::iota(V.begin(),V.end(),0);
            sort( V.begin(),V.end(), [&](int i,int j){return count[i]>count[j];} );
            str = "MRU\n";
            for (int i : V) {
                if (!gisRecord[i].feature_name.empty())
                    str += gisRecord[i].str() + '\n';
            }
            str += "LRU\n";
            return str;
        }
};
#endif //GIS_BUFFERPOOL_H
