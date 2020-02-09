#ifndef ITERATOR
#define ITERATOR

template <typename Iterator>
class IIterable
{

    virtual Iterator begin() const noexcept = 0;
    virtual Iterator end() const noexcept = 0;

};

#endif