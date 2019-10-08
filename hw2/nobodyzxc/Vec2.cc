#include "Vec2.h"
#include <cmath>
#include <assert.h>
#include <sstream>

Vec2::Vec2(){
  x_ = y_ = 0;
}

Vec2::Vec2(Vec2 const &v){
  x_ = v.x(), y_ = v.y();
}

Vec2::Vec2(Vec2 &&v){
  x_ = v.x(), y_ = v.y();
}

Vec2 Vec2::copy(Vec2 const &v){
  return Vec2(v);
}

Vec2 &Vec2::operator=(Vec2 const &){
  return *this;
}

Vec2 &Vec2::operator=(Vec2 &&){
  return *this;
}

Vec2::Vec2(float x, float y){
  x_ = x, y_ = y;
}

Vec2::~Vec2(){ }

float Vec2::size() const{
  return sqrt((x_ * x_) + (y_ * y_));
}

float Vec2::dot(Vec2 v) const{
  return x_ * v.x() + y_ * v.y();
}

float Vec2::angle(Vec2 v) const{
  float mul_length = this->size() * v.size();
  assert(mul_length != 0);
  return acos(this->dot(v) / mul_length);
}

float const &Vec2::x() const{
  return x_;
}

float &Vec2::x(float v){
  return x_ = v;
}

float const &Vec2::y() const{
  return y_;
}

float &Vec2::y(float v){
  return y_ = v;
}

std::string Vec2::repr(){
  std::ostringstream ss;
  ss << "<vec2.vec2 (" << x_ << ", " << y_ << ") at "
     << std::hex << (void*)this << ">";
  return ss.str();
}
