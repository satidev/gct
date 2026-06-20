#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <filesystem>
#include <vector>

namespace GCT::IO
{
inline bool doesFileExist(std::string const &filename)
{
    return std::filesystem::exists(std::filesystem::path{filename});
}
inline size_t fileSize(std::string const &filename)
{
    return file_size(std::filesystem::path{filename});
}

//Get all the files in a directory with the same extension and size of the input file.
std::vector<std::string> getAllFiles(std::string const &filename);

inline bool isWritable(std::string const &dir_name)
{
    namespace fs = std::filesystem;
    auto const dir = fs::path{dir_name};
    auto const status = fs::status(dir);
    return fs::exists(status) &&
        fs::is_directory(status) &&
        ((status.permissions() & fs::perms::owner_write) != fs::perms::none);
}






}//GCT::IO namespaces.

#endif //FILE_UTILS_H


