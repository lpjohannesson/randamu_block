#ifndef __STAGEGEN_H__
#define __STAGEGEN_H__

#include <functional>
#include <map>
#include <string>
#include <type_traits>
#include <variant>

struct GenerationContext;

using GenerationMetadata = std::variant<int, float, bool, std::string, std::vector<int>, std::vector<float>, std::vector<std::string>>;
using GenerationArguments = std::map<std::string, GenerationMetadata>;
using GenerationFunction = std::function<void(GenerationContext&, GenerationArguments&)>;

namespace detail {

template <class... Ts>
std::variant<Ts...> to_variant(const std::variant<Ts...> &);

template <class TAlternative, class TVariant>
inline constexpr bool is_alternative_v = false;

template <class TAlternative, class... Ts>
inline constexpr bool is_alternative_v<TAlternative, std::variant<Ts...>> =
    (... or std::is_same_v<TAlternative, Ts>);

}  // namespace detail

template <class TAlternative>
concept IsGenMetadata = requires(GenerationMetadata var) {
  requires detail::is_alternative_v<TAlternative,
                                    decltype(detail::to_variant(var))>;
};

#endif // __STAGEGEN_H__