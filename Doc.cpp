#include "Doc.h"

Doc::Doc(){
    list=new Repo();
    load();
}

Doc::~Doc(){
    save();
    delete list;
}

void Doc::save(){
    std::ofstream file("list.txt");
    file << list->print();
    file.close();
}

void Doc::load(){
    std::ifstream file("list.txt");
    if(!file.is_open()){
        return;
    }
    // Loading logic
    std::string line;
    while(std::getline(file, line)){
        // Parse line and populate 'list'
        if(line.empty()){
            continue;
        }else if(line[0] == '!'){
            list->addTheme(line);
        }else if(line[0] == '<'){
            size_t symbolEnd = line.find('>');
            size_t parenStart = line.find('(');
            if(symbolEnd == std::string::npos || parenStart == std::string::npos) {
                continue; // Skip malformed lines
            }
            std::string symbol = line.substr(1, symbolEnd - 1);
            std::string theme = this->list->symbol(symbol);
            if(theme.empty()) {
                std::cout << "Warning: Symbol not found: " << symbol << std::endl;
                continue;
            }
            std::string textPlus = line.substr(parenStart);
            if(!this->list->addMedia(theme, textPlus)) {
                std::cout << "Failed to add media" << std::endl;
            }
        }
            
    }
    file.close();
}

Repo* Doc::getRepo(){
    return list;
}