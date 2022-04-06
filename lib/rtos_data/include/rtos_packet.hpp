#pragma once

#include <cstdint>
#include <vector>
#include <ostream>
#include <cstring>
#include <type_traits>


namespace rtos{
    /**
     * @brief Header of a packet defined by an ID and containing the current size of the packet 
     * 
     * @tparam T type of ID
     */
    template<typename T>
    struct packet_header{
        T id {};
        uint16_t size = 0;

        /**
         * @brief Assigns the ID of this header
         * 
         * @tparam X type of ID
         * @param arg 
         * @return packet_header<T>& 
         */
        template<typename X>
        packet_header<T>& operator = (const X& arg){
            static_assert(std::is_standard_layout<X>::value, "Template is too complex");

            std::memcpy(&id, &arg, sizeof(X));

            return *this;
        }
    };

    /**
     * @brief Packet structure which encapsulates a header and a payload. Since only simple data will be used, the payload consists of simple type
     * 
     * @tparam T Header type
     * @tparam R Payload type
     */
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
                os << "{ " << data.header.id << ":" <<  data.payload <<" }";

            return os;
        }

        /**
         * @brief Inserts a payload to this packet
         * 
         * @tparam X Standard or scalar data type
         * @param data 
         * @param in 
         * @return packet_data<T, R>& 
         */
        template<typename X>
        friend packet_data<T, R>& operator << (packet_data<T, R>& data,const X& in){
            static_assert(std::is_standard_layout<X>::value, "Template is too complex");
            std::memcpy(
                &data.payload, &in, sizeof(X)
            );

            data.header.size = data.size();

            return data;
        }


    };

}