#include "catch.hpp"
#include "../../../../src/cpp/io/file_writer/meta_img_file_writer.h"
#include "../../../../src/cpp/io/file_reader/meta_img_reader.h"
#include "../file_reader/test_filenames.h"
#include "../file_reader/array_file_reader_test_fix.h"

namespace GCT::Test
{
TEST_CASE("Exception is thrown due to the invalid path of meta image file",
          "[io][meta_img_file_writer]")
{
    auto const data = std::vector<float>{1.0f};
    auto const fname = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"invalid_dir"} / std::string{"raw.mha"};
    auto constexpr img_dim = size_t{2UL};
    auto meta_data = IO::ArrayFileWriter::MetaImgMetaData<img_dim>{};
    meta_data.dim_size[0] = 1u;
    meta_data.dim_size[1] = 1u;
    REQUIRE_THROWS_AS(IO::ArrayFileWriter::write(meta_data, data, fname), std::runtime_error);
}

TEST_CASE("Exception is thrown due to the mismatch between data and metadata of meta image file",
          "[io][meta_img_file_writer]")
{
    auto const data = std::vector<float>{1.0f};
    auto const filename = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"raw.mha"};
    auto constexpr img_dim = size_t{2UL};
    auto meta_data = IO::ArrayFileWriter::MetaImgMetaData<img_dim>{};
    REQUIRE_THROWS_AS(IO::ArrayFileWriter::write(meta_data, data, filename), std::runtime_error);
}


TEMPLATE_TEST_CASE_SIG("Meta image file (MHA) is written correctly",
                       "[io][meta_img_file_writer]",
                       ((typename ElemType, unsigned Dim), ElemType, Dim),
                       (float, 3u))
{
    auto const &[first_file, first_sec] = testFileNames<IO::MetaImgFileReader>();
    auto const fix = ArrayFileReaderTestFix<ElemType, Dim>{first_file, first_sec};
    auto const data = fix.dataFirstFile();
    auto const metadata_arr = fix.metaData();
    auto metadata_mh = IO::ArrayFileWriter::MetaImgMetaData<Dim>{};
    metadata_mh.dim_size[0] = metadata_arr.num_cols;
    metadata_mh.dim_size[1] = metadata_arr.num_rows;
    metadata_mh.dim_size[2] = metadata_arr.num_slices;

    auto const fname = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"temp.mha"};

    REQUIRE_NOTHROW(IO::ArrayFileWriter::write(metadata_mh, data, fname));

    //Read the written file and verify the data.
    auto const reader = IO::MetaImgFileReader{fname};
    REQUIRE(data == reader.readData<ElemType>());

    std::filesystem::remove(fname);
}

TEMPLATE_TEST_CASE_SIG("Meta image file (MHD) is written correctly",
                       "[io][meta_img_file_writer]",
                       ((typename ElemType, unsigned Dim), ElemType, Dim),
                       (float, 3u))
{
    auto const fix = ArrayFileReaderTestFix<ElemType, Dim>{"01.mhd", "02.mhd"};
    auto const data = fix.dataFirstFile();
    auto const metadata_arr = fix.metaData();
    auto metadata_mh = IO::ArrayFileWriter::MetaImgMetaData<Dim>{};
    metadata_mh.dim_size[0] = metadata_arr.num_cols;
    metadata_mh.dim_size[1] = metadata_arr.num_rows;
    metadata_mh.dim_size[2] = metadata_arr.num_slices;

    auto const fname = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"temp.mhd"};

    REQUIRE_NOTHROW(IO::ArrayFileWriter::write(metadata_mh, data, fname));

    //Read the written file and verify the data.
    auto const reader = IO::MetaImgFileReader{fname};
    REQUIRE(data == reader.readData<ElemType>());

    std::filesystem::remove(fname);
    auto const raw_fname = std::filesystem::path{fname}.parent_path()/
        std::string{"temp.raw"};
    std::filesystem::remove(raw_fname);
}

TEST_CASE("Returns MET_UCHAR", "[io][meta_img_file_writer]")
{
    REQUIRE(IO::ArrayFileWriter::Detail::MetaImage::typeStr<std::uint8_t>() == "MET_UCHAR");
}

TEST_CASE("Returns MET_CHAR", "[io][meta_img_file_writer]")
{
    REQUIRE(IO::ArrayFileWriter::Detail::MetaImage::typeStr<std::int8_t>() == "MET_CHAR");
}

TEST_CASE("Returns MET_USHORT", "[io][meta_img_file_writer]")
{
    REQUIRE(IO::ArrayFileWriter::Detail::MetaImage::typeStr<std::uint16_t>() == "MET_USHORT");
}

TEST_CASE("Returns MET_SHORT", "[io][meta_img_file_writer]")
{
    REQUIRE(IO::ArrayFileWriter::Detail::MetaImage::typeStr<std::int16_t>() == "MET_SHORT");
}

TEST_CASE("Returns MET_UINT", "[io][meta_img_file_writer]")
{
    REQUIRE(IO::ArrayFileWriter::Detail::MetaImage::typeStr<std::uint32_t>() == "MET_UINT");
}

TEST_CASE("Returns MET_INT", "[io][meta_img_file_writer]")
{
    REQUIRE(IO::ArrayFileWriter::Detail::MetaImage::typeStr<std::int32_t>() == "MET_INT");
}

TEST_CASE("Returns MET_ULONG_LONG", "[io][meta_img_file_writer]")
{
    REQUIRE(IO::ArrayFileWriter::Detail::MetaImage::typeStr<std::uint64_t>() == "MET_ULONG_LONG");
}

TEST_CASE("Returns MET_LONG_LONG", "[io][meta_img_file_writer]")
{
    REQUIRE(IO::ArrayFileWriter::Detail::MetaImage::typeStr<std::int64_t>() == "MET_LONG_LONG");
}

TEST_CASE("Returns MET_FLOAT", "[io][meta_img_file_writer]")
{
    REQUIRE(IO::ArrayFileWriter::Detail::MetaImage::typeStr<float>() == "MET_FLOAT");
}

TEST_CASE("Returns MET_DOUBLE", "[io][meta_img_file_writer]")
{
    REQUIRE(IO::ArrayFileWriter::Detail::MetaImage::typeStr<double>() == "MET_DOUBLE");
}

TEST_CASE("Number of elements is 24", "[io][meta_img_file_writer]")
{
    using namespace IO::ArrayFileWriter::Detail::MetaImage;
    REQUIRE(numDataElems(std::array<unsigned, 3UL>{2u, 3u, 4u}) == 24UL);
}

}//GCT::Test namespaces.
