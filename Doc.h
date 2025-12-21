#ifndef DOC_H
#define DOC_H

#include <string>
#include "MedP.h"
#include "Repo.h"
#include <vector>
#include <fstream>

using namespace std;

class Doc{
    private:
        Repo* list;
    public:
        Doc();
        ~Doc();
        void save();
        void load();
        Repo* getRepo();
};

#endif