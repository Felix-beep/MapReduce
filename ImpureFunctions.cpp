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
auto ReadFromSolutions = [](const vector<string>& lines) -> vector<bool> {
    vector<bool> ChapterThemes;

    for_each(lines.begin(), lines.end(), [&](string line){
        if(line.find("peace") != string::npos) ChapterThemes.push_back(false);
        if(line.find("war") != string::npos) ChapterThemes.push_back(true);
    });

    return ChapterThemes;
};

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


auto ReadTextIntoChapters = [](const string& filename) -> vector<vector<string>> {
    ifstream inputFile(filename);
    vector<vector<string>> chapters;
    chapters.push_back(vector<string>());    

    if(!inputFile.is_open()){
        return chapters;
    }

    string line;
    int currentchapter = 0;
    while(std::getline(inputFile, line)){
        if(line.length() != 0){
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

auto ConvertToBook = [](string filename) -> Book {
    vector<vector<string>> Chapters = ReadTextIntoChapters(filename);
    return SplitChaptersIntoWords(Chapters);
};