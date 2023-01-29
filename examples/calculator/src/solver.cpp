#include "solver.hpp"

#include <algorithm>
#include <iostream>
#include <math.h>
#include <utility>

#define VERBOS false

const std::map<std::string, std::string> Solver::brace_closure = {
    {")", "("}, {"]", "["}, {"}", "{"}};

const std::map<char, Solver::Type> Solver::token_map = {
    {'0', NUMBER},    {'1', NUMBER},   {'2', NUMBER},   {'3', NUMBER},
    {'4', NUMBER},    {'5', NUMBER},   {'6', NUMBER},   {'7', NUMBER},
    {'8', NUMBER},    {'9', NUMBER},   {'.', NUMBER},

    {'(', BRACE},     {')', BRACE},    {'[', BRACE},    {']', BRACE},
    {'{', BRACE},     {'}', BRACE},

    {'+', OPERATOR},  {'-', OPERATOR}, {'*', OPERATOR}, {'/', OPERATOR},
    {'^', OPERATOR},  {'%', OPERATOR},

    {' ', WHITESPACE}};

const std::map<std::string, int> Solver::operator_level = {{"(", 0}, {"[", 0}, {"{", 0},

                                                           {"+", 1}, {"-", 1},

                                                           {"*", 2}, {"/", 2}, {"%", 2},

                                                           {"^", 3},

                                                           {")", 4}, {"]", 4}, {"}", 4}};

const std::map<std::string, std::function<float(const float&, const float&)>> Solver::operator_map =
    {{"+", [](const float& l, const float& r) { return l + r; }},
     {"-", [](const float& l, const float& r) { return l - r; }},
     {"*", [](const float& l, const float& r) { return l * r; }},
     {"/", [](const float& l, const float& r) { return l / r; }},
     {"^", [](const float& l, const float& r) { return pow(l, r); }},
     {"%", [](const float& l, const float& r) { return fmod(l, r); }}};

float Solver::eval(const std::string& expr)
{
    std::vector<Token> tokens;
    std::vector<Token> postfix;
    float result;
    unsigned int err = -1;

    if (!tokenize(expr, tokens))
        return err;
    if (!is_balanced(tokens))
        return err;
    if (!to_postfix(tokens, postfix))
        return err;
    if (!eval_postfix(postfix, result))
        return err;

    return result;
}

bool Solver::tokenize(const std::string& text, std::vector<Solver::Token>& outTokens)
{
    std::string stack = "";
    int number_index = -1;
    int index = 0;

    for (auto it = text.cbegin(); it != text.cend(); ++it)
    {
        if (token_map.find(*it) == token_map.end())
        {
            std::cerr << std::string(index, ' ') << "^" << std::endl;
            std::cerr << "Syntax Error: Unknown Symbol at position " << index << std::endl;
            return false;
        }

        if (token_map.at(*it) == Type::NUMBER)
        {
            stack += *it;
            if (number_index == -1)
            {
                number_index = index;
            }
        }
        else
        {
            if (stack != "")
            {
                if (std::count(stack.begin(), stack.end(), '.') > 1)
                {
                    std::cerr << std::string(number_index, ' ') << "^" << std::endl;
                    std::cerr << "Syntax Error: Malformed number at position " << number_index
                              << std::endl;
                    return false;
                }

                outTokens.push_back({stack, Type::NUMBER, number_index});
                stack = "";
                number_index = -1;
            }
            outTokens.push_back({std::string(1, *it), token_map.at(*it), index});
        }

        ++index;
    }

    // If last token was a number we need to flush the stack
    if (stack != "")
    {
        outTokens.push_back({stack, Type::NUMBER, number_index});
        stack = "";
    }

    if (VERBOS)
    {
        for (auto const& i : outTokens)
        {
            std::cout << "{'" << i.value << "': " << i.type << ", " << i.position << "}, ";
        }
        std::cout << std::endl;
    }

    return true;
}

