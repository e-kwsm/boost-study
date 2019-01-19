#include <iostream>
#include <string>
#include <boost/format.hpp>
#include <boost/units/io.hpp>
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/si/prefixes.hpp>
#include <boost/version.hpp>

int main() {
  std::cout << BOOST_LIB_VERSION << '\n';

  using length_t = boost::units::quantity<boost::units::si::length, double>;
  using dimensionless_t = boost::units::quantity<boost::units::si::dimensionless, double>;
  constexpr length_t meter{1.0 * boost::units::si::meter};
  constexpr length_t millimeter{1.0 * boost::units::si::milli * boost::units::si::meter};
  constexpr auto meter_by_meter{meter / boost::units::si::meter};
  constexpr auto meter_by_millimeter{meter / boost::units::si::milli / boost::units::si::meter};
  constexpr auto meter_by_second{meter / boost::units::si::second};
  constexpr auto millimeter_by_meter{millimeter / boost::units::si::meter};
  constexpr auto millimeter_by_millimeter{millimeter / boost::units::si::milli /
                                          boost::units::si::meter};
  constexpr auto millimeter_by_second{millimeter / boost::units::si::second};

  boost::format fmt{"%1% = %2%\n"};

  std::cout << fmt % "1 m      " % meter;
  std::cout << fmt % "1 m  / m " % meter_by_meter;
  std::cout << fmt % "1 m  / mm" % meter_by_millimeter;
  std::cout << fmt % "1 m  / s " % meter_by_second;
  std::cout << fmt % "1 mm     " % millimeter;
  std::cout << fmt % "1 mm / m " % millimeter_by_meter;
  std::cout << fmt % "1 mm / mm" % millimeter_by_millimeter;
  std::cout << fmt % "1 mm / s " % millimeter_by_second;
  std::cout << fmt % "(1 m      ).value()" % meter.value();
  std::cout << fmt % "(1 m  / m ).value()" % meter_by_meter.value();
  std::cout << fmt % "(1 m  / mm).value()" % meter_by_millimeter.value();
  std::cout << fmt % "(1 m  / s ).value()" % meter_by_second.value();
  std::cout << fmt % "(1 mm     ).value()" % millimeter.value();
  std::cout << fmt % "(1 mm / m ).value()" % millimeter_by_meter.value();
  std::cout << fmt % "(1 mm / mm).value()" % millimeter_by_millimeter.value();
  std::cout << fmt % "(1 mm / s ).value()" % millimeter_by_second.value();
  std::cout << fmt % "dimensionless(1 m  / m )" % dimensionless_t{meter_by_meter};
  std::cout << fmt % "dimensionless(1 m  / mm)" % dimensionless_t{meter_by_millimeter};
  // ill-formed
  // std::cout fmt % << "dimensionless(1 m  / s )" %
  // dimensionless_t{meter_by_second};
  std::cout << fmt % "dimensionless(1 mm / m )" % dimensionless_t{millimeter_by_meter};
  std::cout << fmt % "dimensionless(1 mm / mm)" % dimensionless_t{millimeter_by_millimeter};
  std::cout << fmt % "dimensionless(1 m  / m ).value()" % dimensionless_t{meter_by_meter}.value();
  std::cout << fmt % "dimensionless(1 m  / mm).value()"
                   % dimensionless_t{meter_by_millimeter}.value();
  std::cout << fmt % "dimensionless(1 mm / m ).value()"
                   % dimensionless_t{millimeter_by_meter}.value();
  std::cout << fmt % "dimensionless(1 mm / mm).value()"
                   % dimensionless_t{millimeter_by_millimeter}.value();

  return 0;
}
