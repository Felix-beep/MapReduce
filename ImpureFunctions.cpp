#include "Includes.cpp"
#include "PureFunctions.cpp"

using namespace std;

// Debug
auto PrintChapter = [](const Chapter& Chapter, int num) -> void {
    cout << "Chapter " << num << endl;
    for_each(Chapter.begin(), Chapter.end(), [&](Line line){
        for_each(line.begin(), line.end(), [&](string word){
            cout << word << " ";
        });
        cout << endl;
    });
};

// Print

auto PrintText = [](const string Text, string Filename) -> void {
    string filepath = "./ResultFolder/" + Filename;
    ofstream outputFile(filepath);
    outputFile << Text;
};

// Read

auto ReadLinesFromFile = [](const string& filename) -> vector<string> {
    ifstream inputFile(filename);
    vector<string> lines;

    if(!inputFile.is_open()){
        return lines;
    }

    string line;
    while(std::getline(inputFile, line)){
        lines.push_back(line);
    }

    inputFile.close();

    return lines;
};


auto ReadBookFromFile = [](const string& filename) -> vector<vector<string>> {
    ifstream inputFile(filename);
    vector<vector<string>> chapters;
    chapters.push_back(vector<string>());    

    if(!inputFile.is_open()){
        return chapters;
    }

    string line;
    int currentchapter = 0;
    bool bookIsOver = false;
    bool bookHasStarted = false;
    while(std::getline(inputFile, line) && !bookIsOver){
        if(line.length() == 0){
            continue;
        }

        if( !bookHasStarted && line.compare(0, 25, "*** START OF THE PROJECT ") == 0){
            cout << line << endl;
            cout << "book has started" << endl;
            bookHasStarted = true;
            continue;
        }

        if ( bookHasStarted && line.compare(0, 23, "*** END OF THE PROJECT") == 0){
            bookIsOver = true;
            continue;
        } 
        
        if( bookHasStarted ){
            if(line.compare(0, 7, "CHAPTER") == 0){
                currentchapter++;
                chapters.push_back(vector<string>());
            } else {
                chapters[currentchapter].push_back(line);
            } 
        }
        
    }

    inputFile.close();

    return chapters;
};