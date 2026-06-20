#ifndef EXAMPLE_RECONS_H
#define EXAMPLE_RECONS_H

#include <string>

namespace GCT::Examples
{
void recon1DParallelFullScan(std::string const &test_data_dir_name);
void reconArcDetFullScan(std::string const &test_data_dir_name);
void reconLineDetFullScan(std::string const &test_data_dir_name);
void recon2DParallelFullScan(std::string const &test_data_dir_name);
void reconRebinDetFullScan(std::string const &test_data_dir_name);
void reconCylDetFullScan(std::string const &test_data_dir_name);
void reconFlatDetFullScan(std::string const &test_data_dir_name);
void reconFlatDetShortScan(std::string const &test_data_dir_name);
}//GCT::Examples namespace.

#endif //EXAMPLE_RECONS_H
