#include "NatureTags.hpp"

#include <Types/Entity.hpp>
#include <Types/Enums/Nature.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::nature::tags {
void enumToTag(dex::Nature nature, types::handle handle) {
  switch (nature) {
    case dex::Nature::ADAMANT: handle.emplace<Adamant>(); return;
    case dex::Nature::BASHFUL: handle.emplace<Bashful>(); return;
    case dex::Nature::BOLD: handle.emplace<Bold>(); return;
    case dex::Nature::BRAVE: handle.emplace<Brave>(); return;
    case dex::Nature::CALM: handle.emplace<Calm>(); return;
    case dex::Nature::CAREFUL: handle.emplace<Careful>(); return;
    case dex::Nature::DOCILE: handle.emplace<Docile>(); return;
    case dex::Nature::GENTLE: handle.emplace<Gentle>(); return;
    case dex::Nature::HARDY: handle.emplace<Hardy>(); return;
    case dex::Nature::HASTY: handle.emplace<Hasty>(); return;
    case dex::Nature::IMPISH: handle.emplace<Impish>(); return;
    case dex::Nature::JOLLY: handle.emplace<Jolly>(); return;
    case dex::Nature::LAX: handle.emplace<Lax>(); return;
    case dex::Nature::LONELY: handle.emplace<Lonely>(); return;
    case dex::Nature::MILD: handle.emplace<Mild>(); return;
    case dex::Nature::MODEST: handle.emplace<Modest>(); return;
    case dex::Nature::NAIVE: handle.emplace<Naive>(); return;
    case dex::Nature::NAUGHTY: handle.emplace<Naughty>(); return;
    case dex::Nature::QUIET: handle.emplace<Quiet>(); return;
    case dex::Nature::QUIRKY: handle.emplace<Quirky>(); return;
    case dex::Nature::RASH: handle.emplace<Rash>(); return;
    case dex::Nature::RELAXED: handle.emplace<Relaxed>(); return;
    case dex::Nature::SASSY: handle.emplace<Sassy>(); return;
    case dex::Nature::SERIOUS: handle.emplace<Serious>(); return;
    case dex::Nature::TIMID: handle.emplace<Timid>(); return;
    default: {
      ENTT_ASSERT(false, "Adding tag for nature that does not exist");
    }
  }
}
}  // namespace pokesim::nature::tags
