#ifndef REPO_H
#define REPO_H

#include "MedP.h"
#include <iostream>
#include <vector>
#include <string>

class Repo{
    private:
        std::vector<Category*> content;
    public:
        Repo();
        ~Repo();
        Category* addTheme(std::string text);
        Category* addTheme(std::string title, std::string symbol, std::vector<std::string> measures);
        Category* getTheme(std::string title);
        Media* getMedia(std::string theme, std::string title);
        Media* addMedia(std::string theme, std::string text);
        Media* addMedia(std::string theme, Media* media);
        std::string print();
        void changeMedia(std::string theme, std::string text,int index, int No);
        void status(std::string theme, std::string text, std::string status);
        std::string symbol(std::string symbol);

};

#endif