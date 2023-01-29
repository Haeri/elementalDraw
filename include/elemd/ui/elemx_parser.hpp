#ifndef ELEMD_ELEMX_PARSER_HPP
#define ELEMD_ELEMX_PARSER_HPP

#include <unordered_map>

#include "elemd/ui/element.hpp"

namespace elemd
{
    class ELEMD_API elemx_parser
    {
    public:
        std::unordered_map<std::string, Element*> KEYWORDS = {
            //            {"elem", }
        };

        static Element* parse_file(const std::string& file_path);

    private:
        struct source_code
        {
            std::string text;
            int index = 0;

            char next()
            {
                return text[index++];
            }

            char hasNext()
            {
                return text.length() > index;
            }
        };

        enum Type
        {
            VOID,
            INTEGER,
            FLOAT,
            BOOL,
            STRING
        };

        static source_code* pre_process(source_code* sc);
        static void lex(source_code* sc);
        // void parse();
    };
} // namespace elemd

#endif // ELEMD_ELEMX_PARSER_HPP