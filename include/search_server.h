#pragma once

#include <vector>
#include <set>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

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

    /**
    * Установка максимального количество ответов с поискового сервера
    * @param n - новое значение для максимального количества ответов
    */
    void setMaxResponses(int n);

private:
    size_t max_responses = 5;
    InvertedIndex _index;
    std::set<std::string> getSetUniqWords(const std::string& request);
};