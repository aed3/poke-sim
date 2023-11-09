#include "NatureTags.hpp"

#include <Types/Nature.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::tags::nature {
void enumToTag(dex::Nature nature, entt::handle handle) {
  switch (nature) {
    case dex::ADAMANT: handle.emplace<Adamant>(); return;
    case dex::BASHFUL: handle.emplace<Bashful>(); return;
    case dex::BOLD: handle.emplace<Bold>(); return;
    case dex::BRAVE: handle.emplace<Brave>(); return;
    case dex::CALM: handle.emplace<Calm>(); return;
    case dex::CAREFUL: handle.emplace<Careful>(); return;
    case dex::DOCILE: handle.emplace<Docile>(); return;
    case dex::GENTLE: handle.emplace<Gentle>(); return;
    case dex::HARDY: handle.emplace<Hardy>(); return;
    case dex::HASTY: handle.emplace<Hasty>(); return;
    case dex::IMPISH: handle.emplace<Impish>(); return;
    case dex::JOLLY: handle.emplace<Jolly>(); return;
    case dex::LAX: handle.emplace<Lax>(); return;
    case dex::LONELY: handle.emplace<Lonely>(); return;
    case dex::MILD: handle.emplace<Mild>(); return;
    case dex::MODEST: handle.emplace<Modest>(); return;
    case dex::NAIVE: handle.emplace<Naive>(); return;
    case dex::NAUGHTY: handle.emplace<Naughty>(); return;
    case dex::QUIET: handle.emplace<Quiet>(); return;
    case dex::QUIRKY: handle.emplace<Quirky>(); return;
    case dex::RASH: handle.emplace<Rash>(); return;
    case dex::RELAXED: handle.emplace<Relaxed>(); return;
    case dex::SASSY: handle.emplace<Sassy>(); return;
    case dex::SERIOUS: handle.emplace<Serious>(); return;
    case dex::TIMID: handle.emplace<Timid>(); return;
    default: {
      ENTT_ASSERT(false, "Adding tag for nature that does not exist");
    }
  }
}
}  // namespace pokesim::tags::nature
