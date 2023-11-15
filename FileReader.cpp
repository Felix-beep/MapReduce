#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <numeric>
#include <optional>
#include <future>
#include <algorithm>

using namespace std;

using Word = string;
using Line = vector<Word>;
using Chapter = vector<Line>;
using SingleLineChapter = Line;
using Book = vector<Chapter>;

struct ChapterEvaluation {
    int chapterIndex;
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

    outputFile << "Evaluation for Chapter " << EvaluatedChapter.chapterIndex << ":" << endl ;
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
    outputFile << " and " << EvaluatedChapter.warTerms.size() << " war terms." << endl;
    outputFile << "At ";
    for_each(EvaluatedChapter.peaceTerms.begin(), EvaluatedChapter.peaceTerms.end(), [&](int pos){ outputFile << pos << " "; });
    for_each(EvaluatedChapter.warTerms.begin(), EvaluatedChapter.warTerms.end(), [&](int pos){ outputFile << pos << " "; });
    
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

    string currentWord = string();

    for_each(WordLine.begin(), WordLine.end(), [&](char letter){
        if(letter == '\'' || IsALetter(letter)){
            currentWord.push_back(letter);
        }
        else if(letter == ' '){
            Words.push_back(currentWord);
            currentWord = string();
        }
    });

    Words.push_back(currentWord);

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
                WarDistances.push_back(iterator);
            } else {
                PeaceDistances.push_back(iterator);
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
    return (Vector.size() == 0) ? 0 : 1 / Vector.size();
}

auto EvaluateChapter = [](const Chapter& Chapter, const map<string, int> PeaceMapping, const map<string, int> WarMapping) -> ChapterEvaluation* {
    
    auto PairVectors = FilterChapter(Chapter, PeaceMapping, WarMapping);

    

    int PeaceDistance = avrgDistance(PairVectors.first);
    int WarDistance = avrgDistance(PairVectors.second);
    bool isWarChapter = WarDistance < PeaceDistance;

    ChapterEvaluation* Result = new ChapterEvaluation{
        0,
        Chapter,
        PairVectors.first,
        PairVectors.second,
        isWarChapter,
    };

    return Result;
};

auto sortEvaluations = [](const vector<ChapterEvaluation>& Vector){

    vector<ChapterEvaluation> newVector;
    vector<pair<int, int>> ChapterIndexPair;

    int iter = 0;
    for_each(Vector.begin(), Vector.end(), [&](ChapterEvaluation Eval){ 
        ChapterIndexPair.push_back(make_pair(Eval.chapterIndex, iter++));
    });

    std::sort(ChapterIndexPair.begin(), ChapterIndexPair.end(), [](pair<int, int> PairOne, pair<int, int> PairTwo){
        return PairOne.first < PairTwo.first;
    });

    transform(ChapterIndexPair.begin(), ChapterIndexPair.end(), back_inserter(newVector), [=](const pair<int, int> element){
        return Vector[element.second]; 
    });

    return newVector;
};

auto EvaluateAllChapters = [](const Book& Book, const map<string, int>& PeaceMapping, const map<string, int>& WarMapping ) -> vector<ChapterEvaluation> {
    mutex mtx;
    vector<ChapterEvaluation> EvaluatedChapters;
    auto BookView = Book | std::views::all | std::views::drop(1);
    int i = 0;

    // Create a vector of threads
    vector<thread> activethreads = {};

    ranges::for_each(BookView.begin(), BookView.end(), [&](const Chapter& Chapter){
        int Threadnumber = ++i;
        activethreads.emplace_back([&EvaluatedChapters, &mtx, &Chapter, &PeaceMapping, &WarMapping, Threadnumber]() {
            //cout << "Chapter Size at the beginning: " << Chapter.size() << "/" << Book[Threadnumber].size() << endl;
            ChapterEvaluation* result = EvaluateChapter(Chapter, PeaceMapping, WarMapping);
            result->chapterIndex = Threadnumber;
            lock_guard<mutex> lock(mtx);
            for_each(result->peaceTerms.begin(), result->peaceTerms.end(), [&](int pos){ cout << pos << endl; });
            //cout << Threadnumber << endl;
            EvaluatedChapters.emplace_back(*result);
        });
    });

    for (auto& thread : activethreads) {
        thread.join();
    }

    vector<ChapterEvaluation> EvaluatedChaptersSorted = sortEvaluations(EvaluatedChapters);

    return EvaluatedChaptersSorted;
};

auto ConvertToBook = [](string filename) -> Book {
    vector<vector<string>> Chapters = ReadTextIntoChapters(filename);
    return SplitChaptersIntoWords(Chapters);
};