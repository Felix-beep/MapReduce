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
    string Filepath = "./TextFolder/";
    vector<string> WarTerms = ReadLinesFromFile(Filepath + "War.txt");
    map<string, int> WarMapping = MapVector(WarTerms);

    vector<string> PeaceTerms = ReadLinesFromFile(Filepath + "Peace.txt");
    map<string, int> PeaceMapping = MapVector(PeaceTerms);

    Book WordView = ConvertToBook(Filepath + "Book.txt");
    if(WordView.empty()) return 1;

    vector<string> SolutionLines = ReadLinesFromFile(Filepath + "Solution.txt");
    vector<bool> ChapterThemesSolution = ReadFromSolutions(SolutionLines);


    
    vector<ChapterEvaluation> ChapterThemes;

    /*int ChapterNumber = 9;
    auto AnalyzedChapter = EvaluateChapter(WordView[ChapterNumber], PeaceMapping, WarMapping);
    WriteAnalyzedChapter(AnalyzedChapter, ChapterNumber, true);*/
    
    
    ChapterThemes = EvaluateAllChapters(WordView, PeaceMapping, WarMapping);

    string AnalysationResults = BookAnalysationToString(ChapterThemes, false);
    PrintText(AnalysationResults, "AnalysationResults.txt");

    string ThemeResults = ThemesToString(ChapterThemes);
    PrintText(ThemeResults, "Results.txt");
    
    return 0;
}

