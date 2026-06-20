#include "projection_reader.h"

namespace GCT::IO
{
template<>
Data::ProjMetaInfo<1u> projMetaInfo(ArrayMetadata const &metadata)
{
    return Data::ProjMetaInfo<1u>{Data::ProjSize<1u>{metadata.num_cols},
                                  metadata.num_rows * metadata.num_slices};
}

template<>
Data::ProjMetaInfo<2u> projMetaInfo(ArrayMetadata const &metadata)
{
    return Data::ProjMetaInfo<2u>{Data::ProjSize<2u>{metadata.num_cols, metadata.num_rows},
                                  metadata.num_slices};
}

}
