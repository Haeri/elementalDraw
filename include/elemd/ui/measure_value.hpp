#ifndef ELEMD_MEASURE_VALUE_HPP
#define ELEMD_MEASURE_VALUE_HPP

#include <string>

#include "elemd/elemental_draw.hpp"

namespace elemd
{
    class ELEMD_API value
    {
    public:
        enum Type
        {
            PIXELS,
            PERCENT,
            AUTO
        };

        value()
        {
        }
        value(std::string value);
        value(float value, Type type);

        void set_pixels(float pixels);
        void set_percent(float percent);

        float get_value();
        Type get_type();

    private:
        Type _type = Type::AUTO;
        float _value = -1;
    };
} // namespace elemd

#endif // ELEMD_MEASURE_VALUE_HPP