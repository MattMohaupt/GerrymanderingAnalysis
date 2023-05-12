//
//  Project 1 Starter Code - Gerrymandering
//  Matthew Mohaupt
//  University of Illinois Chicago
//  CS 251, Spring 2023

#include "ourvector.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
#include <cctype>

using namespace std;

struct districtinfo{
    string location;
    int howmanydistricts;
    map<int, int> bluevoters;
    map<int, int> redvoters;
};

struct eliginfo{
    string location;
    int totalvoters;
};

//parsing district data to see how many party voters are in a state's districts
//taking in the line of data and returning through the struct of districtinfo to a fully parsed data with easy ways of getting at all the info
void parsedistrictdata(const string &holder, districtinfo &container){
    string copy = holder;
    int index = 0;
    int count = 0;
    int number = 0;

    index = copy.find(",");
    container.location = copy.substr(0, index);
    copy.erase(0, index + 1);

    while(true){
        index = copy.find(",");

        //making sure it isnt looping and finding nothing so index's value doesnt change and filling the struct with garbage data
        if(index < 0){
            break;
        }

        //finding district number and checking to make sure if there is just one
        try{
            number = stoi(copy.substr(0, index));
            throw(number);      
        }
        catch(...){
            number = 1;
        }    
        count++;      
        copy.erase(0, index + 1);
        

        //finding blue votes
        index = copy.find(",");
        container.bluevoters[number] = stoi(copy.substr(0, index));
        copy.erase(0, index + 1);

        //finding red votes
        index = copy.find(",");
        container.redvoters[number] = stoi(copy.substr(0, index));
        copy.erase(0, index + 1);
    }

    container.howmanydistricts = count;

}

//only need to parse 2 pieces of data so only need to find 1 comma and then parse the data that way and modify the eliginfo sturct
void parsedeligdata(const string &holder, eliginfo &container){
    string copy = holder;
    int index = 0;
    int size;
    index = copy.find(",");
    container.location = copy.substr(0, index);
    copy.erase(0, index + 1);
    size = copy.size();
    container.totalvoters = stoi(copy.substr(0, size));
}

//loading in district data by pushing the data in filename into ourvector input with the type of districtinfo
void loaddisdata(ourvector<districtinfo> &input, const string &filename){
    string tempgrab;
    districtinfo data;
    ifstream ifs;
    ifs.open(filename);
    while(getline(ifs, tempgrab)){
        if(ifs.fail()){
            break;
        }
        parsedistrictdata(tempgrab, data);
        input.push_back(data);
        cout << "..." << data.location << "..." << data.howmanydistricts << " districts total" << endl;
    }
    ifs.close();
}

//loading in eligible voter data by pushing the date in filename into ourvector input with the type of eliginfo and checks if the area is in the ourvector for district info
void loadeligdata(ourvector<eliginfo> &input, const string &filename, ourvector<districtinfo> &checker){
    string tempgrab;
    eliginfo data;
    ifstream ifs;
    ifs.open(filename);
    while(getline(ifs, tempgrab)){
        if(ifs.fail()){
            break;
        }
        parsedeligdata(tempgrab, data);
        for(auto element: checker){
            if(element.location == data.location){
                input.push_back(data);
                cout << "..." << data.location << "..." << data.totalvoters << " eligible voters" << endl;
            }
        }
    }
    ifs.close();
}

//makes everything lowercase except first char so no such thing as case sensitive
string loweritall(const string og){
    string newg = og;
    newg.at(0) = toupper(newg.at(0));
    for(int i = 1; i < newg.size(); i++){
        newg.at(i) = tolower(newg.at(i));
    }

    return newg;
}

int main() {
    //TODO:  Write your code here.
    // You should have LOTS of function decomposition.

    string command, districts, eligibility, states; 
    ifstream ifz;
    ourvector<districtinfo> di;
    ourvector<eliginfo> ei;
    string loadedstate = "N/A";
    bool fail1 = true;
    bool fail2 = true;
    bool loaded = false;
    bool runner = true;


    cout << "Welcome to the Gerrymandering App!" << endl;

    while(runner){
        //to clear the buffer
        cout << endl;

        //checking to see what data and state is currently loaded
        cout << "Data loaded? ";
        if(!loaded){
            cout << "No" << endl;
        }
        else{
            cout << "Yes" << endl;
        }
        cout << "State: " << loadedstate << endl;
        cout << endl;


        cout << "Enter command: " << endl;
        cin >> command;
        cout << "\n-----------------------------\n";


//when load is called then it will check one at a time if districts and eleigibilty can be opened, if so then they will be sent to go get parsed and put in their respective vectors
//if load has already been called once then it cant be called again and once both are correctly loaded in then it will set fail1 and fail2 to false both and make it inaccessible
        if( command == "load"){
            if( (!fail1) && (!fail2)){
                cout << "Already read data in, exit and start over." << endl;
                cin.clear();
                continue;
            }
            cin >> districts;
            cin >> eligibility;
            cout << endl;
            cin.clear();
            ifz.open(districts);           
            if(!(ifz.fail())){
                if(fail1){
                    fail1 = false;
                    ifz.close();
                    cout << "Reading: " << districts << endl;
                    loaddisdata(di, districts); 
                    cout << endl;
                }           
            }
            else{
                cout << "Invalid first file, try again." << endl;
                cin.clear();
                fail1 = true;
                ifz.close();
                continue;
            }
            ifz.close();

            ifz.open(eligibility);
            if(!(ifz.fail())){
                if(fail2){
                    fail2 = false;
                    ifz.close();
                    cout << "Reading: " << eligibility << endl;
                    loadeligdata(ei, eligibility, di);
                    cout << endl;
                }               
            }
            else{
                cout << "Invalid second file, try again." << endl;
                cin.clear();
                fail2 = true;
                ifz.close();
                continue;
            }
            ifz.close();

            if( (!fail1) && (!fail2)){
                loaded = true;
            }
            else{
                loaded = false;
            }

        }


//searching for a specific state to be loaded but data needs to be loaded first and must be a correct name
        else if (command == "search"){
            bool nostate = true;
            cin >> states;
            string llocation;
            string lstate = loweritall(states);
            cin.clear();
            cout << endl;
            if(!fail1){
                for(auto element: di){
                    llocation = loweritall(element.location);
                    if(lstate == llocation){
                        loadedstate = lstate;
                        nostate = false;
                    }
                }
                if(nostate){
                    cout << "State does not exist, search again." << endl;
                }
            }
            else{
                cout << "No data loaded, please load data first." << endl;
            }
        }
        else if (command == "stats"){
            cout << endl;
            int allred = 0;
            int allblue = 0;
            string llocation;

            for(auto element : di){
                llocation = loweritall(element.location);
                if(loadedstate == llocation){
                    //stuff
                }
            }
            
        }
        else if (command == "plot"){
            
        }
        else if (command == "exit"){
            runner = false;
        }
        else{
            cin.clear();
            cout << "Enter a correct command" << endl;  
        }

    }
    


    return 0;
}