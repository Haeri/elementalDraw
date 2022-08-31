#ifndef SOLVER_H
#define SOLVER_H

#include <functional>
#include <map>
#include <string>
#include <vector>

class Solver
{
public:
    static float eval(const std::string& expr);

private:
    enum Type
    {
        NUMBER,
        BRACE,
        OPERATOR,
        WHITESPACE,
        INVALID
    };

    struct Token
    {
        std::string value;
        Type type;
        int position;
    };

    static const std::map<std::string, std::string> brace_closure;
    static const std::map<char, Type> token_map;
    static const std::map<std::string, int> operator_level;
    static const std::map<std::string, std::function<float(const float&, const float&)>>
        operator_map;

    static bool tokenize(const std::string& text, std::vector<Token>& outTokens);
    static bool is_balanced(const std::vector<Token>& tokens);
    static bool to_postfix(const std::vector<Token>& tokens, std::vector<Token>& outPostfix);
    static bool eval_postfix(const std::vector<Token>& tokens, float& outResult);
};

#endif // SOLVER_H