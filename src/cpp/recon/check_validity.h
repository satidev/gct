#ifndef CHECK_VALIDITY_H
#define CHECK_VALIDITY_H

#include "../../cpp/data/recon_img.h"
#include "../../cpp/data/projection_stack.h"
#include "../../cpp/geom/ct_geom.h"

namespace GCT::Recon
{
template<typename DetRow, typename ...DetCol>
bool isValidAxialRecon(Geom::CTGeom<DetRow, DetCol...> const &cg,
                       Data::ProjectionStack<float, Data::projDim<DetCol...>()> const &proj,
                       bool verbose);

namespace Detail
{
template<typename DetRow, typename ...DetCol>
bool checkProjDetConsistency(Geom::CTGeom<DetRow, DetCol...> const &cg,
                             Data::ProjectionStack<float, Data::projDim<DetCol...>()> const &proj,
                             bool verbose);

template<typename DetRow, typename ...DetCol>
bool checkProjViewConsistency(Geom::CTGeom<DetRow, DetCol...> const &cg,
                              Data::ProjectionStack<float, Data::projDim<DetCol...>()> const &proj,
                              bool verbose);

}//Detail namespace.


template<typename DetRow, typename ...DetCol>
bool Detail::checkProjDetConsistency(Geom::CTGeom<DetRow, DetCol...> const &cg,
                                     Data::ProjectionStack<float,
                                                           Data::projDim<DetCol...>()> const &proj,
                                     bool verbose)
{
    auto is_valid = true;
    auto const proj_size = proj.projSize();
    // Check the size of projection image and detector geometry.
    if (proj_size.num_cols != std::get<0>(cg.detGeom()).sampler.num_samples) {
        is_valid = false;
        if (verbose) {
            std::cerr << "Inconsistent projection and detector geometry data." << std::endl;
            std::cerr << "Number of columns of projection image:: " << proj_size.num_cols
                      << std::endl;
            std::cerr << "Number of detector columns:: " <<
                      std::get<0>(cg.detGeom()).sampler.num_samples
                      << std::endl;
        }
    }
    if constexpr (Data::projDim<DetCol...>() == 2u) {
        if (proj_size.num_rows != std::get<1>(cg.detGeom()).sampler.num_samples) {
            is_valid = false;
            if (verbose) {
                std::cerr << "Inconsistent projection and detector geometry data." << std::endl;
                std::cerr << "Number of rows of projection image:: " << proj_size.num_rows
                          << std::endl;
                std::cerr << "Number of detector rows:: " <<
                          std::get<1>(cg.detGeom()).sampler.num_samples
                          << std::endl;
            }
        }
    }
    return is_valid;
}

template<typename DetRow, typename ...DetCol>
bool isValidAxialRecon(Geom::CTGeom<DetRow, DetCol...> const &cg,
                       Data::ProjectionStack<float, Data::projDim<DetCol...>()> const &proj,
                       bool verbose)
{
    auto is_valid = true;
    is_valid = is_valid && Detail::checkProjDetConsistency(cg, proj, verbose);
    is_valid = is_valid && Detail::checkProjViewConsistency(cg, proj, verbose);
    return is_valid;
}

template<typename DetRow, typename ...DetCol>
bool Detail::checkProjViewConsistency(Geom::CTGeom<DetRow, DetCol...> const &cg,
                                      Data::ProjectionStack<float,
                                                            Data::projDim<DetCol...>()> const &proj,
                                      bool verbose)
{
    auto is_valid = true;
    // Check the number of projections.
    if (proj.numProj() != cg.projGeom().size()) {
        if (proj.numProj() > cg.projGeom().size()) {
            if(verbose){
                std::cerr
                    << "Projection geometry parameters are not available for all projection images."
                    << std::endl;
            }
        }
        else {
            if(verbose){
                std::cerr
                    << "Projection images are not available for all projection geometries."
                    << std::endl;
            }
        }
        if(verbose){
            std::cerr << "Number of projection images:: " << proj.numProj() << std::endl;
            std::cerr << "Number of projection geometry objects:: " << cg.projGeom().size()
                      << std::endl;
        }
        is_valid = false;
    }
    return is_valid;
}

}//GCT::Recon namespace.

#endif //CHECK_VALIDITY_H
