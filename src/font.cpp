#include "elemd/font.hpp"

namespace elemd
{
    void font::destroy()
    {
        delete this;
    }

} // namespace elemd