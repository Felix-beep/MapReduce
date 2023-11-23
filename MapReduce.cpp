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

    vector<vector<string>> WordView = ReadBookFromFile(Filepath + "Book.txt");
    if(WordView.empty()) return 1;

    vector<string> SolutionLines = ReadLinesFromFile(Filepath + "Solution.txt");
    vector<bool> ChapterThemesSolution = MapFromSolutions(SolutionLines);

    vector<ChapterEvaluation> ChapterThemes;
    
    ChapterThemes = EvaluateAllChapters(WordView, PeaceMapping, WarMapping);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    PrintText("Program took " + std::to_string(duration.count()) + " seconds", "TimeResult.txt");

    string AnalysationResults = BookAnalysationToString(ChapterThemes, false);
    PrintText(AnalysationResults, "AnalysationResults.txt");

    string ThemeResults = ThemesToString(ChapterThemes);
    PrintText(ThemeResults, "Results.txt");

    int differences = 0;
    for(int i = 0; i <= (int)ChapterThemesSolution.size() && i <= (int)ChapterThemes.size(); i++){
        if(ChapterThemesSolution[i] != ChapterThemes[i].isWarChapter){
            differences++;
        }
    }
    cout << differences << endl;
    
    return 0;
}

