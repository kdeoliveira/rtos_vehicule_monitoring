#pragma once
#include <cstdio>
#include "rtos_packet.hpp"
#include "rtos_file.hpp"

#include <memory>
#include <array>

// namespace rtos{

//     template<typename T>
//     class Parser{
//         public:
//             Parser(std::size_t sz) : data{new T[sz]}, sz{sz}{
//                 this->count = 0;
//             }

//             T* parse_line(char * line){
//                 if(this->data != nullptr){
//                     delete this->data;
//                     this->data = new T[this->sz];
//                 }
//             this->data = strtok(line, ",");
//             while(this->data != nullptr){
//                 if(puts(this->data) == EOF){
//                     break;
//                 }
//                 this->data++;

                
//                 this->data = strtok(nullptr, ",");
//             }

//             return this->data;
//             }

//         private:
//             T* data;
//             size_t sz;
//             short int count;
//     };
// } 