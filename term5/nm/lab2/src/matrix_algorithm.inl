#include <cmath>
#include <cassert>
#include <vector>
#include <iomanip>
#include <numeric>

#include "math.hpp"

namespace nm_lab2::matrix_algorithm {

namespace _detail {

template <auto kBegin, auto kEnd, auto kStep, typename Function>
constexpr void ConstexprForLoop(Function&& function) {
  if constexpr (kBegin < kEnd) {
    function(std::integral_constant<decltype(kBegin), kBegin>());
    ConstexprForLoop<kBegin + kStep, kEnd, kStep>(function);
  }
}

} // namespace _detail

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr bool IsSymmetrical(const Matrix<T, kRows, kColumns>& matrix) {
  if (kRows != kColumns) {
    return false;
  }

  for (std::size_t i = 0; i < matrix.GetRows(); ++i) {
    for (std::size_t j = 0; j < matrix.GetColumns(); ++j) {
      if (matrix[i][j] != matrix[j][i]) {
        return false;
      }
    }
  }

  return true;
}

template <typename T, std::size_t kRows, std::size_t kColumns>
constexpr Matrix<T, kColumns, kRows> Transpose(const Matrix<T, kRows, kColumns>& matrix) {
  Matrix<T, kColumns, kRows> result;

  for (std::size_t i = 0; i < kRows; ++i) {
    for (std::size_t j = 0; j < kColumns; ++j) {
      result[j][i] = matrix[i][j];
    }
  }

  return result;
}

template <
    std::size_t kRowBegin,  std::size_t kColumnBegin,
    std::size_t kRowEnd,    std::size_t kColumnEnd,

