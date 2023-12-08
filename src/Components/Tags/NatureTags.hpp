#pragma once
#include <Types/Enums/Nature.hpp>
#include <entt/entity/fwd.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::tags::nature {
struct Adamant {};
struct Bashful {};
struct Bold {};
struct Brave {};
struct Calm {};
struct Careful {};
struct Docile {};
struct Gentle {};
struct Hardy {};
struct Hasty {};
struct Impish {};
struct Jolly {};
struct Lax {};
struct Lonely {};
struct Mild {};
struct Modest {};
struct Naive {};
struct Naughty {};
struct Quiet {};
struct Quirky {};
struct Rash {};
struct Relaxed {};
struct Sassy {};
struct Serious {};
struct Timid {};

// Assigns a nature's tag to a handle
/*_inline_*/ void enumToTag(dex::Nature nature, entt::handle handle);
}  // namespace pokesim::tags::nature
