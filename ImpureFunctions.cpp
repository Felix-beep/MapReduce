#include "Includes.cpp"
#include "PureFunctions.cpp"

using namespace std;


// Debug
auto PrintChapter = [](const Chapter& Chapter, int num) -> void {
    cout << "Chapter " << num << endl;
    for(vector<string> line : Chapter){
        for(string word : line){
            cout << word << " ";
        }
        cout << endl;
    }
};


// write 
auto writeToFile = [](const vector<ChapterEvaluation>& Themes) -> void {
    ofstream outputFile("./TextFolders/Results.txt");
    int counter = 1;
    for_each(Themes.begin(), Themes.end(), [&](ChapterEvaluation Evaluation){
        outputFile << "Chapter " << counter << ": " << ((Evaluation.isWarChapter)? "war-related" : "peace-related") << endl;
        ++counter;
    });
};

auto WriteAnalyzedChapter = [](const ChapterEvaluation EvaluatedChapter, int num, bool printText = false, bool append = false) -> void {
    ofstream outputFile;
    if(append) outputFile.open("./TextFolders/AnalysationResults.txt", std::ios_base::app);
    else outputFile.open("./TextFolders/AnalysationResults.txt");

    outputFile << "Evaluation for Chapter " << EvaluatedChapter.chapterIndex << ":" << endl ;
    int wordcount = 1;

    if(printText){
        auto printWord = [&](Word word){
            if(IsIn(EvaluatedChapter.peaceTerms, wordcount)) outputFile << "(" << word << ") ";
            else if(IsIn(EvaluatedChapter.warTerms, wordcount)) outputFile << "<" << word << "> ";
            else outputFile << word << " ";
            wordcount++;
        };

        auto printLine = [&](Line line){
            outputFile << " - ";
            for_each(line.begin(), line.end(), printWord);
            outputFile << endl;
        };

        outputFile << endl;
        for_each(EvaluatedChapter.chapter.begin(), EvaluatedChapter.chapter.end(), printLine );
        outputFile << endl;
    }

    
    outputFile << "Found " << EvaluatedChapter.peaceTerms.size() << " peace terms";
    outputFile << " and " << EvaluatedChapter.warTerms.size() << " war terms." << endl;
    outputFile << "At ";
    //for_each(EvaluatedChapter.peaceTerms.begin(), EvaluatedChapter.peaceTerms.end(), [&](int pos){ outputFile << pos << " "; });
    // outputFile << " and ";
    //for_each(EvaluatedChapter.warTerms.begin(), EvaluatedChapter.warTerms.end(), [&](int pos){ outputFile << pos << " "; });
    
    outputFile << "It was declared a " << ((EvaluatedChapter.isWarChapter) ? "war-related" : "peace-related") << " Chapter" << endl << endl;
    return;
};

auto WriteAnalyzedBook = [](const vector<ChapterEvaluation>& EvaluatedChapter, bool printText = false) -> void{
    int num = 1;
    for_each(EvaluatedChapter.begin(), EvaluatedChapter.end(), [&](const ChapterEvaluation& Chapter){
        WriteAnalyzedChapter(Chapter, num, printText, (num > 1));
        ++num;
    });
};

// Read
auto ReadFromSolutions = [](const vector<string>& lines) -> vector<bool> {
    vector<bool> ChapterThemes;

    for(string line : lines){
        if(line.find("peace") != string::npos) ChapterThemes.push_back(false);
        if(line.find("war") != string::npos) ChapterThemes.push_back(true);
    }

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