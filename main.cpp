#include <string>
#include <string_view>
#include <iostream>
#include <vector>

enum class Types {
    PointerArithmetic,
    ValueArithmetic,
    IO,
    Loop,
    Comment,
};

Types getType(const char chr) {
    switch (chr) {
    case '<': case '>': return Types::PointerArithmetic;
    case '+': case '-': return Types::ValueArithmetic;
    case '.': case ',': return Types::IO;
    case '[': case ']': return Types::Loop;
    default: return Types::Comment;
    }
}

struct Token {
    Types type = Types::Comment;
    char data = 0;
};

auto optimizer(std::string_view input) {
    std::vector<Token> output;

    char counter = 0;
    Types preceding = Types::Comment;
    bool hasStarted = false;

    for (const char& code : input) {
        Types current = getType(code);

        if (current != preceding && hasStarted) {
            output.push_back({preceding, counter});
            counter = 0;
        }

        if (current == Types::PointerArithmetic || current == Types::ValueArithmetic) {
            hasStarted = true;
            if (code == '+' || code == '>') ++counter; else --counter;
        }

        preceding = current;
    }

    return output;
}

int main() {
    [[maybe_unused]] auto tokens = optimizer("+-->><++-.");
    for (auto token : tokens) {
        std::cout << (int)token.data << '\n';
    }
}
