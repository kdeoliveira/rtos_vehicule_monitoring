#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <any>
#include <vector>
#include <fcntl.h>
#include <rtos_common.hpp>

namespace rtos
{
    enum class PipeMode
    {
        READ_WRITE,
        READ,
        WRITE
    };

    enum class PipeFlag{
        REDIRECT,
        CREATE
    };

    class PipeManager
    {

    public:
        PipeManager(const PipeMode &_mode = PipeMode::READ_WRITE) : mode{_mode}
        {

            if (pipe(this->fd) == -1)
            {
                perror("pipe");
                throw "Unable to create pipe";
            }
            this->flags = PipeFlag::CREATE;
            this->fd_stream = nullptr;
        }

        PipeManager(int _fd[2], const PipeMode &_mode, const PipeFlag &_flags) : mode{_mode} , flags{_flags}
        {
            if(_mode == PipeMode::READ_WRITE){
                throw "READ_WRITE pipe not allowed in this mode";
            }
            if (_mode == PipeMode::READ)
            {
                // dup2(_fd, this->fd[0]);
                close(_fd[1]);
                this->fd[0] = _fd[0];
                if(_flags == PipeFlag::REDIRECT)
                    this->fd_stream = fdopen(fd[0], "r");
            }
            else if (this->mode == PipeMode::WRITE)
            {
                // dup2(_fd, this->fd[1]);
                close(_fd[0]);
                this->fd[1] = _fd[1];

                if(_flags == PipeFlag::REDIRECT)
                    this->fd_stream = fdopen(fd[1], "w");

            }
            // close(_fd);
        }

        PipeManager() = delete;
        PipeManager(const PipeManager &) = delete;
        ~PipeManager()
        {
            puts("Closing pipe");
            this->callbacks.clear();
            close(this->fd[0]);
            close(this->fd[1]);
        }

        void close_fd(PipeMode mode)
        {
            if (mode == PipeMode::READ)
                close(this->fd[0]);
            if (mode == PipeMode::WRITE)
                close(this->fd[1]);
        }

        template <typename T>
        void onRead(T callback)
        {
            static_assert(callback_type<T>::arity == 1, "Only 1 argument per callback allowed");
            callbacks.push_back(callback);
        }

        template <typename T>
        int read_pipe()
        {
            // close(this->fd[1]);
            if (this->mode == PipeMode::WRITE)
                return -1;



            if(this->flags == PipeFlag::CREATE){
                static T buffer;
                 while( read(fd[0], &buffer, sizeof(T) )  > 0 ){
                    for(Callback& x : this->callbacks){
                            if(x(typeid(T))){
                                x(buffer);
                            }
                    }
                }

            }else{
                char line[BUFFER_SIZE];
                setvbuf(this->fd_stream, this->interal_buffer, _IOLBF, BUFFER_SIZE);
                static void* p;
                while ((p = fgets(line, BUFFER_SIZE, this->fd_stream)) != nullptr)
                {
                    for (Callback &x : this->callbacks)
                    {
                        if (x(typeid(T))){
                            x(static_cast<T>(p));
                        }
                    }
                }

            }

            return 0;
        }

        template <typename T>
        int write_pipe(const T &buffer)
        {
            // close(this->fd[0]);
            if (this->mode == PipeMode::READ) return -1;

            if(this->flags == PipeFlag::CREATE){
                int status = write(this->fd[1],&buffer, sizeof(T));
                return status;

            }else{
                setvbuf(this->fd_stream, this->interal_buffer, _IOLBF, 1536);
                fputs(buffer, this->fd_stream);
            }

            return 0;
        }

        int *get_file_descriptor()
        {
            return this->fd;
        }

    private:
        int fd[2];
        PipeMode mode;
        PipeFlag flags;
        FILE* fd_stream;
        std::vector<Callback> callbacks;
        char interal_buffer[BUFFER_SIZE];
    };
}