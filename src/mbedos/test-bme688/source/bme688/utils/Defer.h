#ifndef DEFER_H
#define DEFER_H

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

#endif