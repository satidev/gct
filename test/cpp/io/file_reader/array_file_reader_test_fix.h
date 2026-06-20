#ifndef ARRAY_FILE_READER_TEST_FIX_H
#define ARRAY_FILE_READER_TEST_FIX_H

#include <string>
#include "../../../../src/cpp/io/file_utils.h"
#include "../../../../src/cpp/io/metadata.h"
#include "../../../test_data.h"
#include <numeric>

namespace GCT::Test
{
template<typename ElemType, unsigned Dim>
struct ArrayFileReaderTestFix
{
public:
    ArrayFileReaderTestFix(std::string const &file_first,
                           std::string const &file_second);

    [[nodiscard]] bool areFilesFound() const noexcept
    {
        return files_found;
    }

    std::string nameFirstFile() const noexcept
    {
        return filename_first;
    }
    std::vector<ElemType> dataFirstFile() const noexcept
    {
        return data_first_file;
    }
    std::vector<ElemType> dataAllFiles() const noexcept
    {
        return data_all_files;
    }

    bool areEqual(GCT::IO::ArrayMetadata const &md) const noexcept;

    IO::ArrayMetadata metaData() const noexcept
    {
        return md_gt;
    }

protected:
    static std::filesystem::path dirFullPath();

private:
    bool files_found = false;
    std::string filename_first;
    std::string filename_second;
    GCT::IO::ArrayMetadata md_gt;
    std::vector<ElemType> data_first_file;
    std::vector<ElemType> data_all_files;

    static std::string dirName();
    void setMetaData();
    void setDataAllFiles();
    void setDataFirstFile();
};
template<typename ElemType, unsigned Dim>
std::filesystem::path ArrayFileReaderTestFix<ElemType, Dim>::dirFullPath()
{
    return std::filesystem::path{::TEST_FILE_DIR} / std::string{"io"} /
        std::filesystem::path{ArrayFileReaderTestFix<ElemType, Dim>::dirName()};
}

template<typename ElemType, unsigned int Dim>
bool ArrayFileReaderTestFix<ElemType, Dim>::areEqual(
    GCT::IO::ArrayMetadata const &md) const noexcept
{
    return (md.num_rows == md_gt.num_rows) &&
        (md.num_cols == md_gt.num_cols) &&
        (md.num_slices == md_gt.num_slices) &&
        (md.type == md_gt.type);
}

template<typename ElemType, unsigned int Dim>
void ArrayFileReaderTestFix<ElemType, Dim>::setDataFirstFile()
{
    auto const num_data_elems = md_gt.num_rows * md_gt.num_cols *
        md_gt.num_slices;
    data_first_file.assign(num_data_elems, 0);
    std::iota(std::begin(data_first_file), std::end(data_first_file), 0);
}

template<typename ElemType, unsigned Dim>
void ArrayFileReaderTestFix<ElemType, Dim>::setDataAllFiles()
{
    auto const num_files = 2u;
    auto const num_data_elems = md_gt.num_rows * md_gt.num_cols *
        md_gt.num_slices * num_files;
    data_all_files.assign(num_data_elems, 0);
    std::iota(std::begin(data_all_files), std::end(data_all_files), 0);
}

template<typename ElemType, unsigned Dim>
ArrayFileReaderTestFix<ElemType, Dim>::ArrayFileReaderTestFix(std::string const &file_first,
                                                              std::string const &file_second)
    :
    filename_first{dirFullPath() / file_first},
    filename_second{dirFullPath() / file_second}
{
    using namespace GCT::IO;
    files_found = doesFileExist(filename_first) &&
        doesFileExist(filename_second);
    setMetaData();
    setDataAllFiles();
    setDataFirstFile();
}

}//GCT::Test namespaces.
#endif //ARRAY_FILE_READER_TEST_FIX_H


