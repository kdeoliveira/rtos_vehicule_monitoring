#pragma once

#include <cstdint>
#include <vector>
#include <ostream>
#include <cstring>
#include <type_traits>


namespace rtos{
    template<typename T>
    struct packet_header{
        T id {};
        uint16_t size = 0;

        template<typename X>
        packet_header<T>& operator = (const X& arg){
            static_assert(std::is_standard_layout<X>::value, "Template is too complex");

            std::memcpy(&id, &arg, sizeof(X));

            return *this;
        }
    };

    template<typename T, typename R>
    struct packet_data{
        packet_header<T> header;
        R payload;

        constexpr size_t size() const{
            return sizeof(R);
        }


        template<typename X>
        packet_data<T, R>& operator = (const X& arg){
            static_assert(std::is_same<X, T>::value, "Incorrect template");
            std::memcpy(&header.id, &arg, sizeof(X));
            header.size = sizeof(X);

            return *this;
        }

        friend std::ostream& operator << (std::ostream& os, const packet_data<T, R>& data){
            // if(std::is_union<std::remove_const<decltype(data.payload)>>::value){
                
            //     return os;
            //     // os << "id: " << data.header.id << " | " << "payload: " << static_cast<typeid(arg)>(data.payload);
            // }else{

            //     os << "id: " << data.header.id << " | " << "payload: " << data.payload;
            // }

                os << "{ " << data.header.id << ":" <<  data.payload <<" }";

            return os;
        }

        template<typename X>
        friend packet_data<T, R>& operator << (packet_data<T, R>& data,const X& in){
            static_assert(std::is_standard_layout<X>::value, "Template is too complex");

            // size_t sz = data.payload.size();
            // data.payload.resize(data.payload.size() + sizeof(X));

            std::memcpy(
                &data.payload, &in, sizeof(X)
            );

            data.header.size = data.size();

            return data;
        }


    };

}