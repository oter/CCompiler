//
// Created by Maxym on 10/26/2015.
//

#ifndef CCOMPILER_FILESRESOLVER_HPP
#define CCOMPILER_FILESRESOLVER_HPP

#include <string>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

template<typename Path = fs::path>
class FilesResolver
{
public:
    FilesResolver() = default;

    bool AddIncludePath(const Path &path, const Path& base = fs::current_path())
    {
        if (!boost::filesystem::exists(path))
        {
            // Path is not exists
            return false;
        }
//        boost::filesystem::absolute()
//        include_paths_.push_back(fs::absolute())


    }

private:
    std::vector<Path> include_paths_;
};


#endif //CCOMPILER_FILESRESOLVER_HPP
