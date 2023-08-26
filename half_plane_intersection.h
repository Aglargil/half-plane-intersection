#pragma once

#include <matplot/matplot.h>

#include <Eigen/Dense>
#include <deque>
#include <optional>
#include <vector>

namespace wangzheng {

static double CrossProduct(const Eigen::Vector2d& v1,
                           const Eigen::Vector2d& v2) {
  return v1.x() * v2.y() - v1.y() * v2.x();
}

using Point = Eigen::Vector2d;

class Line {
 public:
  Line(const Point& start, const Point& end, int id)
      : start(start), end(end), id(id) {
    direction = end - start;
    angle = std::atan2(direction.y(), direction.x());
  }
  Point start;
  Point end;
  Eigen::Vector2d direction;
  double angle;
  int id;

  friend std::ostream& operator<<(std::ostream& os, const Line& line) {
    os << "start:" << line.start.x() << "," << line.start.y()
       << " end:" << line.end.x() << "," << line.end.y() << std::endl;
    return os;
  }
};

static bool PointOnRight(const Line& line, const Point& point) {
  Eigen::Vector2d v = point - line.start;
  return CrossProduct(line.direction, v) < 0;
}

// sort by angle, if angle is same, left line advance
static bool Cmp(const Line& a, const Line& b) {
  if (std::abs(a.angle - b.angle) > 1e-6) {
    return a.angle < b.angle;
  } else {
    return PointOnRight(a, b.start);
  }
}

static std::optional<Point> GetIntersection(const Line& a, const Line& b) {
  Eigen::Vector2d va = a.direction;
  Eigen::Vector2d vb = b.direction;
  if (std::abs(CrossProduct(va, vb)) < 1e-6) {
    return std::nullopt;
  }
  Eigen::Vector2d s = b.start - a.start;
  return a.start + va * CrossProduct(s, vb) / CrossProduct(va, vb);
}

static bool IntersectionOnRight(const Line& line, const Line& a,
                                const Line& b) {
  std::optional<Point> intersection = GetIntersection(a, b);
  if (!intersection.has_value()) {
    return false;
  }
  return PointOnRight(line, intersection.value());
}

template <typename LineT>
inline std::deque<LineT> HalfPlaneIntersection(const std::deque<LineT>& lines) {
  if (lines.empty()) {
    return {};
  }
  std::deque<LineT> a = lines;
  std::sort(a.begin(), a.end(), Cmp);
  std::deque<LineT> q{a[0]};
  for (int i = 1; i < a.size(); ++i) {
    if (a[i].angle - a[i - 1].angle < 1e-6) continue;
    while (q.size() >= 2 &&
           IntersectionOnRight(a[i], q[q.size() - 2], q[q.size() - 1])) {
      q.pop_back();
    }
    while (q.size() >= 2 && IntersectionOnRight(a[i], q[0], q[1])) {
      q.pop_front();
    }
    q.push_back(a[i]);
  }
  while (q.size() >= 2 &&
         IntersectionOnRight(q[0], q[q.size() - 2], q[q.size() - 1])) {
    q.pop_back();
  }
  return q;
}

}  // namespace wangzheng
