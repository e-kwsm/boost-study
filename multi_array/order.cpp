#include <iostream>
#include <boost/format.hpp>
#include <boost/multi_array.hpp>

int main() {
  constexpr unsigned ni = 3, nj = 4, nk = 5;
  constexpr unsigned num_elements = ni * nj * nk;
  boost::multi_array<unsigned, 3u> ma_c{
      boost::extents[ni][nj][nk], boost::c_storage_order{}},  // default order
      ma_f{boost::extents[ni][nj][nk], boost::fortran_storage_order{}};

  auto itr_c = ma_c.data();
  auto itr_f = ma_f.data();
  for (unsigned l = 0; l < num_elements; l++) {
    *itr_c++ = l;
    *itr_f++ = l;
  }

  boost::format fmt_ijk{"(%1%,%2%,%3%)"};

  const auto shape_c = ma_c.shape();
  const auto shape_f = ma_f.shape();
  std::cout << "#shape\n";
  std::cout << "C\t" << fmt_ijk % shape_c[0] % shape_c[1] % shape_c[2] << '\n';
  std::cout << "F\t" << fmt_ijk % shape_f[0] % shape_f[1] % shape_f[2] << '\n';

  const auto strides_c = ma_c.strides();
  const auto strides_f = ma_f.strides();
  std::cout << "#strides\n";
  std::cout << "C\t" << fmt_ijk % strides_c[0] % strides_c[1] % strides_c[2]
            << '\n';
  std::cout << "F\t" << fmt_ijk % strides_f[0] % strides_f[1] % strides_f[2]
            << '\n';

  std::cout << "#order\n";
  std::cout << "#indices\tC\tF\n";
  for (unsigned i = 0; i < ni; i++)
    for (unsigned j = 0; j < nj; j++)
      for (unsigned k = 0; k < nk; k++) {
        // operator[]
        std::cout << fmt_ijk % i % j % k << '\t' << ma_c[i][j][k] << '\t'
                  << ma_f[i][j][k] << '\n';
        // operator()
        // std::array<unsigned, 3u> index_list{i, j, k};
        // std::cout << fmt_ijk % i % j % k << '\t' << ma_c(index_list)
        // << '\t' << ma_f(index_list) << '\n';
      }

  return 0;
}
