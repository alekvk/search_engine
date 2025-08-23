#pragma once

#include <vector>
#include <set>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "inverted_index.h"

struct RelativeIndex
{
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const 
    {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};
 

class SearchServer
{

public:
    /**
     * @param idx в конструктор класса передаётся ссылка на класс
    InvertedIndex,
     * чтобы SearchServer мог узнать частоту слов встречаемых в
    запросе
    */    
    SearchServer(InvertedIndex& idx) : _index(idx){ };

    /**
     * Метод обработки поисковых запросов
     * @param queries_input поисковые запросы взятые из файла
       requests.json
     * @return возвращает отсортированный список релевантных ответов для
      заданных запросов
    */
    std::vector<std::vector<RelativeIndex>> search(const
        std::vector<std::string>& queries_input);

    
       
private:
    InvertedIndex _index;
    std::vector<std::string> SplitIntoWords(const std::string& text);
    std::set<std::string> getSetUniqWords(const std::string& request);
    std::vector<std::pair<std::string, int>> 
        getSortWordsEntries(const std::set<std::string>& words);
    size_t getWordCountInDoc(const std::string& word, const size_t doc_id);
    std::vector<size_t> getListIds(std::string word); 
    size_t getAbsolRelevForDocument(size_t docId, std::set<std::string> &uniqWords);
};











//void addDocuments(const vectorStr& documents);
    //std::vector<vectorPairIntFloat> getSearchResults(const vectorStr& requests, int limit);


//std::map<std::string, std::set<int>> search_index;
//vectorPairIntFloat findTopDocuments(const vectorStr& request, int limit) const;
//vectorPairIntFloat findDocuments(const vectorStr& request) const;   

