#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

using namespace std;

int main(int argc, char* argv[]) {
  struct stat stats_final;
  struct stat attrib_nav_facts;
  stat("everything.plg", &stats_final);
  stat("navigation_facts.asp", &attrib_nav_facts);

  if (attrib_nav_facts.st_mtime > stats_final.st_mtime) {
    ifstream facts;
    facts.open("navigation_facts.asp");
  
    //reconstruct room/door data from the definitions
    string def;
    string rooms;
    string doors;
    string hasdoors;
    string acc;
  
    doors = "doors = {";
    rooms = "rooms = {";
    
    while (getline(facts, def)) {
      if ((def.find("room") == 0) || (def.find("door") == 0) ||
          (def.find("location") == 0)) {
        
        int start = def.find("(") + 1;
        string object = def.substr(start, def.find(")") - start);
        
        if (def[start] == 'd' && def.find("door") == 0) {
          doors += object + ", ";
          //cout << "door: " << object << endl;
        }
        if (def[start] == 'l' && (def.find("room") == 0 || def.find("location") == 0)) {
          rooms += object + ", ";
          //cout << "room: " << object << endl;
        }
        
      }
      else if (def.find("hasdoor") == 0) {
        hasdoors += def + "\n";
      }
      else if (def.find("acc") == 0) {
        acc += def + "\n";
      }
      //cout << def << endl;
    }
    size_t commas_doors = doors.find_last_of(",");
    if (commas_doors != string::npos) {
      doors.erase(commas_doors);
    }
    doors += "}.";
    
    size_t commas_rooms = rooms.find_last_of(",");
    if (commas_rooms != string::npos) {
      rooms.erase(commas_rooms);
    }
    rooms += "}.";

    cout << "Rebuilding data file" << endl;
    /*
    cout << doors << endl;
    cout << rooms << endl;
    */


  
    //cat everything together
    ofstream data;
    data.open("data.plg", ios_base::trunc);
    data << doors << endl;
    data << rooms << endl;
    data << hasdoors << endl;
    data << acc << endl;
    data.close();
    facts.close();
  }

  //See if any file is updated
  struct stat attrib_data;
  struct stat attrib_goal;
  struct stat attrib_probs;
  struct stat attrib_logic;
  stat("data.plg", &attrib_data);
  stat("goal.plg", &attrib_goal);
  stat("probabilities.plg", &attrib_probs);
  stat("doors_logic.plg", &attrib_logic);

  if (attrib_data.st_mtime > stats_final.st_mtime || attrib_goal.st_mtime > stats_final.st_mtime ||
      attrib_probs.st_mtime > stats_final.st_mtime || attrib_logic.st_mtime > stats_final.st_mtime) {
  
    ofstream final;
    ifstream plog;
    ifstream goal;
    ifstream data_in;
    ifstream probabilities;
  
    plog.open("doors_logic.plg", ios_base::binary);
    goal.open("goal.plg", ios_base::binary);
    data_in.open("data.plg", ios_base::binary);
    probabilities.open("probabilities.plg", ios_base::binary);
  
    final.open("everything.plg", ios_base::binary);

    final << data_in.rdbuf() << plog.rdbuf()
          << probabilities.rdbuf() << goal.rdbuf();
    cout << "Rebuilding everything" << endl;
  }
  
}
