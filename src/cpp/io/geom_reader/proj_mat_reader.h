#ifndef PROJ_MAT_READER_H
#define PROJ_MAT_READER_H

#include "../../geom/projection_matrix.h"
#include <string>

namespace GCT::IO
{
template<typename FileReader>
struct ProjMatReader
{
    static std::vector<Geom::ProjectionMatrix> read(std::string const &filename,
                                                    bool series = false);
};

template<typename FileReader>
std::vector<Geom::ProjectionMatrix> ProjMatReader<FileReader>::read(const std::string &filename,
                                                                    bool series)
{
    auto const reader = FileReader{filename, series};
    auto const metadata = reader.readMetadata();
    auto const mat_elems_all = reader.template readData<double>();

    // Ensure that data corresponds to projection matrix.
    auto constexpr num_elems_mat = 12u;
    auto const num_mats = metadata.num_slices;
    auto const num_elements = static_cast<size_t>(num_elems_mat * num_mats);
    if (mat_elems_all.size() != num_elements) {
        throw std::runtime_error{
            "Error during the reading of projection matrix file " + filename + ".\n" +
                std::string{"There should be 12 elements in each projection matrix.\n"}};
    }

    auto mat_elems = std::array<double, num_elems_mat>{};
    auto pmats = std::vector<Geom::ProjectionMatrix>{};
    pmats.reserve(num_mats);

    for (auto mat_idx = 0u; mat_idx < num_mats; mat_idx++) {
        std::copy_n(std::begin(mat_elems_all) + mat_idx * num_elems_mat,
                    num_elems_mat, std::begin(mat_elems));
        pmats.emplace_back(mat_elems);
    }

    return pmats;
}

}//GCT::IO namespace.

#endif //PROJ_MAT_READER_H


