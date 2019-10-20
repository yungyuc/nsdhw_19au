#include <vector>
#include <iostream>
#include "Line.h"	

Line::Line(){};
Line::Line(Line const& other) {
	for (size_t i=0; i<other.data.size();i++){
		data.push_back(other.data[i]);
	}
};
Line::Line(Line&& other){
	for (size_t i=0; i<other.data.size();i++){
		data.push_back(other.data[i]);
	}
	other.data.clear();
};
Line& Line::operator=(Line const& other){
	if(this == &other){
		return *this;
	}else{
		this->data = other.data;
		return *this;
	}
};
Line& Line::operator=(Line&& other){
	if(this == &other){
		return *this;
	}else{
		this->data = other.data;
		other.data.clear();
		return *this;
	}
};
Line::Line(size_t size) {
	this->data.reserve(size * 2);
};
Line::~Line(){};
size_t Line::size() const{
	return this->data.size();
};
float const & Line::x(size_t it) const{
	return this->data.at(it).x;
};
float& Line::x(size_t it){
	return this->data.at(it).x;
};
float const & Line::y(size_t it) const{
	return this->data.at(it).y;
};
float& Line::y(size_t it){
	return this->data.at(it).y;
};

