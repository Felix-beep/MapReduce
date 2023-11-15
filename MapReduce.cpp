#include <functional>
#include <list>
#include <random>
#include <ranges>
#include <vector>
#include <iostream>
#include <string>
#include <map>

#include "ImpureFunctions.cpp"

using namespace std;

int main(int argc, char** argv)
{
    vector<string> WarTerms = ReadLinesFromFile("./TextFolders/War.txt");
    map<string, int> WarMapping = MapVector(WarTerms);

    vector<string> PeaceTerms = ReadLinesFromFile("./TextFolders/Peace.txt");
    map<string, int> PeaceMapping = MapVector(PeaceTerms);

    Book WordView = ConvertToBook("./TextFolders/Book.txt");
    if(WordView.empty()) return 1;

    vector<string> SolutionLines = ReadLinesFromFile("./TextFolders/Solution.txt");
    vector<bool> ChapterThemesSolution = ReadFromSolutions(SolutionLines);


    
    vector<ChapterEvaluation> ChapterThemes;

    /*int ChapterNumber = 9;
    auto AnalyzedChapter = EvaluateChapter(WordView[ChapterNumber], PeaceMapping, WarMapping);
    WriteAnalyzedChapter(AnalyzedChapter, ChapterNumber, true);*/
    
    
    ChapterThemes = EvaluateAllChapters(WordView, PeaceMapping, WarMapping);
    WriteAnalyzedBook(ChapterThemes, false);
    writeToFile(ChapterThemes);
    
    return 0;
}

