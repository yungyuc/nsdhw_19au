#include <stdexcept>
#include <algorithm>
#include <vector>
#include <iostream>

#include "line.h"

Line::Line(size_t size) : m_size(size), m_x(size, 0.0), m_y(size, 0.0) {}
/* Define the copy constructor */
Line::Line(Line const &other)
{
    m_size = other.m_size;
    m_x = vector<float>(other.m_x);
    m_y = vector<float>(other.m_y);
}

/* Define the move constructor */
Line::Line(Line &&other)
{
    std::swap(other.m_size, m_size);
    std::swap(other.m_x, m_x);
    std::swap(other.m_y, m_y);
}

/* Define the copy assignment operator */
Line &Line::operator=(Line const &other)
{
    if (this == &other)
    {
        return *this;
    } // don't move to self.
    // This part is the same as what's in the copy constructor.
    m_size = other.m_size;
    m_x = vector<float>(other.m_x);
    m_y = vector<float>(other.m_y);
    return *this;
}

/* Define the move assignment operator */
Line &Line::operator=(Line &&other)
{
    if (this == &other)
    {
        return *this;
    } // don't move to self.
    std::swap(other.m_size, m_size);
    std::swap(other.m_x, m_x);
    std::swap(other.m_y, m_y);
    return *this;
}

void Line::check_range(size_t it) const
{
    if (it >= m_size)
    {
        throw std::out_of_range("Line index out of range");
    }
}
size_t Line::size() const
{
    return m_size;
}
float Line::x(const size_t it) const
{
    check_range(it);
    return m_x[it];
}

float &Line::x(const size_t it)
{
    check_range(it);
    return m_x[it];
}

float Line::y(const size_t it) const
{
    check_range(it);
    return m_y[it];
}

float &Line::y(const size_t it)
{
    check_range(it);
    return m_y[it];
}