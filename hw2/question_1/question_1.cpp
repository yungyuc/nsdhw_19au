#include <vector>
#include <iostream>

class Line{

public:
	Line()=default;
	Line(Line const &old_constructor){
		m_size=old_constructor.m_size;
		m_coord=old_constructor.m_coord;
	}
	Line(Line       &&other_constructor){
		std::swap(other_constructor.m_size, m_size);
		std::swap(other_constructor.m_coord, m_coord);
	}
	Line & operator=(Line const &old_constructor){
		if (this==&old_constructor){
			return *this;
		}
		m_size=old_constructor.m_size;
		m_coord=old_constructor.m_coord;
		return *this;
	}
	Line & operator=(Line       &&other_constructor){
		if (this==&other_constructor){
			return *this;
		}
		std::swap(other_constructor.m_size, m_size);
		std::swap(other_constructor.m_coord, m_coord);
		return *this;
	}
	Line(size_t size): m_size(size), m_coord(new float[size*2]) {}
	~Line()=default;
	size_t size() const {return m_size;}
	float & x(size_t it) const {check_range(it); return m_coord[it*2];}
	float & x(size_t it) {check_range(it); return m_coord[it*2];}
	float & y(size_t it) const {check_range(it); return m_coord[it*2+1];}
	float & y(size_t it) {check_range(it); return m_coord[it*2+1];}

private:
	void check_range(size_t it) const
	{
		if (it >= m_size) {
			throw std::out_of_range("Line index out of range");
		}
	}
	size_t m_size=0;
	float * m_coord=nullptr;
};

int main(int, char **){
	Line line(3);
	line.x(0)=0;
	line.y(0)=1;
	line.x(1)=1;
	line.y(1)=3;
	line.x(2)=2;
	line.y(2)=5;

	Line line2(line);
	line2.x(0)=9;

	std::cout << "line: number of points = " << line.size()  << std::endl;
	for (size_t it=0; it<line.size(); ++it){
		std::cout << "point " << it << ":"
			<< " x= " << line.x(it)
			<< " y= " << line.y(it) << std::endl;
	}

	std::cout << "line2: number of points = " << line.size() << std::endl;
	for (size_t it=0; it<line.size(); ++it){
		std::cout << "point " << it << ":"
			<< " x= " << line2.x(it)
			<< " y= " << line2.y(it) << std::endl;
	}

	return 0;
}
