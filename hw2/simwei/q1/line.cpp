#include "line.h"

Line::~Line() {}

Line::Line(size_t size) {
  m_size = size;

  m_x = std::vector<float>();
  m_x.resize(m_size);
  m_y = std::vector<float>();
  m_y.resize(m_size);
}

Line::Line(Line const &line) {
  m_x = line.m_x;
  m_y = line.m_y;
  m_size = line.m_size;
}

size_t Line::size() const { return m_size; }

float &Line::x(size_t it) { return m_x.at(it); }

float Line::x(size_t it) const { return m_x.at(it); }

float &Line::y(size_t it) { return m_y.at(it); }

float Line::y(size_t it) const { return m_y.at(it); }