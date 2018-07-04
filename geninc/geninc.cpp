#include <vector>
#include <fstream>
#include <iostream>
#include <functional>
#include <exception>

template <class T>
void chunkproc(std::vector<T> &array, size_t chunkSize, std::function<void(std::vector<T>&)> task)
{
    if (chunkSize == 0 || !task) throw std::runtime_error("Incorrect chunk size or no task provided to process array");
    if (chunkSize < array.size())
    {
        size_t count = array.size() / chunkSize, remainder = array.size() % chunkSize;
        for (size_t i = 0; i < count; i++)
        {
            size_t offset = chunkSize * i;
            std::vector<T> temp(array.begin() + offset, array.begin() + offset + chunkSize);
            task(temp);
        }
        size_t offset = chunkSize * count;
        std::vector<T> temp(array.begin() + offset, array.begin() + offset + remainder);
        task(temp);
    }
    else
    {
        task(array);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("%s <file>", argv[0]);
        return 1;
    }
  
    std::ifstream file(argv[1]);
    std::vector<unsigned char> fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    chunkproc<unsigned char>(fileContents, 30, [] (std::vector<unsigned char>& chunk) {
        for (auto c : chunk)
        {
           printf("0x%02x,", c);
        }
        printf("\n");
    });
    
    return 0;
}
