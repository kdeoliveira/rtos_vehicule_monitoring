// #include "rtos_file.hpp"

// namespace rtos
// {
//     InputFile::InputFile(const char* filename){
//         this->file_stream = fopen(
//             filename, "r"
//         );

//         if(this->file_stream == nullptr)    throw "Unable to open file";


//         this->fd = fileno(this->file_stream);
//     }


//     char* InputFile::read(){
//         char** line;
//         char * temp;


//         fgets(temp, 1024, this->file_stream);

//         *line = strtok(temp, ",");
        
//         return *line;


//     }



//     InputFile::~InputFile(){
//         fclose(this->file_stream);
//     }
// }
