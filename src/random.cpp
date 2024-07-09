
#include "random.hpp"

std::random_device plgl::impl::random_device;
plgl::Random plgl::impl::random {plgl::impl::random_device()};
siv::PerlinNoise plgl::impl::perlin {plgl::impl::random_device()};
unsigned int plgl::impl::octaves = 1;
