#ifndef GIS_GISRECORD_H
#define GIS_GISRECORD_H

#include <string>
#include <fstream>
#include <utility>
#include <vector>


using namespace std;

struct DMS{
    int degrees;
    int minutes;
    int seconds;
    char direction;
};

DMS create_DMS(const string& string1) {
    DMS dms{};
    string degrees, minutes, seconds;
    char direction;

    if (string1.back() == 'W' or string1.back() == 'S'){
        if(string1.length() == 9){
            degrees = '-';
            degrees += string1[1];
            degrees += string1[2];
            degrees += string1[3];
            minutes = string1[4];
            minutes += string1[5];
            seconds = string1[6];
            seconds += string1[7];
            direction = string1[8];
        }
    }

    if (string1.length() == 8 and string1[0] == '0') {
        degrees = '-';
        degrees += string1[1];
        degrees += string1[2];
        minutes = string1[3];
        minutes += string1[4];
        seconds = string1[5];
        seconds += string1[6];
        direction = string1[7];
    }
    else {
        degrees += string1[0];
        degrees += string1[1];
        minutes = string1[2];
        minutes += string1[3];
        seconds = string1[4];
        seconds += string1[5];
        direction = string1[6];
    }
    dms.direction = direction;
    dms.seconds = stoi(seconds);
    dms.minutes = stoi(minutes);
    dms.degrees = stoi(degrees);
    return dms;
}

bool contains(const vector<DMS>& world_dms, DMS dms2, DMS dms3){
    if (world_dms[0].degrees<dms2.degrees or world_dms[0].minutes<dms2.minutes ) return false;
    if (world_dms[1].degrees>dms2.degrees or world_dms[1].minutes>dms2.minutes ) return false;
    if (world_dms[2].degrees>dms3.degrees or world_dms[2].minutes>dms3.minutes ) return false;
    if (world_dms[3].degrees<dms3.degrees or world_dms[3].minutes<dms3.minutes ) return false;
    return true;
}

class GISRecord{
//private:
public:
    GISRecord();
    int feature_id{};
    string feature_name;
    string feature_class;
    string state_alpha;
    int state_numeric{};
    string county_name;
    int county_numeric{};
    DMS primary_lat_dms{};
    DMS prim_long_dms{};
    string prim_lat_dec;
    string prim_long_dec;
    DMS source_lat_dms{};
    DMS source_long_dms{};
    string source_lat_dec;
    string source_long_dec;
    int elev_in_m{};
    int elev_in_ft{};
    string map_name;
    string date_created;
    string date_edited;

public:
    explicit GISRecord(vector<string> args){
        for (int i = 0; i < args.size(); ++i) {
            if (args[i].empty()){
                args[i]="";
            }
        }
        feature_id = stoi(args[0]);
        feature_name = args[1];
        feature_class = args[2];
        state_alpha = args[3];
        state_numeric = stoi(args[4]);
        county_name = args[5];
        county_numeric = stoi(args[6]);
        primary_lat_dms = create_DMS(args[7]);
        prim_long_dms = create_DMS(args[8]);
        prim_lat_dec = args[9];
        prim_long_dec = args[10];
        if (!args[11].empty())source_lat_dms =create_DMS(args[11]);
        else source_lat_dms={};
        if (!args[12].empty()) source_long_dms = create_DMS(args[12]);
        else source_long_dms={};
        source_lat_dec=args[13];
        source_long_dec=args[14];
        if (args[15].empty() or args[16].empty()){
        elev_in_m=0;
        elev_in_ft=0;}
        else{
            elev_in_m= stoi(args[15]);
            elev_in_ft= stoi(args[16]);}
        map_name = args[17];
        date_created = args[18];
        date_edited = args[19];
     }


     bool operator==(const GISRecord& gisRecord){
         if (this->feature_id == gisRecord.feature_id){
             return true;
         }
        return false;
    }

    string str(){
        return to_string(feature_id)+"|"+feature_name+"|"+feature_class +"|"+state_alpha+"|"+ to_string(state_numeric)+"|"+county_name+"|"+
                                                                                                                                       to_string(county_numeric)+"|"+
                                                                                                                                                                to_string(primary_lat_dms.degrees)+
                                                                                                                                                                to_string(primary_lat_dms.minutes)+to_string(primary_lat_dms.seconds)+primary_lat_dms.direction+"|"+
                                                                                                                                                                                                   to_string(prim_long_dms.degrees)+to_string(prim_long_dms.minutes)+to_string(prim_long_dms.seconds)+to_string(prim_long_dms.direction)+"|"+prim_lat_dec+"|"+prim_long_dec+"|"+
                                                                                                                                                                                                                                                                                                                                                                             to_string(source_lat_dms.degrees)+
                                                                                                                                                                                                                                                                                                                                                                             to_string(source_lat_dms.minutes)+
                                                                                                                                                                                                                                                                                                                                                                             to_string(source_lat_dms.seconds)+source_lat_dms.direction+"|"+ to_string(source_long_dms.degrees)+ to_string(source_long_dms.minutes)+ to_string(source_long_dms.seconds)+source_long_dms.direction
        +"|"+ source_lat_dec
        +"|"+ source_long_dec
        +"|"+ to_string(elev_in_m)
        +"|"+ to_string(elev_in_ft)
        +"|"+ map_name
        +"|"+ date_created
        +"|"+ date_edited;
    }
};

GISRecord::GISRecord () = default;


#endif //GIS_GISRECORD_H
