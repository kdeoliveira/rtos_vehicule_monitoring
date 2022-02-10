#pragma once

#include <stdio.h>
#include <fstream>
#include <cstring>
#include <memory>
#include <utility>
#include <deque>
#include <sys/stat.h>
#include "rtos_common.hpp"


namespace rtos
{
    class InputFile
    {
    public:
        InputFile() = default;
        InputFile(const char *filename)
        {
            this->file_stream = fopen(
                filename, "rb+"
            );

            this->_line_index = 0;
            this->line_stream = new BYTE[BUFFER_SIZE];
            
            this->position = 0L;

            this->buffer_stream = new char[BUFFER_SIZE];

            setvbuf(
                this->file_stream, this->buffer_stream, _IOLBF, sizeof(char) * BUFFER_SIZE
            );

            this->fd = fileno(this->file_stream);

            struct stat st;


            fstat(this->fd, &st);

            this->file_size = st.st_size;

        }

        void open(const char* filename){
            try{
                this->file_stream = fopen(
                    filename, "rb+"
                );

            }catch(std::exception& e){
                puts(e.what());
            }

            
        }

        ~InputFile()
        {
            fflush(this->file_stream);

            if (this->file_stream != nullptr) {
                fclose(this->file_stream);
            }
            delete[] this->buffer_stream;

            delete[] this->line_stream;
            
        }

        BYTE* read_line()
        {
            if( fgets(this->line_stream, BUFFER_SIZE, this->file_stream) == nullptr ){
                if(ferror(this->file_stream)) throw "Error reading the file";
                return this->line_stream;
            }

            this->_line_index++;
            position = ftell(this->file_stream);

            return this->line_stream;
        }

        // std::pair<char*, float>* get_values(){
        //     return this->values.get();
        // }

        constexpr int line_index() const{
            return this->_line_index;
        }

        constexpr size_t get_file_size() const{
            return this->file_size;
        }

        long int get_position(){
            return this->position;
        }

        // char * begin(InputFile &x){
        //     return x.size() ? x.values[0] : nullptr;
        // }
        // std::pair<char *, float>* begin(){
        //     return this->size()  ? *this->values.get() : nullptr;
        // }
        // std::pair<char *, float>* end(){
        //     return this->values.get()[0] + this->size();
        // }

        // char *end(InputFile &x){
        //     return x.values[0] + x.size();
        // }

        int get_fd(){
            return this->fd;
        }

        bool is_eof(){
            return this->file_size <= this->position;
        }


    private:
        BYTE* line_stream;
        FILE* file_stream;
        int _line_index;
        char* buffer_stream;
        long int position;
        int fd;
        size_t file_size;
        
    };
}