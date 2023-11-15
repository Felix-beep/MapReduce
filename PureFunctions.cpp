#include "Includes.cpp"

using namespace std;

auto IsIn = [](const vector<int>& numbers, int lookingfor) -> bool {
    return any_of(numbers.begin(), numbers.end(), [&](int element){ return element == lookingfor; });
};

auto IsALetter = [](const char letter) -> bool {
    return (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z');
};

auto SplitStringIntoWords = [](string WordLine) -> Line {
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
            Chapter.push_back(SplitStringIntoWords(Line));
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
    int iterator = 1;

    auto evaluateWord = [&](Word word){
        optional<int> Result = Filter(word, PeaceTerms, WarTerms);
        if(Result.has_value()){
            if(*Result){
                WarDistances.push_back(iterator);
            } else {
                PeaceDistances.push_back(iterator);
            }
        }
        iterator++;
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

auto EvaluateChapter = [](const Chapter& Chapter, const map<string, int> PeaceMapping, const map<string, int> WarMapping) -> ChapterEvaluation {
    
    auto PairVectors = FilterChapter(Chapter, PeaceMapping, WarMapping);

    int PeaceDistance = avrgDistance(PairVectors.first);
    int WarDistance = avrgDistance(PairVectors.second);
    bool isWarChapter = WarDistance < PeaceDistance;

    ChapterEvaluation Result{
        0,
        Chapter,
        PairVectors.first,
        PairVectors.second,
        isWarChapter,
    };
    return Result;
};

auto EvaluateAllChapters = [](const Book& Book, const map<string, int>& PeaceMapping, const map<string, int>& WarMapping ) -> vector<ChapterEvaluation> {
    mutex mtx;
    vector<ChapterEvaluation> EvaluatedChapters;
    auto BookView = Book | std::views::all | std::views::drop(1);
    int i = 0;

    vector<thread> activethreads = {};

    ranges::for_each(BookView.begin(), BookView.end(), [&](const Chapter& Chapter){
        int Threadnumber = ++i;
        activethreads.emplace_back([&EvaluatedChapters, &mtx, &Chapter, &PeaceMapping, &WarMapping, Threadnumber]() {
            ChapterEvaluation result = EvaluateChapter(Chapter, PeaceMapping, WarMapping);
            result.chapterIndex = Threadnumber;
            lock_guard<mutex> lock(mtx);
            EvaluatedChapters.emplace_back(result);
        });
    });

    for (auto& thread : activethreads) {
        thread.join();
    }

    vector<ChapterEvaluation> EvaluatedChaptersSorted = sortEvaluations(EvaluatedChapters);

    return EvaluatedChaptersSorted;
};
