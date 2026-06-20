#include "cuda_config.cuh"

namespace GCT::CUDA
{

unsigned int gridSize(unsigned int tot_size, unsigned int block_size)
{
  return (tot_size + block_size - 1u) / block_size;
}

template<>
std::pair<dim3, dim3> execConfig(
    std::array<unsigned int, 1u> const &size)
{
  auto const block = dim3{64u, 1u, 1u};
  auto const grid = dim3{gridSize(size.at(0), block.x), 1u, 1u};
  return std::make_pair(grid, block);
}

template<>
std::pair<dim3, dim3> execConfig(
    std::array<unsigned int, 2u> const &size)
{
  auto const block = dim3{32u, 32u, 1u};
  auto const grid = dim3{gridSize(size.at(0), block.x),
                         gridSize(size.at(1), block.y), 1u};
  return std::make_pair(grid, block);
}

template<>
std::pair<dim3, dim3> execConfig(
    std::array<unsigned int, 3u> const &size)
{
  auto const block = dim3{16u, 16u, 4u};
  auto const grid = dim3{gridSize(size.at(0), block.x),
                         gridSize(size.at(1), block.y),
                         gridSize(size.at(2), block.z)};
  return std::make_pair(grid, block);
}

}//GCT::CUDA namespace.


