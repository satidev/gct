#include "recon_wrappers.cuh"
#include "../recon/fdk_axial.cuh"
#include "../recon/fdk_spiral.cuh"

namespace GCT::Recon::CUDA
{
void reconstruct(Geom::AxialCTGeom<Geom::DetParallel> const &cg,
                 Data::ProjectionStack<float, 1u> const &proj,
                 Data::ReconImg<float, 2u> &vol)
{
    reconFDK<Geom::DetParallel>(cg, proj, vol);
}

void reconstruct(Geom::AxialCTGeom<Geom::DetFanArc> const &cg,
                 Data::ProjectionStack<float, 1u> const &proj,
                 Data::ReconImg<float, 2u> &vol)
{
    reconFDK<Geom::DetFanArc>(cg, proj, vol);
}

void reconstruct(Geom::AxialCTGeom<Geom::DetFanLine> const &cg,
                 Data::ProjectionStack<float, 1u> const &proj,
                 Data::ReconImg<float, 2u> &vol)
{
    reconFDK<Geom::DetFanLine>(cg, proj, vol);
}


void reconstruct(Geom::AxialCTGeom<Geom::DetParallel, Geom::DetParallel> const &cg,
                 Data::ProjectionStack<float, 2u> const &proj,
                 Data::ReconImg<float, 3u> &vol)
{
    reconFDK<Geom::DetParallel, Geom::DetParallel>(cg, proj, vol);
}

void reconstruct(Geom::AxialCTGeom<Geom::DetParallel, Geom::DetFanLine> const &cg,
                 Data::ProjectionStack<float, 2u> const &proj,
                 Data::ReconImg<float, 3u> &vol)
{
    reconFDK<Geom::DetParallel, Geom::DetFanLine>(cg, proj, vol);
}

void reconstruct(Geom::AxialCTGeom<Geom::DetFanArc, Geom::DetFanLine> const &cg,
                 Data::ProjectionStack<float, 2u> const &proj,
                 Data::ReconImg<float, 3u> &vol)
{
    reconFDK<Geom::DetFanArc, Geom::DetFanLine>(cg, proj, vol);
}

void reconstruct(Geom::AxialCTGeom<Geom::DetFanLine, Geom::DetFanLine> const &cg,
                 Data::ProjectionStack<float, 2u> const &proj,
                 Data::ReconImg<float, 3u> &vol)
{
    reconFDK<Geom::DetFanLine, Geom::DetFanLine>(cg, proj, vol);
}


void reconstruct(Geom::SpiralCTGeom<Geom::DetFanArc, Geom::DetFanLine> const &cg,
                 Data::ProjectionStack<float, 2u> const &proj,
                 Data::ReconImg<float, 3u> &vol)
{
    reconFDK<Geom::DetFanArc, Geom::DetFanLine>(cg, proj, vol);
}

}//GCT::Recon::CUDA namespace.










