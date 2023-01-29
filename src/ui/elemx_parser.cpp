#include "elemd/ui/elemx_parser.hpp"

#include <fstream>
#include <regex>
#include <sstream>

namespace elemd
{
    Element* elemx_parser::parse_file(const std::string& file_path)
    {
        source_code sc;
        std::ifstream inFile;
        inFile.open(file_path);

        std::stringstream strStream;
        strStream << inFile.rdbuf();
        sc.text = strStream.str();

        sc = *elemx_parser::pre_process(&sc);

        elemx_parser::lex(&sc);

        return nullptr;
    }

    elemx_parser::source_code* elemx_parser::pre_process(source_code* sc)
    {
        std::regex e_comment("#.*");
        std::regex e_ml_comment("/\\*(.|\n|\n\r)*\\*/");
        std::regex e_tabs("\t");
        std::regex e_many_space("( +)");
        std::regex e_many_breaks("(\n+)");

        sc->text = std::regex_replace(sc->text, e_comment, "");
        sc->text = std::regex_replace(sc->text, e_ml_comment, "");
        sc->text = std::regex_replace(sc->text, e_tabs, "");
        sc->text = std::regex_replace(sc->text, e_many_space, " ");
        sc->text = std::regex_replace(sc->text, e_many_breaks, "\n");

        return sc;
    }

    void elemx_parser::lex(source_code* sc)
    {
        /*
        std::map<std::string, Variable> varmap;
        int iter = 0;
        bool parsing = true;
        std::string token;
        std::string varname;
        Type type;

        bool assignment = false;
        bool typeSet = false;

        while (parsing || iter == 100) {
            ++iter;
            char character = sc.next();
            if (!sc.hasNext()) parsing = false;

            if (character == ' '
                || character == '\n' || character == '\r'
                || character == '(' || character == ')' || character == '{'
                || character == '}')
            {
                std::cout << "Token: " << token << "\n";

                if (token == T_INTEGER)
                {
                    typeSet = true;
                    type = Type::INTEGER;
                }
                else if (token == T_FLOAT)
                {
                    typeSet = true;
                    type = Type::FLOAT;
                }
                else if (token == T_BOOLEAN)
                {
                    typeSet = true;
                    type = Type::BOOL;
                }
                else if (token == T_STRING)
                {
                    typeSet = true;
                    type = Type::STRING;
                }
                else if (token == ASSIGNMENT) {
                    assignment = true;
                }
                else if (assignment)
                {
                    Variable v = { varname, token, type };
                    varmap[varname] = v;

                    assignment = false;
                    typeSet = false;
                    varname = "";
                    typeSet = Type::VOID;
                }
                else
                {
                    varname = token;
                }
                token = "";
            }
            else {
                token += character;
            }
        }

        return varmap;

        */
    }

} // namespace elemd
