#ifndef NOCOPYABLE_H
#define NOCOPYABLE_H

class NoCopyable
{
protected:
    NoCopyable() = default;
    ~NoCopyable() = default;

    NoCopyable(const NoCopyable& rhs) = delete;
    NoCopyable operator=(const NoCopyable& rhs) = delete;
};

#endif