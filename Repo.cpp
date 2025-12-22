#include "Repo.h"

Repo::Repo(){

}

Repo::~Repo(){
    for(auto cat : content){
        delete cat;
    }
}

Category* Repo::addTheme(std::string text){
    auto left= text.find('<');
    if(left==std::string::npos){
        return nullptr;
    }
    auto right= text.find('>',left+1);
    if(right==std::string::npos || right<=left+1){
        return nullptr;
    }
    std::string symbol= text.substr(left+1,right-left-1);
    auto pipe= text.find('|', right+1);
    if(pipe==std::string::npos || pipe<=right+1){
        return nullptr;
    }
    std::string title= text.substr(right+1, pipe-right-1);
    auto open= text.find('{', pipe+1);
    if(open==std::string::npos){
        return nullptr;
    }
    auto close= text.find('}', open+1);
    if(close==std::string::npos || close<=open+1){
        return nullptr;
    }
    std::string measures_str= text.substr(open+1, close-open-1);
    std::vector<std::string> measures;
    size_t start = 0;
    size_t end = measures_str.find(',');
    while (end != std::string::npos) {
        measures.push_back(measures_str.substr(start, end - start));
        start = end + 1;
        end = measures_str.find(',', start);
    }
    measures.push_back(measures_str.substr(start));
    Category* category = new Category(title, symbol, measures);
    content.push_back(category);
    return category;
}

Category* Repo::addTheme(std::string title, std::string symbol, std::vector<std::string> measures){
    Category* category = new Category(title, symbol, measures);
    for(auto cat : content){
        if(cat->cat(title)!=nullptr){
            std::cout<<"Category already exists"<<std::endl;
            std::cout<<"Please use a different title for the new category."<<std::endl;
            delete category;
            return nullptr;
        }
        if(cat->getSymbol() == symbol){
            std::cout<<"Symbol already exists"<<std::endl;
            std::cout<<"Please use a different symbol for the new category."<<std::endl;
            delete category;
            return nullptr;
        }
    }
    content.push_back(category);
    return category;
}

Category* Repo::getTheme(std::string title){
    for(auto cat : content){
        Category* catPtr = cat->cat(title);
        if(catPtr!=nullptr){
            return catPtr;
        }
    }
    return nullptr;
}

Media* Repo::getMedia(std::string theme, std::string title){
    Category* cat = getTheme(theme);
    if(cat==nullptr){
        std::cout<<"Category not found"<<std::endl;
        std::cout<<"Please add the theme first before accessing the theme first."<<std::endl;
        return nullptr;
    }
    MedP* medp = cat->getChild(title);
    if(medp==nullptr){
        std::cout<<"Media not found"<<std::endl;
        std::cout<<"Please add the media first before accessing it."<<std::endl;
        return nullptr;
    }
    Media* media = dynamic_cast<Media*>(medp);
    return media;
}

Media* Repo::addMedia(std::string theme, std::string text){
    Category* cat = getTheme(theme);
    std::string title=text.find('(')!=std::string::npos ? text.substr(text.find('(')+1, text.find(')') - text.find('(') - 1) :"";
    if(title==""){
        std::cout<<"Invalid media format"<<std::endl;
        return nullptr;
    }
    if(cat==nullptr){
        std::cout<<"Category not found"<<std::endl;
        std::cout<<"Please add the theme first before adding media to it."<<std::endl;
        return nullptr;
    }
    std::vector<int> counts;
    size_t openBrace = text.find('{');
    size_t closeBrace = text.find('}');
    if (openBrace != std::string::npos && closeBrace != std::string::npos && closeBrace > openBrace) {
        std::string counts_str = text.substr(openBrace + 1, closeBrace - openBrace - 1);
        size_t start = 0;
        size_t end = counts_str.find(',');
        while (end != std::string::npos) {
            counts.push_back(std::stoi(counts_str.substr(start, end - start)));
            start = end + 1;
            end = counts_str.find(',', start);
        }
        counts.push_back(std::stoi(counts_str.substr(start)));
    }
    Media* media = new Media(cat->getMeasures(),title);
    int index=0;
    for(int count : counts){
        std::cout<<"Adding count: " << count << " to media: " << title << std::endl;
        media->addCount(index,count);
        index++;
    }
    std::string statusPart = text.substr(text.find("\\\\") + 2);
    media->changeStatus(statusPart);
    
    if(cat->getChild(title)!=nullptr){
        std::cout<<"Media already exists"<<std::endl;
        std::cout<<"Please use a different title for the new media."<<std::endl;
        delete media;
        return nullptr;
    }
    cat->add(media);
    return media;
}

Media* Repo::addMedia(std::string theme, Media* media){
    Category* cat = getTheme(theme);
    if(cat==nullptr){
        std::cout<<"Category not found"<<std::endl;
        std::cout<<"Please add the theme first before adding media to it."<<std::endl;
        return nullptr;
    }
    if(cat->getChild(media->getTitle())!=nullptr){
        std::cout<<"Media already exists"<<std::endl;
        std::cout<<"Please use a different title for the new media."<<std::endl;
        return nullptr;
    }
    cat->add(media);
    return media;
}

std::string Repo::print(){
    std::string result;
    for(auto cat : content){
        result+=cat->print();
    }
    for(auto cat : content){
        result+=cat->view();
    }
    return result;
}

void Repo::changeMedia(std::string theme, std::string text,int index, int No){
    Media* media = getMedia(theme, text);
    if(media==nullptr){
        return;
    }
    media->addCount(index, No);
}

void Repo::status(std::string theme, std::string text, std::string status){
    Media* media = getMedia(theme, text);
    if(media==nullptr){
        return;
    }
    media->changeStatus(status);
}

std::string Repo::symbol(std::string symbol){
    for(auto cat : content){
        if(cat->getSymbol() == symbol){
            return cat->getTitle();
        }
    }
    return "";
}
