#include <iostream>
#include <boost/program_options.hpp>

int main(int argc, char** argv) {
  boost::program_options::options_description options{"options"};
  options.add_options()("help,h", "display help");
  options.add_options()("int1", boost::program_options::value<int>(), "receive int");
  options.add_options()("int2", boost::program_options::value<int>()->default_value(42),
                        "receive int (default)");

  boost::program_options::variables_map vmap;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, options), vmap);

  if (vmap.count("help")) {
    std::cout << options;
    return 0;
  }

  if (vmap.count("int1")) {
    auto i = vmap["int1"].as<int>();
    std::cout << "--int1: " << i << '\n';
  }
  {
    auto i = vmap["int2"].as<int>();
    std::cout << "--int2: " << i << '\n';
  }

  return 0;
}
