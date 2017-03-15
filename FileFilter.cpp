/**=================================================================
 * GTB Technologies Proprietary
 * Copyright 2017 GTB Technologies, Inc.
 * UNPUBLISHED WORK
 * This software is the confidential and proprietary information of
 * GTB Technologies, Inc. ("Proprietary Information"). Any use,
 * reproduction, distribution or disclosure of the software or
 * Proprietary Information, in whole or in part, must comply with
 * the terms of the license agreement, nondisclosure agreement
 * or contract entered into with GTB Technologies, Inc. providing
 * access to this software.
 * @author Andrii Kozlovskyi <andy.kozlovskyi@gmail.com>
 *==================================================================
 */


#include "FileFilter.h"

#include <iostream>

#include <boost/algorithm/string.hpp>

FileInfo::FileInfo(const filesystem::path& path):
        _name(path.filename().string()),
        _extension(path.extension().string()),
        _size(boost::filesystem::file_size(path))
{
    std::cout << "FileInfo::_extension = " << _extension << std::endl;
}

//
//std::unique_ptr<FileFilter> ExtensionsFilter::create(const std::vector<std::string>& extensions)
//{
//    return  std::unique_ptr<ExtensionsFilter>(new ExtensionsFilter(extensions));
//}

bool ExtensionsFilter::check(const FileInfo &fileInfo)
{
    for (auto x: _extensions)
    {
        if(boost::iends_with(fileInfo.getExtension(), x))
        {
            std::cout << "Extension matching = true: "<< fileInfo.getFileName() << std::endl;
            return true;
        }
    }
    std::cout << "Extension matching - false: "<< fileInfo.getFileName() << std::endl;
    return false;
}


std::unique_ptr<Condition> SizeFilter::createLessThan(size_t size)
{
    return std::unique_ptr<Condition>(new SizeFilter(size, [=](const size_t a, const size_t b){return a > b;}));
}

std::unique_ptr<Condition> SizeFilter::createGreaterThan(size_t size)
{
    return std::unique_ptr<Condition>(new SizeFilter(size, [=](const size_t a, const size_t b)->bool{return a < b;}));
}

std::unique_ptr<Condition> SizeFilter::createRangeFilter(size_t sizeMin, size_t sizeMax)
{
    std::unique_ptr<And> cond(new And());
    cond->append(createGreaterThan(sizeMin));
    cond->append(createLessThan(sizeMax));
    return std::unique_ptr<Condition>(std::move(cond));
}

bool SizeFilter::check(const FileInfo& fileInfo)
{
    bool result = _func(_size, fileInfo.getSize());
    std::cout << "Size matching: "<< fileInfo.getFileName() <<  (result?" true": " false") << std::endl;
    return result;
}


