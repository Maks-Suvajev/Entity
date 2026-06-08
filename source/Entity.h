#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <limits>

namespace Entity
{

using Entity = uint32_t;
inline constexpr Entity MaxEntityValue = std::numeric_limits<uint32_t>::max();
inline constexpr Entity NullEntity = 0U;

}
#endif