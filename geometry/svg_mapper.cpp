#include <iostream>
#include <fstream>
#include <boost/format.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/io/svg/svg_mapper.hpp>

int main() {
  using point_t      = boost::geometry::model::d2::point_xy<int>;
  using linestring_t = boost::geometry::model::linestring<point_t>;
  using ring_t       = boost::geometry::model::ring<point_t>;
  using segment_t    = boost::geometry::model::segment<point_t>;

  const ring_t ring{{
      {100, 0},
      {50, -87},
      {-50, -87},
      {-100, 0},
      {-50, 87},
      {50, 87},
      {100, 0}
  }};
  const segment_t segment{{-50, 0}, {50, 0}};
  const linestring_t linestring{{0, 80}, {-20, 40}, {20, -40}, {0, -80}};

  std::ofstream svg{"svg_mapper.svg"};
  constexpr int width  = 300;
  constexpr int height = 600;
  auto width_height = [](auto width, auto height) {
    if (width <= 0 || height <= 0) {
      throw std::invalid_argument{""};
    }
    return (boost::format{"width=\"%1%\" height=\"%2%\""} % width % height).str();
  };
  boost::geometry::svg_mapper<point_t> mapper{svg, width, height, width_height(width, height)};

  mapper.add(ring);
  mapper.add(segment);
  mapper.add(linestring);
  mapper.map(ring, "fill-opacity: 0.5; fill: blue");
  mapper.map(segment, "stroke: green");
  mapper.map(linestring, "stroke: red");

  return 0;
}
