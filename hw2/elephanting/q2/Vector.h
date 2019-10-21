class Vector
{
public:
    Vector(float, float);
    float len();
    float getX() const;
    float getY() const;
private:
    float m_x;
    float m_y;
};