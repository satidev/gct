#ifndef PROJECTION_READER_H
#define PROJECTION_READER_H

#include "../../data/projection_stack.h"
#include "../../data/proj_meta_info.h"
#include <string>
#include "../metadata.h"

namespace GCT::IO
{
template<size_t Dim>
Data::ProjMetaInfo<Dim> projMetaInfo(ArrayMetadata const &metadata);

template<typename FileReader>
struct ProjectionReader
{
public:
    template<typename PixType, size_t Dim>
    static Data::ProjectionStack<PixType, Dim> read(std::string const &filename,
                                                    bool series = false);
};

template<typename FileReader>
template<typename PixType, size_t Dim>
Data::ProjectionStack<PixType, Dim>
ProjectionReader<FileReader>::read(const std::string &filename, bool series)
{
    auto const reader = FileReader{filename, series};
    auto const meta_info = projMetaInfo<Dim>(reader.readMetadata());
    auto proj = Data::ProjectionStack<PixType, Dim>{meta_info.size, meta_info.num_proj};
    auto pixel_data = reader.template readData<PixType>();
    proj.setData(std::move(pixel_data));

    return proj;
}

}//GCT::IO namespace.


#endif //PROJECTION_READER_H
