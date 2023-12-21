#include "StatusTags.hpp"

#include <Types/Entity.hpp>
#include <Types/Enums/Status.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::tags::status {
void enumToTag(dex::Status status, types::handle& handle) {
  switch (status) {
    case dex::Status::BRN: handle.emplace<Burn>(); return;
    case dex::Status::FRZ: handle.emplace<Freeze>(); return;
    case dex::Status::PAR: handle.emplace<Paralysis>(); return;
    case dex::Status::PSN: handle.emplace<Poison>(); return;
    case dex::Status::SLP: handle.emplace<Sleep>(); return;
    case dex::Status::TOX: handle.emplace<Toxic>(); return;
    default: {
      ENTT_ASSERT(false, "Adding tag for status that does not exist");
    }
  }
}
}  // namespace pokesim::tags::status
