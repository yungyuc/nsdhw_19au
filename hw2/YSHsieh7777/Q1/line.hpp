#include <cstddef>
#include <list>
#include <utility>

typedef std::pair<float, float> dot_t; 

#ifndef LINE_H
#define LINE_H

class Line
{
public:
    Line();
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size);
    ~Line();
    size_t size() const;
    float & x(size_t it) const;
    float & x(size_t it);
    float & y(size_t it) const;
    float & y(size_t it);

private:
    void check_range(size_t it) const;
    std::list<dot_t> *m_dots;
};

#endif