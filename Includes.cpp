#ifndef My_Includes
#define My_Includes

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <numeric>
#include <optional>
#include <future>
#include <algorithm>
#include <sstream>
#include <map>
#include <ranges>

using Word = std::string;
using Line = std::vector<Word>;
using Chapter = std::vector<Line>;
using SingleLineChapter = Line;

using Book = std::vector<std::vector<std::string>>;

struct ChapterEvaluation {
    int chapterIndex;
    const std::vector<std::string>& chapter;
    const std::vector<int> peaceTerms;
    const std::vector<int> warTerms;
    const bool isWarChapter;
};

#endif // My_Includes