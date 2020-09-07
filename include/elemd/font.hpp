#ifndef ELEMD_FONT_HPP
#define ELEMD_FONT_HPP

#include <elemd/elemental_draw.hpp>

namespace elemd
{
    class ELEMD_API font
    {
    public:
        static font* create();
        void destroy();

    protected:
        font() = default;
        virtual ~font() = default;
    };

} // namespace elemd

#endif // ELEMD_FONT_HPP