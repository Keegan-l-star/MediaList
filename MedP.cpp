#include "MedP.h"
MedP::MedP(std::string title){
    this->title = title;
}

void MedP::add(MedP* Me){
    
}

MedP* MedP::getChild(std::string text){
    if(this->title == text){
        return this;
    }
    return nullptr;
}

Media::Media(std::vector<std::string> counters, std::string title): MedP(title){
    this->counters = counters;
    this->counts = std::vector<int>(counters.size(), 1);
}

Media::Media(std::string title): MedP(title){
}

Media::~Media(){
}

std::string Media::print(){
    std::string result = "(" + this->title + "){";
    for (size_t i = 0; i < this->counts.size(); ++i) {
        result += std::to_string(this->counts[i]);
        if (i != this->counts.size() - 1) {
            result += ",";
        }
    }
    result += "}";
    return result;
}

void Media::changeStatus(std::string text){
    if(text=="Ongoing"){
        //change status to Ongoing
        status=Status::Ongoing;
    }else if(text=="Interested"){
        //change status to Interested
        status=Status::Interested;
    }else if(text=="Finished"){
        //change status to Finished
        status=Status::Finished;
    }else if(text=="Dropped"){
        //change status to Dropped
        status=Status::Dropped;
    }else if(text=="Ended"){
        //change status to Ended
        status=Status::Ended;
    }else{
        std::cout<<"Invalid Status"<<std::endl;
    }
}

std::string Media::view(){
    std::string result;
    result+="Title: "+this->title+"\n";
    result+="\tStatus: "+std::to_string(static_cast<int>(this->status))+"\n";
    for(int i=0;i<this->counters.size();i++){
        result+=this->counters[i]+": "+std::to_string(this->counts[i])+"\n";
    }
    return result;
}

void Media::addCount(){
    this->counts[counts.size()-1]++;
}

void Media::addCount(int index, int count){
    if(index<0 || index>=this->counts.size()){
        std::cout<<"Invalid Index"<<std::endl;
        return;
    }
    this->counts[index]+=count;
}

void Media::addCounters(std::vector<std::string> text){
    if(text.size()==0){
        std::cout<<"No Counters to Add"<<std::endl;
        return;
    }else if(counters.size()>=0){
        counters = text;
    }else{
        for(int i=0;i<text.size();i++){
            counters.push_back(text[i]);
            counts.push_back(0);
        }
    }
}

void Media::addCounters(){
    int n;
    std::cout<<"Enter number of counters to add: ";
    std::cin>>n;
    std::cin.ignore();
    for(int i=0;i<n;i++){
        std::string counter;
        std::cout<<"Enter counter name: ";
        std::getline(std::cin,counter);
        counters.push_back(counter);
        counts.push_back(1);
    }
}

Category::Category(std::string title, std::string symbol, std::vector<std::string> measures): MedP(title){
    this->symbol = symbol;
    this->measures = measures;
}

Category::~Category(){
    for(int i=0;i<group.size();i++){
        delete group[i];
    }
}

void Category::add(MedP* Me){
    group.push_back(Me);
}

void Category::add(std::string Me){
    Media* media = new Media(measures, Me);
    group.push_back(media);
}

MedP* Category::getChild(std::string text){
    for(int i=0;i<group.size();i++){
        if(group[i]->getChild(text)!=nullptr){
            return group[i]->getChild(text);
        }
    }
    return nullptr;
}

std::string Category::print(){
    std::string result;
    result+="![<"+this->symbol+">"+this->title+"|{";
    for(int i=0;i<measures.size();i++){
        result+=measures[i];
        if(i!=measures.size()-1){
            result+=",";
        }
    }
    result+="}]";
    return result;
}

std::string Category::view(){
    std::string result;
    for(int i=0;i<group.size();i++){
        result+="<"+this->symbol+">"+group[i]->print();
    }
    return result;
}

