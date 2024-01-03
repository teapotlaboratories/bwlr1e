#ifndef UTILS_DEFER_H
#define UTILS_DEFER_H

namespace teapotlabs{
namespace utils {
    
template <class T>
class Defer
{
    T& deferred_object;
    T target_object;

    public:
        Defer( T& deferred, T target ):deferred_object(deferred), target_object(target) {}
        ~Defer()
        {
            deferred_object = target_object;
        };
};

} // namespace utils
} // namespace teapotlabs

#endif