bool Solver::is_balanced(const std::vector<Token>& tokens)
{
    std::vector<Token> stack;

    for (auto it = tokens.begin(); it != tokens.end(); ++it)
    {
        if (it->type == Type::BRACE)
        {
            // Check for opening braces
            if (operator_level.at(it->value) == 0)
            {
                stack.push_back(*it);
            }
            // Check for closing braces
            else if (operator_level.at(it->value) == 4)
            {
                // Check if closing brace has its corresponding opening brace
                if (!stack.empty() && brace_closure.at(it->value) == stack.back().value)
                {
                    stack.pop_back();
                }
                else
                {
                    std::cerr << std::string(it->position, ' ') << "^" << std::endl;
                    std::cerr
                        << "Syntax Error: Missmatched or unnecessary closing brace at position "
                        << it->position << std::endl;
                    return false;
                }
            }
        }
    }

    if (!stack.empty())
    {
        std::cerr << std::string(stack.back().position, ' ') << "^" << std::endl;
        std::cerr << "Syntax Error: Missing corresponding closing braces for '"
                  << stack.back().value << "' at position " << stack.back().position << std::endl;
        return false;
    }

    return true;
}

bool Solver::to_postfix(const std::vector<Token>& tokens, std::vector<Token>& outPostfix)
{
    std::vector<std::pair<Token, int>> stack;

    for (auto it = tokens.begin(); it != tokens.end(); ++it)
    {
        // Add Numbers directly to output
        if (it->type == Type::NUMBER)
        {
            outPostfix.push_back(*it);
        }
        // Handle cases where token is an operator
        else if (it->type == Type::OPERATOR)
        {
            // Pop stack to output as long as stack is not empty and
            // operator level is not greater or equal the qurrent operator
            while (!stack.empty() && stack.back().second >= operator_level.at(it->value))
            {
                outPostfix.push_back(stack.back().first);
                stack.pop_back();
            }

            // push operator to stack
            stack.push_back({*it, operator_level.at(it->value)});
        }
        // Handle cases where token is a brace
        else if (it->type == Type::BRACE)
        {
            // Opening braces
            if (operator_level.at(it->value) == 0)
            {
                stack.push_back({*it, operator_level.at(it->value)});
            }
            // Closing braces
            else if (operator_level.at(it->value) == 4)
            {
                // Pop stack to output as long as no matching opening brace is detected
                while (!stack.empty() && stack.back().first.value != brace_closure.at(it->value))
                {
                    outPostfix.push_back(stack.back().first);
                    stack.pop_back();
                }

                // Pop matching brace
                stack.pop_back();
            }
        }
    }

    // Pop all remaining tokens from stack to output in reverse order
    for (auto it = stack.rbegin(); it != stack.rend(); ++it)
    {
        outPostfix.push_back(it->first);
    }

    if (VERBOS)
    {
        for (auto const& i : outPostfix)
        {
            std::cout << "{'" << i.value << "': " << i.type << ", " << i.position << "}, ";
        }
        std::cout << std::endl;
    }

    return true;
}

bool Solver::eval_postfix(const std::vector<Token>& tokens, float& outResult)
{
    std::vector<float> stack;

    for (auto it = tokens.begin(); it != tokens.end(); ++it)
    {
        // Push numbers directly to stack
        if (it->type == Type::NUMBER)
        {
            stack.push_back(std::stof(it->value));
        }
        else if (it->type == Type::OPERATOR)
        {
            if (stack.size() < 2)
            {
                std::cerr << std::string(it->position, ' ') << "^" << std::endl;
                std::cerr << "Invalid Expression at position " << it->position << std::endl;
                return false;
            }
            // Evaluate left and right numbers with current operator
            else
            {
                float right = stack.back();
                stack.pop_back();
                float left = stack.back();
                stack.pop_back();

                stack.push_back(operator_map.at(it->value)(left, right));

                if (VERBOS)
                {
                    std::cout << "\t" << left << " " << it->value << " " << right << " = "
                              << stack.back() << std::endl;
                }
            }
        }
    }

    if (stack.size() != 1)
    {
        std::cerr << "Invalid Expression!" << std::endl;
        return false;
    }

    outResult = stack.back();

    return true;
}
