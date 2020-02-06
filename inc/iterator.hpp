#ifndef ITERATOR
#define ITERATOR

template <typename Iterator>
class IIterable
{

    virtual Iterator begin() = 0;
    virtual Iterator end() = 0;

};

#endif