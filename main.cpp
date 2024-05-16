#include <string_view>
#include <iostream>
#include <vector>
#include <array>
#include <utility>
#include <optional>

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
    // make a smart union for less memory usage depending on the use case
    // std::variant<Types, std::tuple<char, bool>> container;
    Types type = Types::Comment;
    char data = 0;
    bool isSecondOperation = false;
};

template <typename NumericType>
NumericType nrOfDigits(NumericType number) {
    NumericType counter{0};
    while (number) {
        ++counter;
        number /= 10;
    } return counter;
}

auto optimizer(std::string_view input) {
    std::vector<Token> output;
    unsigned long long size = 0;
    
    unsigned char indentation = 0;

    // Trying to pack these properly but compiler will most likely optimize it anyway
    char valueCounter = 0;
    char braceCounter = 0;
    Types preceding = Types::Comment;
    bool hasStarted = false;

    for (const char& code : input) {
        Types current = getType(code);

        bool ommit = false;

        // Brace checking
        if (current == Types::Loop) {
            if (code == '[') ++braceCounter; else --braceCounter;

            // FIXME: add smart checking
            // if (current == preceding) {
            //     if (code == ']') {
            //         ommit = true;
            //         // output.pop_back();
            //     }
            // }
        }

        // Code for arithmetic
        if (current != preceding && hasStarted && valueCounter != 0) {
            if (current == Types::PointerArithmetic)
                size += 7;
            if (current == Types::ValueArithmetic)
                size += 8;
            size += nrOfDigits(valueCounter);

            output.push_back({preceding, valueCounter});
            valueCounter = 0;
        }

        if (current == Types::PointerArithmetic || current == Types::ValueArithmetic) {
            hasStarted = true;
            if (code == '+' || code == '>') ++valueCounter; else --valueCounter;
        } else {
            bool isSecondOperation = false;
            if (code == '.' || code == ']') isSecondOperation = true;

            switch (code) {
                case ',': size += 16 + indentation; break;
                case '.': size += 13 + indentation; break;
                case '[': indentation += 2; size += 13 + indentation; break;
                case ']': indentation -= 2; size += 14 + indentation; break;
            }
            // 16 13 13 14
            output.push_back({current, 0, isSecondOperation});
        }

        preceding = current;
    }

    if (braceCounter > 0) throw std::logic_error("Too many OPEN braces");
    if (braceCounter < 0) throw std::logic_error("Too many CLOSE braces");

    return std::make_tuple(output, size);
}

std::string translator(const auto& input) {
    const auto& [tokens, size] = input;
    std::string output;
    output.reserve(size);

    unsigned char indentationLevel = 0;

    for (const auto& token : tokens) {
        output += std::string(indentationLevel * 2, ' ');

        if (token.type == Types::PointerArithmetic || token.type == Types::ValueArithmetic) {
            std::string sign;
            if (token.type == Types::PointerArithmetic) output += "*";
            if (token.data < 0) sign = "-"; else sign = "+"; // weird ahh fix
            output += "p " + sign + "= " + std::to_string(abs(token.data)) + ";";
        } else if (token.type == Types::IO) {
            if (!token.isSecondOperation)
                output += "*p = getchar();";
            else
                output += "putchar(*p);";
        } else if (token.type == Types::Loop) {
            if (!token.isSecondOperation) {
                output += "if (*p) do {";
                ++indentationLevel;
            }
            else {
                output.pop_back(); output.pop_back();
                output += "} while (*p);";
                --indentationLevel;
            }
        } else throw std::logic_error("Something went wrong at translation");
        output += '\n';
    }

    return output;
}

int main() {
    std::string_view input{"+>[+++],."};

    auto tokens = optimizer(input);
    auto output = translator(tokens);

    std::cout << output;
}
