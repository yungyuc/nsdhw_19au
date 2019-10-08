#include<stddef.h>
#include<string>

class Vec2{
  public:
    Vec2();
    Vec2(Vec2 const &);
    Vec2(Vec2       &&);
    Vec2 &operator=(Vec2 const &);
    Vec2 &operator=(Vec2       &&);
    Vec2(float , float);
    ~Vec2();
    static Vec2 copy(Vec2 const &);
    float size() const;
    float dot(Vec2) const;
    float angle(Vec2) const;
    float const &x() const;
    float &x(float);
    float const &y() const;
    float &y(float);
    std::string repr();
  //private:
    float x_, y_;
};