    typename T,
    std::size_t kRows,      std::size_t kColumns
>
constexpr Matrix<T, kRowEnd - kRowBegin, kColumnEnd - kColumnBegin> SubMatrix(
  const Matrix<T, kRows, kColumns>& matrix
) {
  Matrix<T, kRowEnd - kRowBegin, kColumnEnd - kColumnBegin> sub_matrix;

  for (std::size_t i = 0; i < sub_matrix.GetRows(); ++i) {
    for (std::size_t j = 0; j < sub_matrix.GetColumns(); ++j) {
      sub_matrix[i][j] = matrix[i + kRowBegin][j + kColumnBegin];
    }
  }

  return sub_matrix;
}

template <Arithmetic T, std::size_t kSize>
Vector<T, kSize> SquareRootMethod(
  const Matrix<T, kSize, kSize>&  coefficients,
  const Vector<T, kSize>&         constant_terms
) {
  assert(IsSymmetrical(coefficients));

  Matrix<T, kSize, kSize> d;
  Matrix<T, kSize, kSize> s;

  for (std::size_t i = 0; i < kSize; ++i) {
    T sum = T();

    for (std::size_t j = 0; j < i; ++j) {
      sum += s[j][i] * s[j][i] * d[j][j];
    }

    // Matrix d
    d[i][i] = math::Sgn(coefficients[i][i] - sum);

    // Matrix s
    s[i][i] = std::sqrt(std::abs(coefficients[i][i] - sum));

    for (std::size_t j = i + 1; j < kSize; ++j) {
      sum = T();

      for (std::size_t k = 0; k < i; ++k) {
        sum += s[k][i] * d[k][k] * s[k][j];
      }

      s[i][j] = (coefficients[i][j] - sum) / (d[i][i] * s[i][i]);
    }
  }

  auto st_d = matrix_algorithm::Transpose(s) * d;

  // Vector y
  Vector<T, kSize> y;

  for (std::size_t i = 0; i < kSize; ++i) {
    T sum = T();

    for (std::size_t j = 0; j < i; ++j) {
      sum += st_d[i][j] * y[j];
    }

    y[i] = (constant_terms[i] - sum) / st_d[i][i];
  }

  // Vector x
  Vector<T, kSize> x;

  for (std::size_t i = kSize - 1; i + 1 > 0; --i) {
    T sum = T();

    for (std::size_t j = kSize - 1; j + 1 > i; --j) {
      sum += s[i][j] * x[j];
    }

    x[i] = (y[i] - sum) / s[i][i];
  }

  return x;
}

template <Arithmetic T, std::size_t kSize>
Vector<T, kSize> SeidelMethod(
  const Matrix<T, kSize, kSize>&  coefficients,
  const Vector<T, kSize>&         constant_terms,
  T                               epsilon
) {
  assert(IsSymmetrical(coefficients));

  // Check if Seidel's method is coinciding
  _detail::ConstexprForLoop<0, kSize, 1>([&](auto i) {
    assert(GetDeterminant(SubMatrix<0, 0, i + 1, i + 1>(coefficients)) > 0);
  });

  Vector<T, kSize> x;

  while (true) {
    auto previous_x = x;

    for (std::size_t i = 0; i < kSize; ++i) {
      T sum = T();

      for (std::size_t j = 0; j < kSize; ++j) {
        if (j == i) {
          continue;
        }

        sum += coefficients[i][j] * x[j];
      }

      x[i] = (constant_terms[i] - sum) / coefficients[i][i];
    }

    std::vector<T> deltas(kSize);

    for (std::size_t i = 0; i < kSize; ++i) {
      deltas[i] = std::abs(previous_x[i] - x[i]);
    }

#ifdef NM_LAB2_DEBUG
    std::cout << "x: ";

    for (const auto& c : x) {
      std::cout << std::fixed << std::setprecision(7) << c << "\t";
    }

    std::cout << std::endl;
#endif

    if (*std::max_element(std::begin(deltas), std::end(deltas)) <= epsilon) {
      break;
    }
  }

  return x;
}

template <Arithmetic T, std::size_t kSize>
T GetDeterminant(const Matrix<T, kSize, kSize>& matrix) {
  assert(IsSymmetrical(matrix));

  Matrix<T, kSize, kSize> d;
  Matrix<T, kSize, kSize> s;

  for (std::size_t i = 0; i < kSize; ++i) {
    T sum = T();

    for (std::size_t j = 0; j < i; ++j) {
      sum += s[j][i] * s[j][i] * d[j][j];
    }

    // Matrix d
    d[i][i] = math::Sgn(matrix[i][i] - sum);

    // Matrix s
    s[i][i] = std::sqrt(std::abs(matrix[i][i] - sum));

    for (std::size_t j = i + 1; j < kSize; ++j) {
      sum = T();

      for (std::size_t k = 0; k < i; ++k) {
        sum += s[k][i] * d[k][k] * s[k][j];
      }

      s[i][j] = (matrix[i][j] - sum) / (d[i][i] * s[i][i]);
    }
  }

  T product = T(1);

  for (std::size_t i = 0; i < kSize; ++i) {
    product *= d[i][i] * s[i][i] * s[i][i];
  }

  return product;
}

template <Arithmetic T, std::size_t kRows, std::size_t kColumns>
constexpr T GetNorm(const Matrix<T, kRows, kColumns>& matrix) {
  T norm;

  for (std::size_t i = 0; i < kRows; ++i) {
    auto sum = std::accumulate(matrix[i].Begin(), matrix[i].End(), T(), [](T sum, T coefficient) {
      return sum + static_cast<T>(std::abs(coefficient));
    });

    norm = i == 0 ? norm : std::max(norm, sum);
  }

  return norm;
}

template <Arithmetic T, std::size_t kSize>
constexpr Matrix<T, kSize, kSize> GetInverse(const Matrix<T, kSize, kSize>& matrix) {
  // Gauss method
  auto result = matrix;
  auto e = Matrix<T, kSize, kSize>(1);

  for (std::size_t k = 0; k < kSize; ++k) {
    auto diagonal_element = result[k][k];

    for (std::size_t j = 0; j < kSize; ++j) {
      result[k][j] /= diagonal_element;
      e[k][j] /= diagonal_element;
    }

    for (std::size_t i = k + 1; i < kSize; ++i) {
      auto coefficient = result[i][k];

      for (std::size_t j = 0; j < kSize; ++j) {
        result[i][j] -= result[k][j] * coefficient;
        e[i][j] -= e[k][j] * coefficient;
      }
    }
  }

  for (std::size_t k = kSize - 1; k > 0; --k) {
    for (std::size_t i = k - 1; i + 1 > 0; --i) {
      auto coefficient = result[i][k];

      for (std::size_t j = 0; j < kSize; ++j) {
        result[i][j] -= result[k][j] * coefficient;
        e[i][j] -= e[k][j] * coefficient;
      }
    }
  }

  return e;
}

template <Arithmetic T, std::size_t kSize>
constexpr T GetConditionNumber(const Matrix<T, kSize, kSize>& matrix) {
  return GetNorm(matrix) * GetNorm(GetInverse(matrix));
}

} // namespace nm_lab2::matrix_algorithm
