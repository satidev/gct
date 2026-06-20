#ifndef RECON_WRAPPERS_CUH
#define RECON_WRAPPERS_CUH

#include "../../cpp/data/recon_img.h"
#include "../../cpp/data/projection_stack.h"
#include "../../cpp/geom/axial_ct_geom.h"
#include "../../cpp/geom/spiral_ct_geom.h"
#include "../../cpp/geom/detector_1d.h"

// Since templated CUDA functions cannot be called from cpp files,
// wrapper functions are created to call recon functions specific to
// each detector geometry (1D and 2D).
namespace GCT::Recon::CUDA
{
// 1D parallel recon.
void reconstruct(Geom::AxialCTGeom<Geom::DetParallel> const &cg,
                 Data::ProjectionStack<float, 1u> const &proj,
                 Data::ReconImg<float, 2u> &vol);
// 1D arc detector recon.
void reconstruct(Geom::AxialCTGeom<Geom::DetFanArc> const &cg,
                 Data::ProjectionStack<float, 1u> const &proj,
                 Data::ReconImg<float, 2u> &vol);
// 1D line detector recon.
void reconstruct(Geom::AxialCTGeom<Geom::DetFanLine> const &cg,
                 Data::ProjectionStack<float, 1u> const &proj,
                 Data::ReconImg<float, 2u> &vol);

// 2D parallel recon.
void reconstruct(Geom::AxialCTGeom<Geom::DetParallel, Geom::DetParallel> const &cg,
                 Data::ProjectionStack<float, 2u> const &proj,
                 Data::ReconImg<float, 3u> &vol);
// 2D rebin recon.
void reconstruct(Geom::AxialCTGeom<Geom::DetParallel, Geom::DetFanLine> const &cg,
                 Data::ProjectionStack<float, 2u> const &proj,
                 Data::ReconImg<float, 3u> &vol);
// 2D cylindrical detector recon.
void reconstruct(Geom::AxialCTGeom<Geom::DetFanArc, Geom::DetFanLine> const &cg,
                 Data::ProjectionStack<float, 2u> const &proj,
                 Data::ReconImg<float, 3u> &vol);
// 2D flat panel detector recon
void reconstruct(Geom::AxialCTGeom<Geom::DetFanLine, Geom::DetFanLine> const &cg,
                 Data::ProjectionStack<float, 2u> const &proj,
                 Data::ReconImg<float, 3u> &vol);

}//GCT::Recon::CUDA namespace.

#endif //RECON_WRAPPERS_CUH
