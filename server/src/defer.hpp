#ifndef __DEFER_HPP__
#define __DEFER_HPP__

// Golang-like defer keyword implementation for modern C++.
// Based on the RAII and perfect-forwarding.
//
// 2023 Simon Ryabinkov <smrbkv@proton.me>
//
// Documentation:
//  Functions:
//    void defered(std::function<void()> function)
//      >> execute lambda on scope exit
//      >>
//      >> defered([] {puts("swag");})
//
//  Macroses:
//    defer (lambda body)
//      >> execute lambda body on scope exit
//      >> use carefully
//      >>
//      >> defer (
//      >>  puts("swag")
//      >> )

#include <utility>

namespace Defer {
  inline namespace DeferImplv1 {
    template <class Function> class DeferedObject {
      public:
        template <class Object> DeferedObject(
            Object&& f
        ) : DeferedFunction(
              std::forward<Object>(f)
            )
        {}

        ~DeferedObject() {
          DeferedFunction();
        }

      private:
        Function DeferedFunction;
    };
  }

  template <class Object> auto ReturnDeferedObject(
    Object&& Function
  ) -> DeferedObject<Object> {
    return {
      std::forward<Object>(Function)
    };
  }

  #define __MACRO_REAL_JOIN(x, y) x##y
  #define __MACRO_JOIN(x, y) __MACRO_REAL_JOIN(x, y)
  #define __DEFER_UNIQUE_VAR(x) __MACRO_JOIN(x, __LINE__)

  #define defered(Lambda)                             \
    [[maybe_unused]] const auto&                      \
      __DEFER_UNIQUE_VAR(DeferedObject) =             \
        Defer::ReturnDeferedObject(Lambda);

  #define defer(FunctionBody) defered([&]() {FunctionBody;})

} /* namespace Defer */

#endif /* __DEFER_HPP__ */
