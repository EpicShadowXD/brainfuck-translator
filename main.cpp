#include <unordered_map>
#include <string>
#include <vector>
#include <utility>

enum class Token {
    MinusPointer,
    PlusPointer,
    MinusValue,
    PlusValue,
    BeginLoop,
    EndLoop,
    Input,
    Output,
    Uninitialized
};

const std::unordered_map<char, Token> types {
    {'<', Token::MinusPointer},
    {'>', Token::PlusPointer},
    {'-', Token::MinusValue},
    {'+', Token::PlusValue},
    {'[', Token::BeginLoop},
    {']', Token::EndLoop},
    {',', Token::Input},
    {'.', Token::Output}
};

auto tokenizer(std::string input) {
    std::vector<Token> output;
    output.reserve(input.size());

    for (const char& chr : input)
        output.push_back(types.at(chr));

    return output;
}

const std::unordered_map<Token, std::string> c_conversions{
    {Token::MinusPointer, "p -= "},
    {Token::PlusPointer, "p += "},
    {Token::MinusValue, "\\*p -= "},
    {Token::PlusValue, "\\*p += "},
    {Token::BeginLoop, "if (\\*p) do {"},
    {Token::EndLoop, "} while (\\*p);"},
    {Token::Input, "\\*p = getchar();"},
    {Token::Output, "putchar(\\*p);"}
};

bool isValueTypeInstruction(Token token) {
    switch(token) {
        case Token::MinusPointer:
        case Token::PlusPointer:
        case Token::MinusValue:
        case Token::PlusValue:
            return true;
    } return false;
}

std::string translator(const auto& tokens) {
    std::string output;

    int counter = 0;
    bool isPointer = false;
    bool isValue = false;
    Token lastToken = Token::Uninitialized;

    for (const auto& token : tokens) {
        isPointer = false; isValue = false;
        if (token == Token::MinusPointer || token == Token::PlusPointer)
            isPointer = true;
        if (token == Token::MinusValue || token == Token::PlusValue)
            isValue = true;
        
    }

    return output;
}

int main() {

}
