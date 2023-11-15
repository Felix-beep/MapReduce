#include "Includes.cpp"
#include "ImpureFunctions.cpp"

using namespace std;

int main(int argc, char** argv)
{
    auto start = std::chrono::high_resolution_clock::now();

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

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    PrintText("Program took " + std::to_string(duration.count()) + " seconds", "TimeResult.txt");

    string AnalysationResults = BookAnalysationToString(ChapterThemes, false);
    PrintText(AnalysationResults, "AnalysationResults.txt");

    string ThemeResults = ThemesToString(ChapterThemes);
    PrintText(ThemeResults, "Results.txt");
    
    return 0;
}

