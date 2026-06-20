#ifndef IMG_WRITER_ALL_H
#define IMG_WRITER_ALL_H

#include "../../data/recon_img.h"
#include <string>
#include "../file_writer/den_file_writer.h"
#include "../file_writer/meta_img_file_writer.h"

namespace GCT::IO
{
template<size_t Dim>
ArrayFileWriter::DenMetaData denMetaData(Geom::ReconImgGeom<Dim> const &geom);

template<size_t Dim>
ArrayFileWriter::MetaImgMetaData<Dim> metaImgMetaData(Geom::ReconImgGeom<Dim> const &geom);

template<typename PixType, size_t Dim>
void write(Data::ReconImg<PixType, Dim> const &img, std::string const &filename);

template<typename PixType, size_t Dim>
void write(Data::ReconImg<PixType, Dim> const &img, std::string const &filename)
{
    auto const path = std::filesystem::path(filename).extension();
    auto const pixel_data = img.elements();

    if(toLower(path) == std::string{".den"}){

        auto const metadata = denMetaData(img.geom());
        ArrayFileWriter::write(metadata, pixel_data, filename);
    }
    else if((toLower(path) == std::string{".mha"}) ||
        (toLower(path) == std::string{".mhd"})){

        auto const metadata = metaImgMetaData(img.geom());
        ArrayFileWriter::write(metadata, pixel_data, filename);
    }
    else{
        std::cerr << "Currently images can be saved to den or meta image file." << std::endl;
    }
}

}

#endif //IMG_WRITER_ALL_H
