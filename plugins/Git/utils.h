#ifndef UTILS_H
#define UTILS_H

template<class... Ts>
struct overload : Ts... {
    using Ts::operator()...;
};
template<class... Ts>
overload(Ts...) -> overload<Ts...>;

#endif // UTILS_H
