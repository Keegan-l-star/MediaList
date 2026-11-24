#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

bool isTerminal() {
#ifdef _WIN32
    DWORD mode;
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    return GetConsoleMode(output, &mode);
#else
    return isatty(STDOUT_FILENO);
#endif
}

class userInterface{
    protected:
        Display* display;
    public:
        userInterface::userInterface(){
            if(isTerminal()){

            }else{

            }
        }
};


int main(){
    userInterface interact;

    return 0;
}