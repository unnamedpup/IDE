#include <catch2/catch_all.hpp>
#include <src/json.hpp>
#include <src/runner.hpp>
#include <src/output.hpp>

TEST_CASE("LOCAL TEST: Adding numbers", "[adding_numbers]") {
    Output *output = new Output(1, 1, 0, 1, L"../../tests/adding_test.json");
    output->setContents(L"");
    run(output);
    int expectedResult = 12488;
    REQUIRE(std::stoi(output->getContents()) == expectedResult);
}

TEST_CASE("LOCAL TEST: Subtracting numbers", "[subtracting_numbers]") {
    Output *output = new Output(1, 1, 0, 1, L"../../tests/subtracting_test.json");
    output->setContents(L"");
    run(output);
    int expectedResult = -9972;
    REQUIRE(std::stoi(output->getContents()) == expectedResult);
}

TEST_CASE("LOCAL TEST: Multiplication numbers", "[multiplication_numbers]") {
    Output *output = new Output(1, 1, 0, 1, L"../../tests/multiplication_test.json");
    output->setContents(L"");
    run(output);
    int expectedResult = 28934;
    REQUIRE(std::stoi(output->getContents()) == expectedResult);
}


TEST_CASE("LOCAL TEST: Division numbers", "[division_numbers]") {
    Output *output = new Output(1, 1, 0, 1, L"../../tests/division_test.json");
    output->setContents(L"");
    run(output);
    int expectedResult = 16;
    REQUIRE(std::stoi(output->getContents()) == expectedResult);
}


TEST_CASE("LOCAL TEST: Null division numbers", "[null_division_numbers]") {
    Output *output = new Output(1, 1, 0, 1, L"../../tests/null_division_test.json");
    output->setContents(L"");
    run(output);
    std::wstring expectedResult = L"Деление на ноль";
    REQUIRE(output->getContents() == expectedResult);
}