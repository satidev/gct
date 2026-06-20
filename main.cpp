#include "example_recons.h"
#include <chrono>
#include <iostream>
#include <filesystem>

int main(int argc, char *argv[])
{
    auto test_data_dir_name = std::string{};
    if (argc >= 2) {
        test_data_dir_name = std::string{argv[1]};
    }
    if (std::filesystem::exists(std::filesystem::path{test_data_dir_name})) {

        auto const start = std::chrono::high_resolution_clock::now();

        std::cout << "Reconstructing image from 1D parallel projections." << std::endl;
        GCT::Examples::recon1DParallelFullScan(test_data_dir_name);

        std::cout << "Reconstructing image from 1D fan-beam (arc detector) projections." << std::endl;
        GCT::Examples::reconArcDetFullScan(test_data_dir_name);

        std::cout << "Reconstructing image from 1D fan-beam (line detector) projections." << std::endl;
        GCT::Examples::reconLineDetFullScan(test_data_dir_name);

        std::cout << "Reconstructing volume from 2D parallel projections." << std::endl;
        GCT::Examples::recon2DParallelFullScan(test_data_dir_name);

        std::cout << "Reconstructing volume from 2D cone-beam (flat detector) full-scan projections." << std::endl;
        GCT::Examples::reconFlatDetFullScan(test_data_dir_name);

        std::cout << "Reconstructing volume from 2D cone-beam (flat detector) short-scan projections." << std::endl;
        GCT::Examples::reconFlatDetShortScan(test_data_dir_name);

        std::cout << "Reconstructing volume from 2D cone-beam (cylindrical detector) projections." << std::endl;
        GCT::Examples::reconCylDetFullScan(test_data_dir_name);

        std::cout << "Reconstructing volume from 2D cone-parallel (rebinned detector) projections." << std::endl;
        GCT::Examples::reconRebinDetFullScan(test_data_dir_name);

        auto const finish = std::chrono::high_resolution_clock::now();
        auto const elapsed = std::chrono::duration<float>{finish - start};
        std::cout << "Elapsed time: " << elapsed.count() << " seconds." << std::endl;
    }
    else {
        std::cout << "Please provide the name of the test data directory as the first command-line argument."
                  << std::endl;
    }
    return 0;
}

