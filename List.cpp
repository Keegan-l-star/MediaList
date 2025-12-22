#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "Doc.h"
#include "Repo.h"
#include "MedP.h"

using namespace std;

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void printHeader() {
    cout << "========================================" << endl;
    cout << "          MEDIA LIST MANAGER           " << endl;
    cout << "========================================" << endl;
}

void printMenu() {
    cout << "\n--- Main Menu ---" << endl;
    cout << "1. View all media" << endl;
    cout << "2. Add a new category/theme" << endl;
    cout << "3. Add new media to a category" << endl;
    cout << "4. Update media progress" << endl;
    cout << "5. Change media status" << endl;
    cout << "6. Save and Exit" << endl;
    cout << "\nEnter your choice: ";
}

void waitForEnter() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void viewAllMedia(Doc& doc) {
    clearScreen();
    printHeader();
    cout << "\n--- All Media ---\n" << endl;
    string content = doc.getRepo()->print();
    if (content.empty()) {
        cout << "No media found. Add some categories and media first!" << endl;
    } else {
        cout << content << endl;
    }
    waitForEnter();
}

void addCategory(Doc& doc) {
    clearScreen();
    printHeader();
    cout << "\n--- Add New Category ---\n" << endl;

    string title, symbol;
    vector<string> measures;
    string measure;

    cout << "Enter category name (e.g., Anime, Movie, Manga): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, title);

    cout << "Enter symbol for this category (single character, e.g., @, #, +, *): ";
    getline(cin, symbol);

    cout << "Enter measures/counters for this category." << endl;
    cout << "(e.g., Episodes, Seasons, Chapters, Pages)" << endl;
    cout << "Enter each measure and press Enter. Type 'done' when finished:" << endl;

    while (true) {
        cout << "  Measure " << measures.size() + 1 << ": ";
        getline(cin, measure);
        if (measure == "done" || measure == "Done" || measure == "DONE") {
            break;
        }
        if (!measure.empty()) {
            measures.push_back(measure);
        }
    }

    if (measures.empty()) {
        cout << "\nError: You must add at least one measure!" << endl;
        waitForEnter();
        return;
    }

    Category* cat = doc.getRepo()->addTheme(title, symbol, measures);
    if (cat != nullptr) {
        cout << "\nCategory '" << title << "' added successfully!" << endl;
    } else {
        cout << "\nFailed to add category. Title or symbol may already exist." << endl;
    }
    waitForEnter();
}

void addMedia(Doc& doc) {
    clearScreen();
    printHeader();
    cout << "\n--- Add New Media ---\n" << endl;

    string theme, title, status;

    cout << "Enter the category/theme name (e.g., Anime, Movie): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, theme);

    Category* cat = doc.getRepo()->getTheme(theme);
    if (cat == nullptr) {
        cout << "\nCategory '" << theme << "' not found!" << endl;
        cout << "Please add the category first." << endl;
        waitForEnter();
        return;
    }

    cout << "Enter media title: ";
    getline(cin, title);

    cout << "\nAvailable statuses:" << endl;
    cout << "  - Interested" << endl;
    cout << "  - Ongoing" << endl;
    cout << "  - Finished" << endl;
    cout << "  - Dropped" << endl;
    cout << "  - Ended" << endl;
    cout << "Enter status: ";
    getline(cin, status);

    // Get initial counts for each measure
    vector<string> measures = cat->getMeasures();
    string mediaStr = "(" + title + "){";
    
    for (size_t i = 0; i < measures.size(); i++) {
        int count;
        cout << "Enter initial " << measures[i] << " count: ";
        cin >> count;
        mediaStr += to_string(count);
        if (i < measures.size() - 1) {
            mediaStr += ",";
        }
    }
    mediaStr += "}\\\\" + status;

    Media* media = doc.getRepo()->addMedia(theme, mediaStr);
    if (media != nullptr) {
        cout << "\nMedia '" << title << "' added successfully!" << endl;
    } else {
        cout << "\nFailed to add media. It may already exist." << endl;
    }
    waitForEnter();
}

void updateProgress(Doc& doc) {
    clearScreen();
    printHeader();
    cout << "\n--- Update Media Progress ---\n" << endl;

    string theme, title;

    cout << "Enter the category/theme name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, theme);

    Category* cat = doc.getRepo()->getTheme(theme);
    if (cat == nullptr) {
        cout << "\nCategory '" << theme << "' not found!" << endl;
        waitForEnter();
        return;
    }

    cout << "Enter media title: ";
    getline(cin, title);

    Media* media = doc.getRepo()->getMedia(theme, title);
    if (media == nullptr) {
        cout << "\nMedia '" << title << "' not found in '" << theme << "'!" << endl;
        waitForEnter();
        return;
    }

    cout << "\nCurrent status: " << media->print() << endl;

    vector<string> measures = cat->getMeasures();
    cout << "\nWhich counter do you want to update?" << endl;
    for (size_t i = 0; i < measures.size(); i++) {
        cout << "  " << i + 1 << ". " << measures[i] << endl;
    }
    cout << "Enter choice (1-" << measures.size() << "): ";
    
    int choice;
    cin >> choice;

    if (choice < 1 || choice > (int)measures.size()) {
        cout << "\nInvalid choice!" << endl;
        waitForEnter();
        return;
    }

    cout << "Enter amount to add: ";
    int amount;
    cin >> amount;

    doc.getRepo()->changeMedia(theme, title, choice - 1, amount);
    cout << "\nProgress updated!" << endl;
    cout << "New status: " << media->print() << endl;
    waitForEnter();
}

void changeStatus(Doc& doc) {
    clearScreen();
    printHeader();
    cout << "\n--- Change Media Status ---\n" << endl;

    string theme, title, status;

    cout << "Enter the category/theme name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, theme);

    cout << "Enter media title: ";
    getline(cin, title);

    Media* media = doc.getRepo()->getMedia(theme, title);
    if (media == nullptr) {
        cout << "\nMedia not found!" << endl;
        waitForEnter();
        return;
    }

    cout << "\nCurrent: " << media->print() << endl;

    cout << "\nAvailable statuses:" << endl;
    cout << "  - Interested" << endl;
    cout << "  - Ongoing" << endl;
    cout << "  - Finished" << endl;
    cout << "  - Dropped" << endl;
    cout << "  - Ended" << endl;
    cout << "Enter new status: ";
    getline(cin, status);

    doc.getRepo()->status(theme, title, status);
    cout << "\nStatus updated!" << endl;
    cout << "New: " << media->print() << endl;
    waitForEnter();
}

int main() {
    Doc document;
    int choice;
    bool running = true;

    while (running) {
        clearScreen();
        printHeader();
        printMenu();

        cin >> choice;

        switch (choice) {
            case 1:
                viewAllMedia(document);
                break;
            case 2:
                addCategory(document);
                break;
            case 3:
                addMedia(document);
                break;
            case 4:
                updateProgress(document);
                break;
            case 5:
                changeStatus(document);
                break;
            case 6:
                cout << "\nSaving and exiting..." << endl;
                running = false;
                break;
            default:
                cout << "\nInvalid choice! Please try again." << endl;
                waitForEnter();
                break;
        }
    }

    cout << "Goodbye!" << endl;
    return 0;
}