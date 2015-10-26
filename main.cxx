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
    return 0;
}
