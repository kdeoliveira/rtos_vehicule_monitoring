#pragma once
#include <type_traits>
#include <any>
#include <memory>
#include "rtos_traits.hpp"
#include <iostream>
struct Base{
        virtual bool operator()(const std::type_info &type) const = 0;
        virtual void operator()(const std::any &data) = 0;
        virtual ~Base(){}
};

template <typename T>
struct Wrapper : public Base {
    T wp;
    using argument = typename callback_type<T>::template arguments<0>;

    Wrapper(const T &t) : wp(t){};

    bool operator()(const std::type_info& type) const override{
        return typeid(argument) == type;
    }

    void operator()(const std::any& data) override{
        wp(std::any_cast<argument>(data));
    }
};

struct Callback{
    
    std::shared_ptr<Base> fn;

    template<typename T>
    Callback(T &&obj) : fn(
                                   std::make_shared<Wrapper<typename std::remove_reference<T>::type>>(std::forward<T>(obj)))
    {
    }


    bool operator()(const std::type_info &type) const {
        return (*fn)(type);
    }

    void operator()(const std::any &data){
        return (*fn)(data);
    }

};