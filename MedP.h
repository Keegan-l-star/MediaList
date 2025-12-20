#ifndef MEDP_H
#define MEDP_H

#include <string>
#include <iostream>
#include <vector>

class MedP{
    protected:
        std::string title;
    public:
        MedP(std::string title);
        void add(MedP* Me);
        virtual MedP* getChild(std::string text);
        virtual std::string print()=0;
};

class Media: public MedP{
    private:
        std::vector<std::string> counters;
        std::vector<int> counts;
        enum status{
            Ongoing,
            Interested,
            Finished,
            Dropped,
            Ended
        };
    public:
        Media(std::vector<std::string> counters, std::string title);
        Media(std::string title);
        ~Media();
        std::string print();
        void changeStatus(std::string text);
        std::string view();
        void addCount();
        void addCount(int index, int count);
        void addCounters(std::vector<std::string> text);
        void addCounters();
};

class Category: public MedP{
    private:
        std::vector<MedP*> group;
        std::vector<std::string> measures;
    public:
        Category(std::string title);
        ~Category();
        void add(MedP* Me);
        void add(std::string Me);
        MedP* getChild(std::string text);
        std::string print();
};

#endif