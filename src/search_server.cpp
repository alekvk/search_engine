#include "search_server.h"

std::set<std::string> SearchServer::getSetUniqWords(const std::string& request)
{
    std::set<std::string> result;
    
    for (auto word : SplitIntoWords(request)) 
        result.insert(word);

    return result;
}

std::vector<std::pair<std::string, int>> 
SearchServer::getSortWordsEntries(const std::set<std::string>& words)
{
    std::vector<std::pair<std::string, int>> result;

    for (const auto& word : words)
    {
        auto wordEntries = _index.GetWordCount(word);
        size_t wordEntriesSum = 0;

        for (auto wordEntry : wordEntries)
            wordEntriesSum += wordEntry.count;
        
        result.push_back({word, wordEntriesSum});
    }

    std::sort(result.begin(), result.end(), [](auto &l, auto &r)
    {
        return l.second < r.second;
    });

    return result;
}


std::vector<size_t> 
SearchServer::getListIds(std::string word)
{
    std::vector<size_t> result;

    for (auto entry : _index.GetWordCount(word))
        result.push_back(entry.doc_id);

    return result;
}


size_t SearchServer::getAbsolRelevForDocument(size_t docId, std::set<std::string> &uniqWords) {
   
    size_t absoluteRelevance = 0;

    for (const auto& word : uniqWords) {
       size_t wordCountInDoc = getWordCountInDoc(word, docId);
       absoluteRelevance += wordCountInDoc;
    }

    return absoluteRelevance;
}
 


size_t SearchServer::getWordCountInDoc(const std::string& word, const size_t doc_id) 
{
    auto vectorEntry = _index.GetWordCount(word); 
    auto beg = vectorEntry.begin();
    auto end = vectorEntry.end();
    
    auto it = std::find_if(beg, end, [doc_id](auto p)
        {
            return p.doc_id == doc_id;
        });

    if (it == end)
        return 0;
    else
        return it->count;
}

std::vector<std::vector<RelativeIndex>>
SearchServer::search(const std::vector<std::string>& queries_input)
{

    std::vector<std::vector<RelativeIndex>> result{};

    if (queries_input.empty())
    {
        std::cout << "Requests are empty!"<<std::endl;
        return result;
    }

    for (const auto& query_input : queries_input) {
               
        auto uniqWords = getSetUniqWords(query_input);
        if (uniqWords.empty())
        {
            std::cout << "Empty request\n";
            continue;
        }

        
        auto wordsEntries = getSortWordsEntries(uniqWords);
       
        auto conteinerIdDocs = getListIds(wordsEntries[0].first);

        std::vector<RelativeIndex> relativeIndexes;
        size_t maxAbsolRelevance {0};

        for (auto docId : conteinerIdDocs) {
            size_t absolRelevance = getAbsolRelevForDocument(docId, uniqWords);
            RelativeIndex relIndex;
            relIndex.doc_id = docId;
            relIndex.rank = absolRelevance;
            relativeIndexes.push_back(relIndex);
          
            if (absolRelevance > maxAbsolRelevance) 
                maxAbsolRelevance = absolRelevance;
        }

        for (auto& relativeIndex : relativeIndexes) {
            if (maxAbsolRelevance != 0)
                relativeIndex.rank /=(float) maxAbsolRelevance;
            else relativeIndex.rank = 0;
        }

        std::sort(relativeIndexes.begin(), relativeIndexes.end(),
                 [](RelativeIndex &l, RelativeIndex &r)
                      {
                           return (l.rank > r.rank );
                      });

        result.push_back(relativeIndexes);
    }

    return result;
}

std::vector<std::string> 
SearchServer::SplitIntoWords(const std::string& text) 
{
    std::vector<std::string> words;
    std::string word;
    for (const char ch : text) {
        if (!((ch >= 48 && ch <= 57) || (ch >= 64 && ch <= 90) 
            || (ch >= 97 && ch <= 122)) ) 
        {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += std::tolower(static_cast<unsigned char>(ch));;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}