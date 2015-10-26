//
// Created by Maxym on 10/25/2015.
//

#ifndef CCOMPILER_PREPROCESSEDSLICE_HPP
#define CCOMPILER_PREPROCESSEDSLICE_HPP

#include <memory>
#include <vector>
#include <boost/noncopyable.hpp>
#include "PreprocessorDefine.hpp"


class PreprocessedSlice : private boost::noncopyable
{
public:

private:
    std::string file_name_;
    std::vector<PreprocessorDefine> defines_;
    std::shared_ptr<PreprocessedSlice> slices_;

};


#endif //CCOMPILER_PREPROCESSEDSLICE_HPP
