#include <iostream>
#include <thread>

#include <boost/filesystem.hpp>
#include <chrono>
#include "boost/filesystem/operations.hpp"

#include <memory>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include "FileFilter.h"

using namespace std;


std::shared_ptr<FileFilter> extFile;
// Use std=C++11 for build test app

void ScanDirectory(const std::string& directory)
{
    for(boost::filesystem::recursive_directory_iterator dir_end, dir(directory); dir != dir_end; ++dir) {
        try {
            boost::filesystem::path _path(*dir);
            if (!boost::filesystem::is_directory(_path)) {
                std::cout << "SCAN: " << _path.string() << std::endl;
//                std::cout << "Basename: " << _path.filename()  << std::endl;
                std::cout << "Extension: " << boost::filesystem::extension(_path)  << std::endl;
//                std::cout << "Size: " <<   boost::filesystem::file_size(_path) << std::endl;
//                std::time_t tmp = boost::filesystem::last_write_time(_path);
//                std::cout << "Write time: " <<   tmp << std::endl;
//
//                std::chrono::time_point tp = std::chrono::system_clock::from_time_t(tmp);
//                std::time_t t1 = std::chrono::system_clock::to_time_t(tp);
//                std::cout << "Date: " << std::put_time(std::localtime(&t1), "%F %T") << '\n';


                boost::filesystem::file_status status =  boost::filesystem::status(_path);
                std::cout << "permissions: " << status.permissions() << std::endl;


                std::cout << "rejected: " << extFile->accept(FileInfo(_path)) << std::endl;
                std::cout << "===========================================================" << std::endl;
            }
        } catch (boost::filesystem::filesystem_error e) {
            std::cout << e.what() << std::endl;
        }
    }
}



int main()
{

    extFile = make_shared<FileFilter>();

    std::vector<std::string> ext = {".txt"};
    std::vector<std::string> ext1 = {".pdf"};
    std::vector<std::string> ext2 = {".djvu"};
    std::unique_ptr<Condition> pdf (new ExtensionsFilter(ext1));
    std::unique_ptr<Condition> djvu (new ExtensionsFilter(ext2));


    std::unique_ptr<And> range(new And());
    range->append(SizeFilter::createRangeFilter(50000000, 100000000));
    range->append(std::move(pdf));

    extFile->append(std::move(range));

    extFile->append(std::move(djvu));

    ScanDirectory ("/home/akozlovskyi/Ebook/SQL");

    return 0;
}