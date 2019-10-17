#include <cstddef>
#include <vector>
#include <utility>

typedef std::pair<float, float> dot_t; 

#ifndef LINE_H
#define LINE_H

class Line
{
public:
    Line() = default;
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
    std::vector<dot_t> *m_dots = nullptr;
};

#endif