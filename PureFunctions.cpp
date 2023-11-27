#include "Includes.cpp"

using namespace std;


auto for_each_collection = [](auto Vector, auto fn){
    return for_each(Vector.begin(), Vector.end(), fn);
};

// boolean lambdas

auto IsIn = [](const vector<int>& numbers, int lookingfor) -> bool {
    return any_of(numbers.begin(), numbers.end(), [&](int element){ return element == lookingfor; });
};

auto IsALetter = [](const char letter) -> bool {
    return (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z');
};

// calculations

auto AvrgDistance = [](const vector<int>& Vector) -> double {

    if(Vector.size() <= 3) return double(1000);

    auto start = Vector.begin() + 1;
    auto end = Vector.end() - 1;

    int sum = 0;
    int last = *start;

    for_each(start, end, [&](int num){
        sum += num - last;
        last = num;
    });
    return sum / (Vector.size() - 3);
};

auto AvrgDistanceBounded = [](const vector<int>& Vector) -> double {

    if(Vector.size() <= 1) return double(1000);

    int sum = 0;
    int last = 0;

    for_each_collection(Vector, [&](int num){
        sum += num - last;
        last = num;
    });

    return sum / (Vector.size() - 1);
};

auto Density = [](const vector<int>& Vector) -> double {
    return Vector.size();
};

// String Creators

auto ThemesToString = [](const vector<ChapterEvaluation>& Themes) -> string {
    stringstream outputString;
    int counter = 1;
    for_each_collection(Themes, [&](ChapterEvaluation Evaluation){
        outputString << "Chapter " << counter << ": " << ((Evaluation.isWarChapter)? "war-related" : "peace-related") << endl;
        ++counter;
    });

    return outputString.str();
};

auto AnalysationToString = [](const ChapterEvaluation EvaluatedChapter, bool printText = false) -> string {
    stringstream outputString;

    outputString << "Evaluation for Chapter " << EvaluatedChapter.chapterIndex << ":" << endl ;

    if(printText){

        auto printLine = [&](string line){
            outputString << line << endl;
        };

        outputString << endl;
        for_each_collection(EvaluatedChapter.chapter, printLine );
        outputString << endl;
    }

    
    outputString << "Found " << EvaluatedChapter.peaceTerms.size() << " peace terms";
    outputString << " and " << EvaluatedChapter.warTerms.size() << " war terms." << endl;
    
    outputString << "It was declared a " << ((EvaluatedChapter.isWarChapter) ? "war-related" : "peace-related") << " Chapter" << endl << endl;
    return outputString.str();
};

auto BookAnalysationToString = [](const vector<ChapterEvaluation>& EvaluatedChapter, bool printText = false) -> string {
    string results;
    for_each_collection(EvaluatedChapter, [&](const ChapterEvaluation& Chapter){
        string result = AnalysationToString(Chapter, printText);
        results.append(result);
    });
    return results;
};

auto DifferencesToString = [](vector<bool>& ChapterThemesSolution, vector<ChapterEvaluation>& ChapterThemes){
    stringstream outputString;
    int differences = 0;
    for(int i = 0; i <= (int)ChapterThemesSolution.size() && i <= (int)ChapterThemes.size(); i++){
        if(ChapterThemesSolution[i] != ChapterThemes[i].isWarChapter){
            differences++;
        }
    }
    outputString << "Number of differences: " << differences << endl;
    outputString << "with an accuracy of: " << ChapterThemesSolution.size() - differences << "/" << ChapterThemesSolution.size() << endl;

    return outputString.str();
};

// String reformers

auto SplitStringIntoWords = [](string WordLine) -> Line {
    Line Words;

    string currentWord = string();

    for_each_collection(WordLine, [&](char letter){
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

// Mapping

auto MapFromSolutions = [](const vector<string>& lines) -> vector<bool> {
    vector<bool> ChapterThemes;

    for_each_collection(lines, [&](string line){
        if(line.find("peace") != string::npos) ChapterThemes.push_back(false);
        if(line.find("war") != string::npos) ChapterThemes.push_back(true);
    });

    return ChapterThemes;
};

auto MapVector = [](const vector<string> Vector) -> map<string, int> {
    map<string, int> Mapping;
    for_each_collection(Vector, [&](string Word){
        Mapping[Word]++;
    });
    return Mapping;
};

// Filtering

auto Filter = [](const string& Word, const map<string, int>& PeaceTerms, const map<string, int>& WarTerms) -> optional<int> {
    if(PeaceTerms.find(Word) != PeaceTerms.end()) return 0;
    if(WarTerms.find(Word) != WarTerms.end()) return 1;
    return nullopt;
};

auto FilterChapter = [](const vector<string>& Chapter, const map<string, int>& PeaceTerms, const map<string, int>& WarTerms ) -> pair<vector<int>, vector<int>> {
    vector<int> FoundPeaceTerms = { 0 };
    vector<int> FoundWarTerms = { 0 };
    int iterator = 1;

    auto evaluateWord = [&](Word word){
        optional<int> Result = Filter(word, PeaceTerms, WarTerms);
        if(Result.has_value()){
            if(*Result){
                FoundWarTerms.push_back(iterator);
            } else {
                FoundPeaceTerms.push_back(iterator);
            }
        }
        iterator++;
    };

    auto evaluateLine = [&](string line){
        Line Words = SplitStringIntoWords(line);
        for_each_collection(Words, evaluateWord);
    };

    for_each_collection(Chapter, evaluateLine);   

    FoundPeaceTerms.push_back(iterator); 
    FoundWarTerms.push_back(iterator);
    
    return make_pair(FoundPeaceTerms, FoundWarTerms);
};

// Sorting

auto sortEvaluations = [](const vector<ChapterEvaluation>& Vector){
    vector<ChapterEvaluation> newVector;
    vector<pair<int, int>> ChapterIndexPair;

    int iter = 0;
    for_each_collection(Vector, [&](ChapterEvaluation Eval){ 
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

// Evaluation

auto EvaluateChapter = [](const vector<string>& Chapter, const map<string, int> PeaceMapping, const map<string, int> WarMapping) -> ChapterEvaluation {
    auto PairVectors = FilterChapter(Chapter, PeaceMapping, WarMapping);

    int PeaceDistance = Density(PairVectors.first);
    int WarDistance = Density(PairVectors.second);
    bool isWarChapter = WarDistance > PeaceDistance;

    ChapterEvaluation Result{
        0,
        Chapter,
        PairVectors.first,
        PairVectors.second,
        isWarChapter,
    };
    return Result;
};
