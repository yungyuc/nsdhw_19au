#pragma once

#include <vector>

class Line {
    public:
        Line();
        Line(Line const &);
        Line(Line &&);
        Line & operator=(Line const &);
        Line & operator=(Line &&);
        Line (size_t size);
        ~Line();
        size_t size() const;

        float & x(size_t it) const;
        float & x(size_t it);
        float & y(size_t it) const;
        float & y(size_t it);

    private:
        size_t get_x_shift(size_t it);
        size_t get_y_shift(size_t it);
        std::vector<float> *data_ptr;
};