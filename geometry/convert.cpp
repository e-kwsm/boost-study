#include <iostream>
#include <boost/geometry/algorithms/convert.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/io/dsv/write.hpp>
#include <boost/geometry/io/svg/svg_mapper.hpp>

int main() {
  using point_t = boost::geometry::model::d2::point_xy<int>;
  using linestring_t = boost::geometry::model::linestring<point_t>;
  using polygon_t = boost::geometry::model::polygon<point_t>;
  using ring_t = boost::geometry::model::ring<point_t>;
  using segment_t = boost::geometry::model::segment<point_t>;

  const segment_t segment{{-50, 0}, {50, 0}};
  linestring_t linestring;
  boost::geometry::convert(segment, linestring);

  std::cout << boost::geometry::dsv(segment) << '\n';
  std::cout << boost::geometry::dsv(linestring) << '\n';

  const ring_t ring{{{100, 0},
                     {50, -87},
                     {-50, -87},
                     {-100, 0},
                     {-50, 87},
                     {50, 87},
                     {100, 0}}};
  polygon_t polygon;
  boost::geometry::convert(ring, polygon);

  std::cout << boost::geometry::dsv(ring) << '\n';
  std::cout << boost::geometry::dsv(polygon) << '\n';

  return 0;
}
