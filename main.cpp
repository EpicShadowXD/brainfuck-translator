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
    case '-': case '+': return Types::ValueArithmetic;
    case ',': case '.': return Types::IO;
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

    // Trying to pack these properly but compiler will most likely optimize it anyway
    char valueCounter = 0;
    char braceCounter = 0;
    Types preceding = Types::Comment;
    bool hasStarted = false;

    for (const char& code : input) {
        Types current = getType(code);

        // Brace checking
        if (current == Types::Loop) {
            if (code == '[') ++braceCounter; else --braceCounter;
        }

        // Code for arithmetic
        if (current != preceding && hasStarted && valueCounter != 0) {
            output.push_back({preceding, valueCounter});
            valueCounter = 0;
        }

        if (current == Types::PointerArithmetic || current == Types::ValueArithmetic) {
            hasStarted = true;
            if (code == '+' || code == '>') ++valueCounter; else --valueCounter;
        }

        preceding = current;
    }

    if (braceCounter > 0) throw std::logic_error("Too many OPEN braces");
    if (braceCounter < 0) throw std::logic_error("Too many CLOSE braces");

    return output;
}

int main() {
    std::string_view input{"[++]--<>>+++."};

    auto tokens = optimizer(input);

    for (auto token : tokens)
        std::cout << (int)token.data << '\n';
}
