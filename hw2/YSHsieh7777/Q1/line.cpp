
#include "line.hpp"

Line::Line(size_t size)
{
    m_dots = new std::list<dot_t>(size);
}

Line::~Line()
{
    delete m_dots;
}

size_t Line::size() const
{
    return m_dots->size();
}