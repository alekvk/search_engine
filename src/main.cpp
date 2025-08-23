#include <iostream>
#include "converter_json.h"
#include "inverted_index.h"
#include "search_server.h"

std::vector<std::vector<std::pair<int, float>>>
ConvertRelIndexToPair(const std::vector<std::vector<RelativeIndex>>& searchResult);


int main()
{
    ConverterJSON convJSON;
    std::vector<std::string> texts;
    std::vector<std::string> requests;
      
    try {
        texts = convJSON.GetTextDocuments();
        requests = convJSON.GetRequests();       
    } catch (std::runtime_error err) {
        std::cout<<err.what()<<"  Exit program!"<<std::endl;
        return 1;    
    }
    
    InvertedIndex invInd; 
    invInd.UpdateDocumentBase(texts);

    SearchServer searchServer(invInd);
    auto searchResult = searchServer.search(requests);

    convJSON.putAnswers(ConvertRelIndexToPair(searchResult));

    return 0;
}





std::vector<std::vector<std::pair<int, float>>>
ConvertRelIndexToPair(const std::vector<std::vector<RelativeIndex>>& searchResult)
{
    std::vector<std::vector<std::pair<int, float>>> result;

    for (auto vectRelIndex : searchResult) {
        std::vector<std::pair<int, float>> vectPair;

        for (auto RelIndex : vectRelIndex)
            vectPair.push_back({RelIndex.doc_id, RelIndex.rank});

        result.push_back(vectPair);
    }

    return result;
}