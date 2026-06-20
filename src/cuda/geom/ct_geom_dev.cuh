#ifndef CT_GEOM_DEV_CUH
#define CT_GEOM_DEV_CUH

#include "../../cpp/geom/ct_geom.h"
#include "proj_geom_dev.cuh"
#include "det_geom_dev.cuh"
#include <vector>

namespace GCT::Geom::CUDA
{
//CT geometry parameters for CUDA kernels: The detector and projection geometry parameters
//of the entire scan are stored in CUDA unified memory and be accessed by the kernels.
template<typename DetRow, typename ...DetCol>
struct CTGeomDev
{
public:
    explicit CTGeomDev(CTGeom<DetRow, DetCol...> const &cg_host)
        :
        dg{cg_host.detGeom()},
        num_proj{static_cast<unsigned>(cg_host.projGeom().size())},
        pg_scan{num_proj}
    {
        if (num_proj == 0u) {
            throw std::runtime_error{"Empty collection of projection geometry.\n"};
        }
        allocateProjGeom();
        copyProjGeomHostToDev(cg_host.projGeom());
    }

    DetGeomDev<DetRow, DetCol...> const &detGeom() const noexcept
    {
        return dg;
    }
    unsigned numProj() const noexcept
    {
        return num_proj;
    }
    ProjGeomScanDev<DetRow, DetCol...> const *projGeom() const noexcept
    {
        return pg_scan.dataPointer();
    }

    ~CTGeomDev()
    {
        deallocateProjGeom();
    }

private:
    DetGeomDev<DetRow, DetCol...> dg;
    unsigned num_proj;
    Data::CUDA::UnifiedMem<ProjGeomScanDev<DetRow, DetCol...>> pg_scan;

    void allocateProjGeom();
    void deallocateProjGeom();
    void copyProjGeomHostToDev(std::vector<ProjGeom<DetRow, DetCol...>> const &host);
};

}//GCT::Geom::CUDA namespace.

#endif //CT_GEOM_DEV_CUH


