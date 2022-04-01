#pragma once

#include <stdio.h>
#include <fstream>
#include <cstring>
#include <memory>
#include <utility>
#include <deque>
#include <sys/stat.h>
#include <rtos_common.hpp>


namespace rtos
{
    /**
     * @brief Input stream for files. Alternative C implementation of ifstream provided by the std library
     * 
     */
    class InputFile
    {
    public:
        InputFile() = default;
        /**
         * @brief Construct a new Input File object
         * 
         * @param filename absolute path of file
         */
        InputFile(const char *filename)
        {
            if (
                ( this->file_stream = fopen(filename, "rb+") ) == NULL
            ){
                throw "Unable to open file";
            }


            this->_line_index = 0;
            this->line_stream = new BYTE[BUFFER_SIZE];
            
            this->position = 0L;

            this->buffer_stream = new char[BUFFER_SIZE];

            if(this->file_stream != NULL)
                setvbuf(
                    this->file_stream, this->buffer_stream, _IOLBF, sizeof(char) * BUFFER_SIZE
                );

            this->fd = fileno(this->file_stream);

            struct stat st;


            fstat(this->fd, &st);

            this->file_size = st.st_size;

        }

        InputFile(const InputFile&) = delete;
        InputFile& operator=(const InputFile&) = delete;

        /**
         * @brief Opens file in read mode
         * 
         * @param filename 
         */
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

        /**
         * @brief Reads file line by line and stores inside a temporary buffer
         * Note that maximum size of read bytes is defined by BUFFER_SIZE
         * @return BYTE* char* equivalent value of the buffer.
         */
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

        /**
         * @brief Get current line number
         * 
         * @return const int 
         */
        const int line_index() const{
            return this->_line_index;
        }

        /**
         * @brief Get the file size object
         * 
         * @return const size_t 
         */
        const size_t get_file_size() const{
            return this->file_size;
        }

        /**
         * @brief Get the current position of the stream
         * 
         * @return long int 
         */
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

        /**
         * @brief Get a file descriptor for the stream
         * 
         * @return int 
         */
        int get_fd(){
            return this->fd;
        }

        /**
         * @brief Returns if stream has reached END OF FILE
         * 
         * @return bool  
         */
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