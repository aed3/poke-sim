#include "StatusTags.hpp"

#include <Types/Status.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::tags::status {
void enumToTag(dex::Status status, entt::handle& handle) {
  switch (status) {
    case dex::BRN: handle.emplace<Burn>(); return;
    case dex::FRZ: handle.emplace<Freeze>(); return;
    case dex::PAR: handle.emplace<Paralysis>(); return;
    case dex::PSN: handle.emplace<Poison>(); return;
    case dex::SLP: handle.emplace<Sleep>(); return;
    case dex::TOX: handle.emplace<Toxic>(); return;
    default: {
      ENTT_ASSERT(false, "Adding tag for status that does not exist");
    }
  }
}
}  // namespace pokesim::tags::status
