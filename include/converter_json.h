#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include "nlohmann/json.hpp"



class ConverterJSON 
{
public:
    ConverterJSON() = default;
    
    /**
     * The method of getting the contents of files
     * @return Returns a list with the contents of the files listed
     * in config.json
     */
    std::vector<std::string> GetTextDocuments();

    /**
     * The method reads the field max_responses to determine the maximum
     *  number of responses per request
     * @return
     */
    int GetResponsesLimit();
    
    /**
    * Method of getting requests from a file requests.json
    * @return retrieves a list of requests from a file requests.json
    */
    std::vector<std::string> GetRequests();

    /**
     * Put it in a file answers.json search query results
     */
    void putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers);
        
private:
    void configInit();
    const std::string json_path = "..\\..\\json_files\\";
    const std::string config_path = json_path + "config.json";
    const std::string requests_path = json_path + "requests.json";
    const std::string answers_path = json_path + "answers.json.";
    std::string name_project;
    std::string version; 
    int max_responses = 0;
    std::vector<std::string> files;
    std::string readFile(const std::string& path);
    std::string getNameRequest(int n);
    bool checkConfig();
};
