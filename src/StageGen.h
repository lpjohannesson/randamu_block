#ifndef __STAGEGEN_H__
#define __STAGEGEN_H__

#include <functional>
#include <map>
#include <ranges>
#include <string>
#include <type_traits>
#include <variant>

struct GenerationContext;

using GenerationMetadata = std::variant<float, std::string, std::vector<float>, std::vector<std::string>>;
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

template <typename T>
concept Vector = requires {
    requires std::same_as<std::decay_t<T>,
                          std::vector<typename std::decay_t<T>::value_type> >; 
};

}  // namespace detail

using PossibleGenerationMetadata = std::variant<int, float, bool, std::string, std::vector<int>, std::vector<float>, std::vector<std::string>>;


template <class TAlternative>
concept IsGenMetadata = requires(PossibleGenerationMetadata var) {
  requires detail::is_alternative_v<TAlternative,
                                    decltype(detail::to_variant(var))>;
};

template <typename T>
inline T GetMetadata(GenerationMetadata value){
  return std::get<T>(value);
}

template<>
inline int GetMetadata<int>(GenerationMetadata value){
  return std::get<float>(value);
}

template<>
inline bool GetMetadata<bool>(GenerationMetadata value){
  return std::get<float>(value);
}

template<>
inline std::vector<int> GetMetadata<std::vector<int>>(GenerationMetadata value){
  return std::ranges::to<std::vector<int>>(std::get<std::vector<float>>(value) | std::views::transform([](float f)->int{return f;}));
}
#endif // __STAGEGEN_H__