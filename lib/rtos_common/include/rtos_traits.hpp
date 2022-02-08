#pragma once
#include <any>
#include <type_traits>
#include <tuple>


template<typename Function>
struct callback_type : public callback_type<decltype(&std::remove_reference<Function>::type::operator())>
{};

template<typename ReturnType, typename... Arguments>
struct callback_type<ReturnType (*)(Arguments...)>
{
    template<std::size_t Index>
    using arguments = typename std::tuple_element<Index, std::tuple<Arguments...>>::type;


    static constexpr std::size_t arity = sizeof...(Arguments);
};

template <typename ClassType, typename ReturnType, typename... Arguments>
struct callback_type<ReturnType (ClassType::*)(Arguments...) const> : callback_type<ReturnType (*)(Arguments...)>
{
};

template <typename ClassType, typename ReturnType, typename... Arguments>
struct callback_type<ReturnType (ClassType::*)(Arguments...)> : callback_type<ReturnType (*)(Arguments...)>
{
};