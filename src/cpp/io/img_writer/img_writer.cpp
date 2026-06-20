#include "img_writer.h"

namespace GCT::IO
{
template<>
ArrayFileWriter::DenMetaData denMetaData(Geom::ReconImgGeom<2u> const &geom)
{
    auto metadata = ArrayFileWriter::DenMetaData{};
    metadata.num_rows = geom.y.num_samples;
    metadata.num_cols = geom.x.num_samples;
    metadata.num_slices = 1u;

    return metadata;
}

template<>
ArrayFileWriter::DenMetaData denMetaData(Geom::ReconImgGeom<3u> const &geom)
{
    auto metadata = ArrayFileWriter::DenMetaData{};
    metadata.num_rows = geom.y.num_samples;
    metadata.num_cols = geom.x.num_samples;
    metadata.num_slices = geom.z.num_samples;

    return metadata;
}

template<>
ArrayFileWriter::MetaImgMetaData<2u> metaImgMetaData(Geom::ReconImgGeom<2u> const &geom)
{
    auto metadata = ArrayFileWriter::MetaImgMetaData<2u>{};
    metadata.dim_size = std::array<unsigned, 2u>{geom.x.num_samples,
                                                 geom.y.num_samples};

    metadata.spacing = std::array<float, 2u>{geom.x.sampling_width.val,
                                             geom.y.sampling_width.val};

    auto const first_pix_pos = GCT::Geom::firstVoxPos(geom);
    metadata.offset = std::array<float, 2u>{first_pix_pos.x.val,
                                            first_pix_pos.y.val};

    return metadata;
}

template<>
ArrayFileWriter::MetaImgMetaData<3u> metaImgMetaData(Geom::ReconImgGeom<3u> const &geom)
{
    auto metadata = ArrayFileWriter::MetaImgMetaData<3u>{};
    metadata.dim_size = std::array<unsigned, 3u>{geom.x.num_samples,
                                                 geom.y.num_samples,
                                                 geom.z.num_samples};

    metadata.spacing = std::array<float, 3u>{geom.x.sampling_width.val,
                                             geom.y.sampling_width.val,
                                             geom.z.sampling_width.val};

    auto const first_pix_pos = GCT::Geom::firstVoxPos(geom);
    metadata.offset = std::array<float, 3u>{first_pix_pos.x.val,
                                            first_pix_pos.y.val,
                                            first_pix_pos.z.val};

    return metadata;
}

}//GCT::IO namespace.
