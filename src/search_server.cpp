#include "search_server.h"


std::vector<std::vector<RelativeIndex>>
SearchServer::search(const std::vector<std::string> &queries_input)
{
    std::vector<std::vector<RelativeIndex>> result{};

    for (const auto& request : queries_input)
    {
        std::vector<RelativeIndex> relativeIndexes;
        std::map<size_t, size_t> mapIdCount;

        for (const auto& word : getSetUniqWords(request))
        {
            std::vector<Entry> entries = _index.GetWordCount(word);

            if (!entries.empty())
            {
                for (auto entry : entries){
                    auto it = mapIdCount.find(entry.doc_id);
                    if (it != mapIdCount.end())
                        mapIdCount[entry.doc_id] += entry.count;
                    else
                        mapIdCount[entry.doc_id] = entry.count;
                }
            }
        }


        auto iterMaxValuePair = std::max_element(mapIdCount.begin(), mapIdCount.end(),
                                                            [](auto p1, auto p2)
        {
            return p1.second < p2.second;
        });

        size_t maxAbsolRelevance = (*iterMaxValuePair).second;

        for (auto it = mapIdCount.begin(); it != mapIdCount.end(); ++it) {
            float rank;
            if (maxAbsolRelevance != 0)
                rank  = (float)(it->second)/maxAbsolRelevance ;
            else
                rank = 0.0;

            relativeIndexes.push_back({it->first, rank});
        }

        std::sort(relativeIndexes.begin(), relativeIndexes.end(),
                 [](RelativeIndex &l, RelativeIndex &r)
        {
                 return l.rank > r.rank ||
                        (l.rank == r.rank && l.doc_id < r.doc_id);
        });

        if (relativeIndexes.size() > max_responses )
            relativeIndexes.resize(max_responses);

        result.push_back(relativeIndexes);
    }

    return result;
}


std::set<std::string> SearchServer::getSetUniqWords(const std::string& text)
{
    std::set<std::string> words;
    std::istringstream ist(text);

    while (ist) {
        std::string word;
        ist >> word;
        if (!word.empty())
            words.insert(word);
    }

    return words;
}

void SearchServer::setMaxResponses(int n)
{
    if (n > 0) max_responses = n;
}