#pragma once

#include <vector>

#ifndef SIMWEI_LINE_H
#define SIMWEI_LINE_H

class Line {
public:
  Line();
  Line(Line const &line);
  Line(size_t size);
  ~Line();
  size_t size() const;
  float x(size_t it) const;
  float &x(size_t it);
  float y(size_t it) const;
  float &y(size_t it);

private:
  size_t m_size;
  std::vector<float> m_x;
  std::vector<float> m_y;
}; /* end class Line */

#endif // SIMWEI_LINE_H
