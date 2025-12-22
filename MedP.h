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
        virtual void add(MedP* Me);
        virtual MedP* getChild(std::string text);
        virtual std::string print()=0;
        virtual std::string view()=0;
        virtual MedP* cat(std::string text);
};

class Media: public MedP{
    private:
        std::vector<std::string> counters;
        std::vector<int> counts;
        enum class Status{
            Ongoing,
            Interested,
            Finished,
            Dropped,
            Ended
        };
        Status status;
        std::string statusText() const;
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
        std::string getTitle(){
            return title;
        }
};

class Category: public MedP{
    private:
        std::vector<MedP*> group;
        std::vector<std::string> measures;
        std::string symbol;
    public:
        Category(std::string title, std::string symbol, std::vector<std::string> measures);
        ~Category();
        void add(MedP* Me);
        void add(std::string Me);
        MedP* getChild(std::string text);
        std::string print();
        std::string view();
        Category* cat(std::string text);
        std::vector<std::string> getMeasures(){
            return measures;
        };
        std::string getSymbol(){
            return symbol;  
        }
        std::string getTitle(){
            return title;
        }
};

#endif