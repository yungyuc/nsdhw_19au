
#include <cstddef>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <assert.h>

using namespace std;


class Line{

private:
    vector<float>* x_v{};
    vector<float>* y_v{};

    void check_coordinate(size_t c) const{
        if(c < 0 or c >= this->size()){
            throw out_of_range("Line index out of range");
        }
    }

public:

    Line() = default;
    Line(size_t size): x_v(new vector<float>(size)), y_v(new vector<float>(size)){};
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&) = default;

    ~Line() = default;
    size_t size() const;
    float & x(size_t it) const;
    float & x(size_t it);
    float & y(size_t it) const;
    float & y(size_t it);

};


Line::Line(Line const& l){

    Line* line = new Line(l.size());


    for(int i = 0; i < (int)l.size(); i++){
        line->x_v->at(i) = l.x(i);
        line->y_v->at(i) = l.y(i);
    }

    x_v = line->x_v;
    y_v = line->y_v;
}


Line::Line(Line && l) {
    x_v = l.x_v;
    y_v = l.y_v;
}

Line& Line::operator=(Line const & l) {
	this->x_v = std::move(l.x_v);
	this->y_v = std::move(l.y_v);
	return *this;
}

size_t Line::size() const {
    return this->x_v->size();
}

float& Line::x(size_t it) const {

    this->check_coordinate(it);
    return this->x_v->at(it);
}

float& Line::y(size_t it) const {
    this->check_coordinate(it);
    return this->y_v->at(it);
}

float& Line::x(size_t it) {
    this->check_coordinate(it);
    return this->x_v->at(it);
}

float& Line::y(size_t it) {
    this->check_coordinate(it);
    return this->y_v->at(it);
}


void printl(Line const& line){

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }
}

int main(int, char **)
{
    
	Line line1(1), line2(2);

	line1.x(0) = 4.3;
	line1.y(0) = 1;
	
	line2 = line1;
	
	line2.x(0) = 2;
	
	assert(line1.x(0) == line2.x(0));

	Line line3(line2);
	
	assert(line3.x(0) == line2.x(0));
	assert(&line2 != &line3);

	return 0;
}
