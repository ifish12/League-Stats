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
#include <regex>
#include <curl/curl.h>
#include "json/json.h"
#include "key.h"

using namespace std;

string data; //will hold the url's contents

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer
    
    for (int c = 0; c<size*nmemb; c++)
    {
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

void test_regex_search(const std::string& input)
{
    std::regex rx("[0-9][0-9][0-9][0-9][0-9]*,");
    std::smatch match;
    
    if (std::regex_search(input.begin(), input.end(), match, rx))
    {
        string SummonerID = match.str();
        SummonerID.erase (SummonerID.begin()+8);
        cout << "SummonerID = " << SummonerID << '\n';
       
    }
    else
        std::cout << "No match\n";
}


int main()
{
    char name[25] = "";
    smatch match;
    
    char dURL1[100] = "https://prod.api.pvp.net/api/lol/na/v1.4/summoner/by-name/";
    char dURL2[100] = "?api_key=";
    char dURL3[100] = API_KEY;
    cout << "What is your summoner name?" << endl;
    cin >> name;
    
    char fURL[225];
    
    strcat(fURL, dURL1);
    strcat(fURL, name);
    strcat(fURL, dURL2);
    strcat(fURL, dURL3);
    cout << fURL << endl;
    
    
    CURL* curl; //our curl object
    
    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_URL, fURL );
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress
    
    curl_easy_perform(curl);
    
    cout << endl << data << endl;
    //test_regex_search(data);
    
    // Let's parse it
    Json::Value root;
    Json::Reader reader;
    bool parsedSuccess = reader.parse(data, root, false);
    root = root[root.getMemberNames()[0]];
    cout << root["id"];
    cin.get();
    
    
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}