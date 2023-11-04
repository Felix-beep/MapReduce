#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <numeric>
#include <optional>
#include <future>

using namespace std;

using Word = string;
using Line = vector<Word>;
using Chapter = vector<Line>;
using SingleLineChapter = Line;
using Book = vector<Chapter>;

struct ChapterEvaluation {
    const Chapter& chapter;
    const vector<int> peaceTerms;
    const vector<int> warTerms;
    const bool isWarChapter;
};

auto writeToFile = [](const vector<ChapterEvaluation>& Themes) -> void {
    ofstream outputFile("./TextFolders/Results.txt");
    int counter = 1;
    for_each(Themes.begin(), Themes.end(), [&](ChapterEvaluation Evaluation){
        outputFile << "Chapter " << counter << ": " << ((Evaluation.isWarChapter)? "war-related" : "peace-related") << endl;
        ++counter;
    });
};

auto PrintChapter = [](const Chapter& Chapter, int num) -> void {
    cout << "Chapter " << num << endl;
    for(vector<string> line : Chapter){
        for(string word : line){
            cout << word << " ";
        }
        cout << endl;
    }
};

auto isin = [](const vector<int>& numbers, int lookingfor) -> bool {
    return any_of(numbers.begin(), numbers.end(), [&](int element){ return element == lookingfor; });
};

auto WriteAnalyzedChapter = [](const ChapterEvaluation EvaluatedChapter, int num, bool printText = false, bool append = false) -> void {
    ofstream outputFile;
    if(append) outputFile.open("./TextFolders/AnalysationResults.txt", std::ios_base::app);
    else outputFile.open("./TextFolders/AnalysationResults.txt");

    outputFile << "Evaluation for Chapter " << num << ":" << endl ;
    int wordcount = 1;

    if(printText){
        auto printWord = [&](Word word){
            if(isin(EvaluatedChapter.peaceTerms, wordcount)) outputFile << "(" << word << ") ";
            else if(isin(EvaluatedChapter.warTerms, wordcount)) outputFile << "<" << word << "> ";
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
    outputFile << " and " << EvaluatedChapter.warTerms.size() << " war terms." << endl;;
    outputFile << "It was declared a " << ((EvaluatedChapter.isWarChapter) ? "war-related" : "peace-related") << " Chapter" << endl << endl;
    return;
};

auto WriteAnalyzedBook = [](const vector<ChapterEvaluation>& EvaluatedChapter, bool printText = false) -> void{
    int num = 1;
    for_each(EvaluatedChapter.begin(), EvaluatedChapter.end(), [&](const ChapterEvaluation& Chapter){
        WriteAnalyzedChapter(Chapter, num, printText, (num > 1));
        ++num;
        cout << num << endl;
    });
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

auto ReadFromSolutions = [](const vector<string>& lines) -> vector<bool> {
    vector<bool> ChapterThemes;

    for(string line : lines){
        if(line.find("peace") != string::npos) ChapterThemes.push_back(false);
        if(line.find("war") != string::npos) ChapterThemes.push_back(true);
    }

    return ChapterThemes;
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

auto IsALetter = [](const char letter) -> bool {
    return (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z');
};

auto SplitIntoWords = [](string WordLine) -> Line {
    Line Words;
    Words.push_back(string());

    // TO DO: change this into a transform
    int currentWord = 0;
    for(char letter : WordLine){
        if(letter == '\'' || IsALetter(letter)){
            Words[currentWord].push_back(letter);
        }
        else if(letter == ' '){
            currentWord++;
            Words.push_back(string());
        }
    }

    return Words;
};

auto SplitChaptersIntoWords = [](vector<vector<string>> InputBook) -> Book {
    Book Book;

    for_each(InputBook.begin(), InputBook.end(), [&](vector<string> InputChapter){
        Chapter Chapter;
        for_each(InputChapter.begin(), InputChapter.end(), [&](string Line){
            Chapter.push_back(SplitIntoWords(Line));
        });
        Book.push_back(Chapter);
    });

    return Book;
};

auto MapVector = [](const vector<string> Vector) -> map<string, int> {
    map<string, int> Mapping;
    for(string Word : Vector){
        Mapping[Word]++;
    }
    return Mapping;
};

auto Filter = [](const string& Word, const map<string, int>& PeaceTerms, const map<string, int>& WarTerms) -> optional<int> {
    if(PeaceTerms.find(Word) != PeaceTerms.end()) return 0;
    if(WarTerms.find(Word) != WarTerms.end()) return 1;
    return nullopt;
};

auto FilterChapter = [](const Chapter& Chapter, const map<string, int>& PeaceTerms, const map<string, int>& WarTerms ) -> pair<vector<int>, vector<int>> {
    vector<int> PeaceDistances = { };
    vector<int> WarDistances = { };
    int iterator = 0;

    auto evaluateWord = [&](Word word){
        optional<int> Result = Filter(word, PeaceTerms, WarTerms);
        if(Result.has_value()){
            if(*Result){
                WarDistances.emplace_back(iterator);
            } else {
                PeaceDistances.emplace_back(iterator);
            }
        }
    };

    auto evaluateLine = [&](Line line){
        for_each(line.begin(), line.end(), evaluateWord);
    };

    for_each(Chapter.begin(), Chapter.end(), evaluateLine);

    return make_pair(PeaceDistances, WarDistances);
};

auto avrgDistance(const vector<int>& Vector) -> float {
    /*int sum = 0;
    int last = *Vector.begin();

    for_each(Vector.begin(), Vector.end(), [&](int num){
        sum += num - last;
        last = num;
    });
    return int(sum / Vector.size());*/
    return (Vector.size() == 0) ? 0 : 1 / Vector.size();
}

auto EvaluateChapter = [](const Chapter& Chapter, const map<string, int> PeaceMapping, const map<string, int> WarMapping) -> ChapterEvaluation {
    
    auto PairVectors = FilterChapter(Chapter, PeaceMapping, WarMapping);

    int PeaceDistance = avrgDistance(PairVectors.first);
    int WarDistance = avrgDistance(PairVectors.second);
    bool isWarChapter = WarDistance < PeaceDistance;

    ChapterEvaluation Result{
        Chapter,
        PairVectors.first,
        PairVectors.second,
        isWarChapter,
    };

    return Result;
};

auto EvaluateAllChapters = [](const Book& Book, const map<string, int>& PeaceMapping, const map<string, int>& WarMapping ) -> vector<ChapterEvaluation> {
    bool skipFirst = true;
    vector<ChapterEvaluation> EvaluatedChapters = {};

    for(Chapter Chapter : Book){
        // To Do: create a view of the Book, and then just drop(1)
        if(skipFirst){
            skipFirst = false;
            continue;
        }

        auto result = EvaluateChapter(Chapter, PeaceMapping, WarMapping);
        EvaluatedChapters.emplace_back(result);
    }

    return EvaluatedChapters;
};

auto ConvertToBook = [](string filename) -> Book {
    vector<vector<string>> Chapters = ReadTextIntoChapters(filename);
    return SplitChaptersIntoWords(Chapters);
};