#ifndef UTILS_H
#define UTILS_H

#define UNUSED(x) (void)(x)

/**
 * @brief A utility structure for enabling function overloading with template-based classes.
 * see : https://stackoverflow.com/a/64018031
 */
template<class... Ts>
struct overload : Ts... {
    using Ts::operator()...;
};
/**
 * @brief Deduction guide for the `overload` template.
 * see : https://stackoverflow.com/a/64018031
*/
template<class... Ts>
overload(Ts...) -> overload<Ts...>;

#endif // UTILS_H
