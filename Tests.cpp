#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Includes.cpp"
#include "PureFunctions.cpp"

using namespace std;

// IsIn

TEST_CASE("IsIn"){
    vector<int> Numbers = { 13, 23, 25, 934, 23};
    CHECK(IsIn(Numbers, 13));
    CHECK(IsIn(Numbers, 23));
    CHECK(!IsIn(Numbers, 3));
}

// IsALetter

TEST_CASE("IsALetter"){
    CHECK(IsALetter('a'));
    CHECK(IsALetter('q'));
    CHECK(IsALetter('Z'));
    CHECK(!IsALetter('3'));
    CHECK(!IsALetter('?'));
    CHECK(!IsALetter('.'));
}

// avrgDistance

TEST_CASE("avrgDistance"){
    vector<int> NoNumbers = { };
    vector<int> Numbers = { 13, 23, 25, 93 };
    vector<int> NumbersTwo = { 14, 23, 25, 193, 23 };

    CHECK(avrgDistance(NoNumbers) < avrgDistance(Numbers));
    CHECK(avrgDistance(Numbers) < avrgDistance(NumbersTwo));
    CHECK(avrgDistance(Numbers) == avrgDistance(Numbers));
}

// ThemesToString

// AnalysationToString

// BookAnalysationToString

// SplitStringIntoWords

TEST_CASE("SplitStringIntoWords"){
    string String = "A quick brown fox";
    vector<string> Vector = SplitStringIntoWords(String);

    CHECK(Vector[0] == "A");
    CHECK(Vector[1] == "quick");
    CHECK(Vector[2] == "brown");
    CHECK(Vector[3] == "fox");

    CHECK(Vector[0] != "B");
    CHECK(Vector[1] != "A");
}

// SplitChapterIntoWords

// MapFromSolutions

TEST_CASE("MapFromSolution"){
    vector<string> Vector {
        "war-chapter",
        "peace-chapter",
        "peace",
        "war",
        "nothing",
    };

    vector<bool> Themes = MapFromSolutions(Vector);

    CHECK(Themes.size() == 4);
    CHECK(Themes[0] == true);
    CHECK(Themes[1] == false);
    CHECK(Themes[2] == false);
    CHECK(Themes[3] == true);
}

// MapVector

TEST_CASE("MapVector"){
    Line String { "A", "quick", "quick", "very", "brown", "fox" };
    map<string, int> Vector = MapVector(String);
    CHECK(Vector["quick"]);
    CHECK(Vector["A"]);
    CHECK(!Vector["food"]);
}

// Filter

TEST_CASE("Filter"){
    Line String { "A", "quick", "quick", "very", "brown", "fox" };

    Line Terms1 { "A", "very" };
    map<string, int> Map1 = MapVector(Terms1);

    Line Terms2 { "quick", "brown" };
    map<string, int> Map2 = MapVector(Terms2);

    optional<int> Results = Filter(String[0], Map1, Map2);
    CHECK(Results.has_value());
    CHECK(Results.value() == 0);

    Results = Filter(String[1], Map1, Map2);
    CHECK(Results.has_value());
    CHECK(Results.value() == 1);

    Results = Filter(String[5], Map1, Map2);
    CHECK(!Results.has_value());
}

// FilterChapter

TEST_CASE("FilterChapter"){
    Chapter String { {"A", "quick"}, {"quick", "very", "brown", "fox"} };

    Line Terms1 { "A", "very" };
    map<string, int> Map1 = MapVector(Terms1);

    Line Terms2 { "quick", "brown" };
    map<string, int> Map2 = MapVector(Terms2);

    pair<vector<int>, vector<int>> Results = FilterChapter(String, Map1, Map2);
    CHECK(Results.first.size() == 2);
    CHECK(Results.second.size() == 3);
}

TEST_CASE("FilterEmptyChapter"){
    Chapter String { { } };

    Line Terms1 { "A", "very" };
    map<string, int> Map1 = MapVector(Terms1);

    Line Terms2 { "quick", "brown" };
    map<string, int> Map2 = MapVector(Terms2);

    pair<vector<int>, vector<int>> Results = FilterChapter(String, Map1, Map2);
    CHECK(Results.first.size() == 0);
    CHECK(Results.second.size() == 0);
}

// sortEvaluations

// EvaluateChapter

// EvaluateAllChapters


