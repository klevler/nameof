// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2018 - 2019 Daniil Goncharov <neargye@gmail.com>.
//
// Permission is hereby  granted, free of charge, to any  person obtaining a copy
// of this software and associated  documentation files (the "Software"), to deal
// in the Software  without restriction, including without  limitation the rights
// to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
// copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
// IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
// FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
// AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
// LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <nameof.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

constexpr long double operator"" _deg(long double deg) {
  return deg * 3.141592 / 180.0;
}

std::string operator"" _string(const char* str, std::size_t) {
  return std::string{str};
}

struct SomeStruct {
  int somefield = 0;

  void SomeMethod1(int i) { somefield = i; }

  int SomeMethod2() const { return somefield; }
};

void SomeMethod3() {
  std::cout << NAMEOF(SomeMethod3) << " no called!" << std::endl;
}

template <typename T, typename U>
std::string SomeMethod4(U value) {
  std::stringstream s;
  s << NAMEOF(SomeMethod4<T, U>) << "<" << NAMEOF_TYPE(T) << ", " << NAMEOF_TYPE(U) << ">(" << NAMEOF_TYPE(U) << " " << NAMEOF(value) << ")";
  return s.str();
}

template <typename T>
class SomeClass {
 public:
  void SomeMethod5() const {
    std::cout << nameof::nameof_type<T>() << std::endl;
  }

  template <typename C>
  C SomeMethod6() const {
    C t{};
    std::cout << NAMEOF_VAR_TYPE(t) << std::endl;
    return t;
  }
};

struct Long {
  struct LL {
    int field = 0;
  };
  LL ll;
};

enum class Color { RED, GREEN, BLUE };

SomeStruct structvar;
Long othervar;
SomeStruct* ptrvar = &structvar;

int main() {
  // Compile-time.
  constexpr auto name = NAMEOF(structvar);
  static_assert("structvar" == name);

  // Nameof enum variable.
  auto color = Color::RED;
  std::cout << nameof::nameof_enum(color) << std::endl; // 'RED'
  std::cout << NAMEOF_ENUM(color) << std::endl; // 'RED'

  // Nameof.
  std::cout << NAMEOF(structvar) << std::endl; // 'structvar'
  std::cout << NAMEOF(::structvar) << std::endl; // 'structvar'
  std::cout << NAMEOF(structvar.somefield) << std::endl; // 'somefield'
  std::cout << NAMEOF((&structvar)->somefield) << std::endl; // 'somefield'
  std::cout << NAMEOF(othervar.ll.field) << std::endl; // 'field'
  std::cout << NAMEOF(ptrvar) << std::endl; // 'ptrvar

  // Nameof function.
  std::cout << NAMEOF(&SomeStruct::SomeMethod1) << std::endl; // 'SomeMethod1'
  std::cout << NAMEOF(structvar.SomeMethod2()) << std::endl; // 'SomeMethod2'
  std::cout << NAMEOF(SomeMethod3) << std::endl; // 'SomeMethod3'
  std::cout << NAMEOF(SomeMethod4<int, float>(1.0f)) << std::endl; // 'SomeMethod4'
  std::cout << NAMEOF(&SomeClass<int>::SomeMethod5) << std::endl; // 'SomeMethod5'
  std::cout << NAMEOF(&SomeClass<int>::SomeMethod6<long int>) << std::endl; // 'SomeMethod6'

  // Nameof with template suffix.
  std::cout << NAMEOF_FULL(SomeMethod4<int, float>) << std::endl; // 'SomeMethod4<int, float>'
  std::cout << NAMEOF_FULL(&SomeClass<int>::SomeMethod6<long int>) << std::endl; // 'SomeMethod6<long int>'

  // Nameof variable type.
  std::cout << nameof::nameof_type<decltype(structvar)>() << std::endl; // 'SomeStruct'
  std::cout << NAMEOF_VAR_TYPE(structvar) << std::endl; // 'SomeStruct'
  std::cout << NAMEOF_VAR_TYPE(SomeClass<int>{}) << std::endl; // 'SomeClass<int>'
  std::cout << NAMEOF_VAR_TYPE(othervar.ll) << std::endl; // 'Long::LL'

  // Nameof type.
  std::cout << nameof::nameof_type<SomeClass<int>>() << std::endl; // 'SomeClass<int>'
  std::cout << NAMEOF_TYPE(SomeClass<int>) << std::endl; // 'SomeClass<int>'
  std::cout << NAMEOF_TYPE(Long::LL) << std::endl; // 'Long::LL'

  // Nameof macro.
  std::cout << NAMEOF(__LINE__) << std::endl; // '__LINE__'
  std::cout << NAMEOF(NAMEOF(structvar)) << std::endl; // 'NAMEOF'

  // Nameof raw.
  std::cout << NAMEOF_RAW(structvar.somefield) << std::endl; // 'structvar.somefield'
  std::cout << NAMEOF_RAW(&SomeStruct::SomeMethod1) << std::endl; // '&SomeStruct::SomeMethod1'

  // Some more complex example.

  std::cout << SomeMethod4<int>(structvar) << std::endl; // 'SomeMethod4<int, SomeStruct>(SomeStruct value)'

  const auto div = [](int x, int y) -> int {
    if (y == 0) {
      throw std::invalid_argument(std::string(NAMEOF(y)).append(" should not be zero!"));
    }
    return x / y;
  };

  try {
    const int z = div(10, 0);
    std::cout << z << std::endl;
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl; // 'y should not be zero!'
  }

  /* Remarks */
#if 0
  // This expression does not have name.
  std::cout << NAMEOF(ptrvar[0]) << std::endl; // ''
  std::cout << NAMEOF(42.0) << std::endl; // ''
  std::cout << NAMEOF(42) << std::endl; // ''
  std::cout << NAMEOF(42.0_deg) << std::endl; // ''
  std::cout << NAMEOF((structvar)) << std::endl; // ''
  std::cout << NAMEOF((SomeMethod4<int, float>)(1.0f)) << std::endl; // ''
  std::cout << NAMEOF(42, 42, 42) << std::endl; // ''
  std::cout << NAMEOF(42 + 42) << std::endl; // ''
  std::cout << NAMEOF("Bad case"_string) << std::endl; // ''
  std::cout << NAMEOF("Bad case") << std::endl; // ''
  std::cout << NAMEOF("somevar.somefield") << std::endl; // ''
  std::cout << NAMEOF(42.f) << std::endl; // ''
  std::cout << NAMEOF(structvar.somefield++) << std::endl; // ''
  std::cout << NAMEOF(std::string{"test"}) << std::endl; // ''
#endif

  return 0;
}
