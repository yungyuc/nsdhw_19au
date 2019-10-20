#include <vector>

struct Vertex
{
	float x, y;
};


class Line
{
public:
	Line();
	Line(size_t it){
		data.reserve(it);
	};
	~Line();



private:
	std::vector<Vertex> data;
	
};