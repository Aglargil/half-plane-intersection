#include <matplot/matplot.h>

#include <iostream>

#include "half_plane_intersection.h"

class MyLine : public wangzheng::Line {
 public:
  MyLine(const wangzheng::Point& start, const wangzheng::Point& end, int id)
      : wangzheng::Line(start, end, id) {}
};

int main() {
  using namespace wangzheng;
  std::deque<MyLine> input_lines;
  input_lines.emplace_back(Point(0, 0), Point(1, 0), 1);
  input_lines.emplace_back(Point(1, 0), Point(1, 1), 2);
  input_lines.emplace_back(Point(1, 1), Point(0, 1), 3);
  input_lines.emplace_back(Point(0, 1), Point(0, 0), 4);
  input_lines.emplace_back(Point(0.5, 0.5), Point(0.5, 1.5), 5);

  std::deque<MyLine> output_lines = HalfPlaneIntersection(input_lines);

  // visualize
  matplot::figure(1);
  matplot::hold(matplot::on);
  for (const auto& line : input_lines) {
    std::vector<double> x = {line.start.x(), line.end.x()};
    std::vector<double> y = {line.start.y(), line.end.y()};
    std::cout << "x:" << x[0] << "," << x[1] << std::endl;
    std::cout << "y:" << y[0] << "," << y[1] << std::endl;
    matplot::plot(x, y, "r-o");
  }

  std::cout << "output_lines.size():" << output_lines.size() << std::endl;
  for (const auto& line : output_lines) {
    std::vector<double> x = {line.start.x(), line.end.x()};
    std::vector<double> y = {line.start.y(), line.end.y()};
    matplot::plot(x, y, "b-o");
  }
  matplot::axis({-1, 2, -1, 2});
  matplot::xlabel("X");
  matplot::ylabel("Y");
  matplot::hold(matplot::off);
  matplot::show();

  return 0;
}