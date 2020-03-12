#include <boost/process.hpp>
#include <iostream>

int main() {
  {
    std::error_code ec;
    boost::process::system("date -I", ec);
    std::cout << ec.value() << std::endl;
  }

  {
    boost::process::ipstream pipe_stream;
    boost::process::child c{"c++ --version",
                            boost::process::std_out > pipe_stream};

    std::string line;
    while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
      std::cerr << line << std::endl;

    c.wait();
  }
}
