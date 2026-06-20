#ifndef RECON_IMG_GEOM_TEST_FIX_H
#define RECON_IMG_GEOM_TEST_FIX_H

#include "../../../src/cpp/geom/recon_img_geom.h"

namespace GCT::Test
{
struct ReconImgGeomTestFix
{
    static constexpr auto geom2d = Geom::ReconImgGeom<2u>{Geom::EqDistSampler {1u, 2.3f},
                                                          Geom::EqDistSampler{2u, 3.4f},
                                                          Geom::VecWCS<2u>{1.2f, 3.5f}};
    static constexpr auto geom3d = Geom::ReconImgGeom<3u>{Geom::EqDistSampler{1u, 2.3f},
                                                          Geom::EqDistSampler{2u, 3.4f},
                                                          Geom::EqDistSampler{3u, 4.5f},
                                                          Geom::VecWCS<3u>{1.2f, 3.5f}};
};

}//GCT::Test namespace.

#endif //RECON_IMG_GEOM_TEST_FIX_H


