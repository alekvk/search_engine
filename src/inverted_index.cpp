#include <inverted_index.h>

std::mutex InvertedIndex::mutex_freq_dictionary;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
{
    std::size_t doc_id = 0;
    
    for (auto document :input_docs) {

        std::thread index([this, document, doc_id]()
            {indexingDocument(SplitIntoWords(document), doc_id);});
        ++doc_id;
        index.join();
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
    std::string word;
    for (const char c : text) {
        if (!((c >= 48 && c <= 57) || (c >= 64 && c <= 90) 
            || (c >= 97 && c <= 122)) ) 
        {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += std::tolower(static_cast<unsigned char>(c));
            
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}