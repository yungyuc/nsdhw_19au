
#include "line.hpp"

Line::Line(size_t size)
{
    m_dots = new std::vector<dot_t>(size);
}

Line::~Line()
{
    delete m_dots;
}

size_t Line::size() const
{
    return m_dots->size();
}

Line::Line(Line const &other)
{
    if(other.m_dots)
    {
        if(m_dots)
            delete m_dots;

        m_dots = new std::vector<dot_t>(other.size());
        std::copy(other.m_dots->begin(), other.m_dots->end(), m_dots->begin());
    }
    else
    {
        if(m_dots)
        {
            delete m_dots;
            m_dots = nullptr;
        }
    }
}

Line::Line(Line &&other)
{
    if(m_dots)
    {
        delete m_dots;
        m_dots = nullptr;
    }

    m_dots = other.m_dots;
    other.m_dots = nullptr;
}

Line & Line::operator=(Line const &other)
{
    if(this == &other)
        return *this;

    if(other.m_dots)
    {
        if(m_dots)
            delete m_dots;
        m_dots = new std::vector<dot_t>(other.size());
        std::copy(other.m_dots->begin(), other.m_dots->end(), m_dots->begin());
    }
    else
    {
        if(m_dots)
        {
            delete m_dots;
            m_dots = nullptr;
        }
    }
    return *this;
}

Line & Line::operator=(Line &&other)
{
    if(this == &other)
        return *this;
    
    if(m_dots)
    {
        delete m_dots;
        m_dots = nullptr;
    }
    m_dots = other.m_dots;
    other.m_dots = nullptr;
    return *this;
}

float & Line::x(size_t it) const
{
    return m_dots->at(it).first;
}

float & Line::x(size_t it)
{
    return m_dots->at(it).first;
}

float & Line::y(size_t it) const
{
    return m_dots->at(it).second;
}

float & Line::y(size_t it)
{
    return m_dots->at(it).second;
}