//
//  main.cpp
//  Test
//
//  Created by Geoff Shapiro on 4/14/14.
//  Copyright (c) 2014 Geoff Shapiro. All rights reserved.
//
#include <iostream>
#include <string>
#include <cstring>
#include <curl/curl.h> // doesn't work in windows, consider moving to boost or some other library in the future
#include "json/json.h"
#include "key.h"

using namespace std;

CURL* curl; //our curl object
Json::Value root;
Json::Reader reader;
string sID;
string data; //will hold the url's contents
char name[25] = "";

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{   //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer
    
    for (int c = 0; c<size*nmemb; c++)
    {
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

void GetMatchHistory()
{
    
    char mURL1[100] = "https://prod.api.pvp.net/api/lol/na/v1.3/game/by-summoner/";
    int rURL2i = root["id"].asInt();
    char rURL2[100];
    snprintf(rURL2, 100, "%d", rURL2i);
    char rURL3[100] = "/recent?api_key=";
    char rURL4[100] = API_KEY;
    char rmURL[400] = {};
    
    strcat(rmURL, mURL1);
    strcat(rmURL, rURL2);
    strcat(rmURL, rURL3);
    strcat(rmURL, rURL4);
    
    cout << rmURL << endl;
    
    curl_easy_setopt(curl, CURLOPT_URL, rmURL );
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);

    curl_easy_perform(curl);
    
    bool parsedSuccess = reader.parse(data, root, false);
    cout << root["games"][2]["fellowPlayers"][2]; // The number here is static for now but it will output that specific game from the JSON Riot gives us.
    // TODO: Implement a nice way to get the individual values and display them

}

void GetCurrentRankedLeague()
{
    cout << "This is a stub for GetCurrentRankedLeague()" << endl;
}


void DisplayMenu()
{
    cout << "Hello Summoner! What would you like to check up for " << name << "?" << endl;
    cout << "1: Match history" << endl;
    cout << "2: Current ranked League" << endl;
    cout << "3: Nothing I want to leave" << endl;
    int choice = NULL;
    cin >> choice;
    switch (choice) {
        case 1:
            GetMatchHistory();
            break;
        case 2:
            GetCurrentRankedLeague();
            break;
            
        default:
            break;
    }
}


int main()
{
    
    char dURL1[100] = "https://na.api.pvp.net/api/lol/na/v1.4/summoner/by-name/";
    char dURL2[100] = "?api_key=";
    char dURL3[100] = API_KEY;
    cout << "Which summoner do you wish to lookup?" << endl;
    cin >> name;
    
    char fURL[225];
    
    strcat(fURL, dURL1);
    strcat(fURL, name);
    strcat(fURL, dURL2);
    strcat(fURL, dURL3);
    cout << fURL << endl;
    
    
    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_URL,fURL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress
    
    curl_easy_perform(curl);
    
    cout << endl << data << endl;
    
    // Let's parse it

    bool parsedSuccess = reader.parse(data, root, false);
    root = root[root.getMemberNames()[0]];
    cout << root["id"];

    data = "";
    DisplayMenu();
    cin.get();
    
    
    
    
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}