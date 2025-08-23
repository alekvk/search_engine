#include "converter_json.h"

    
void ConverterJSON::configInit()
{
    nlohmann::json dict;
    std::ifstream file(config_path);

    if (file.is_open()) {
        file >> dict;
        file.close();
    } else {
        throw std::runtime_error ("Config file is missing!");
    }

    

    int count = 1;

    for (const auto& d : dict.items()) {
        if (count == 1) {
            if(d.key() != "config"){
                throw std::runtime_error("Config file is empty!");     
        }    
               
        for (const auto& j : d.value().items()) {
            if (j.key() == "name") 
                name_project = j.value();
            else if (j.key() == "version") 
                version = j.value();
            else if (j.key() == "max_responses") 
                max_responses = j.value();
        }     
        }
        if (count == 2) 
                files = d.value(); 
        
        ++count;
    }

    if (checkConfig())
        std::cout<<"\nSTART!\nProject name "<<name_project<<std::endl;
    else
        throw std::runtime_error("Configuration is not specified in the config.json file!  ");
}

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
    configInit();

    std::vector<std::string> result;
    
    for (auto file : files) {
        std::string path = file;
        std::string text; 

        try {
            text = readFile(path);    
        } catch (std::runtime_error err) {
            std::cout<<"\n"<<err.what()<<std::endl;
            continue;
        }
       
        result.push_back(text);
    }

    return result;
}

int ConverterJSON::GetResponsesLimit()
{
    return max_responses;
}

void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers)
{
    std::ofstream file(answers_path);

    if (file.is_open()) { 
        nlohmann::json jsDict;

        if (!answers.empty()) {
            int intRequestNum = 1;
                   
            for (auto request : answers) {
                std::string requestNum = getNameRequest(intRequestNum);
                jsDict["answers"][requestNum]["result"] = !request.empty();

                if (request.size() == 1) {
                    jsDict["answers"][requestNum]["docid"] = request[0].first;
                    jsDict["answers"][requestNum]["rank"] = request[0].second;
                
                } else {
                    auto jsArrayIdRel = nlohmann::json::array();
                    int relevId = 0;

                    for (auto pairIdRel : request) {
                        ++relevId;

                        if (relevId > max_responses) 
                            break;

                        auto jsIdRel = nlohmann::json::object();
                        jsIdRel["docid"] = pairIdRel.first;
                        jsIdRel["rank"] = round (pairIdRel.second * 1000 ) / 1000;
                        jsArrayIdRel.push_back(jsIdRel);
                    }

                    jsDict["answers"][requestNum]["relevance"] = jsArrayIdRel;
                }
                
                ++intRequestNum;
            }
        } else 
            jsDict["answers"] = "No answers to push.\n";
                
        file << jsDict;
        file.close();

    } else  
        std::cout<<"Error opening the answers.json file";
}    

std::vector<std::string> ConverterJSON::GetRequests()
{
    std::vector<std::string> req;
    nlohmann::json dict;
    std::ifstream file(requests_path);
   
    if (file.is_open()) 
        file >> dict;
    else 
        throw std::runtime_error ("The requests file is missing on the path: " + requests_path + ".");
    
    for (auto i : dict.items())
        req = i.value();
    
    return req;
}

std::string ConverterJSON::readFile(const std::string& path) 
{
    std::ifstream file(path);

    if(!file.is_open()) 
        throw std::runtime_error ("The file is missing on the path: " + path);    
    
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    std::string s(size, ' ');
    file.seekg(0);
    file.read(&s[0], size); 
    return s;
}

std::string ConverterJSON::getNameRequest(int n)
{
    std::string name;

    if (n <= 0 || n >= 999)
        name = "request1000";
    else if (n > 99)
        name = "request" + std::to_string(n);
    else if (n > 9)
        name = "request0" + std::to_string(n);     
    else 
        name = "request00" + std::to_string(n);

    return name;    
}

bool ConverterJSON::checkConfig()
{
    return (!name_project.empty() && !version.empty()
            && max_responses > 0);
}