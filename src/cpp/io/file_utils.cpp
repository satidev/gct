#include "file_utils.h"
#include <iostream>
#include <algorithm>

namespace GCT::IO
{
std::vector<std::string> getAllFiles(std::string const &filename)
{
    if (!doesFileExist(filename)) {
        throw std::runtime_error{"File " + filename + " is not found.\n"};
    }

    auto const file_path = std::filesystem::path{filename};
    auto const dir_path = file_path.parent_path();
    auto const file_ext = file_path.extension();
    auto const file_size = fileSize(filename);

    std::vector<std::string> files;
    for (auto const &file: std::filesystem::recursive_directory_iterator(dir_path)) {
        if ((file.path().extension() == file_ext) &&
            (file_size == fileSize(file.path()))) {
            files.emplace_back(file.path());
        }
    }
    std::sort(std::begin(files), std::end(files));
    return files;
}

}//GCT::IO namespaces.
