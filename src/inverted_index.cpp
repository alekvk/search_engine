#include <inverted_index.h>

std::mutex InvertedIndex::mutex_freq_dictionary;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
{
    std::size_t doc_id = 0;

    std::vector<std::thread> threads;

    for (auto document : input_docs) {
        threads.emplace_back([this, document, doc_id]() {
        indexingDocument(SplitIntoWords(document), doc_id);
        });
        ++doc_id;
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

void InvertedIndex::indexingDocument(const std::vector<std::string>& document, int doc_id)
{
    std::map<std::string, Entry> indexDocument;
    
    for (auto word : document) {
        indexDocument[word].doc_id = doc_id;  
        ++indexDocument[word].count;        
    }

    mutex_freq_dictionary.lock();

    for (auto i : indexDocument) {
        if(!freq_dictionary.count(i.first))
            freq_dictionary[i.first] = {i.second};       
        else 
            freq_dictionary[i.first].push_back(i.second);          
    }

    mutex_freq_dictionary.unlock(); 
}
   

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word)
{
    return freq_dictionary[word];
}



std::vector<std::string> 
InvertedIndex::SplitIntoWords(const std::string& text) 
{

    std::vector<std::string> words;
    std::istringstream ist(text);

    while (ist) {
        std::string word;
        ist >> word;
        std::transform(word.begin(), word.end(), word.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        if (!word.empty())
            removePunctuatMark(word);
            words.push_back(word);
    }

    return words;
}

void InvertedIndex:: removePunctuatMark(std::string& word)
{
    std::vector<char> charASCII {21,40,41,44,46,58,59,63};

    while (true) {
        auto result {std::find(begin(charASCII), end(charASCII),
                     word[word.length() - 1])};
        if (result == end(charASCII))
            return;
        else
            word.erase(word.length() - 1);
    }
}