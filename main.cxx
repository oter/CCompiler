#include <iostream>
#include <fstream>
#include "utils/CommentsCleaner.hpp"
#include "utils/FSingleton.hpp"
#include "utils/FSettings.hpp"
#include <iterator>

// Disable warnings when use deprecated features like auto_ptr in boost.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <boost/regex.hpp>

#pragma GCC diagnostic pop

int main()
{
    auto settings = FSingleton<FSettings>::GetInstance();
    settings->LoadDefaultSettings();

    const auto &p = settings->Get<bool>("Pedantic");

    std:: cout << p;


    std::ifstream in_file;
    in_file.open("./../tests/preprocessor/comments.c");
    if (!in_file.is_open())
    {
        std::cout << "Error: Cannot open file comments.c" << std::endl;
        return 0;
    }
    if (in_file.fail())
    {
        std::cout << "Error occured." << std::endl;
        return 0;
    }

    if (!in_file.seekg(0, std::ios::end))
    {
    }

    auto file_length = in_file.tellg();
    in_file.seekg(0, std::ios::beg);

    std::string file_content;
    file_content.reserve(file_length);

    file_content.assign(std::istreambuf_iterator<char>(in_file), std::istreambuf_iterator<char>());

    std::cout << file_content << std::endl;
    in_file.close();

    CommentsCleaner cleaner(file_content);

    std::cout << "#####################" << std::endl;
    std::cout << cleaner.ToString();

    std::ofstream out_file("out.c");
    out_file << cleaner.ToString();

    out_file.close();

    return 0;
}