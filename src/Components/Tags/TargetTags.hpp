#pragma once

namespace pokesim::move {
namespace tags {
struct AnySingleTarget {};
struct AnySingleFoe {};
struct AnySingleAlly {};
struct AllyOrSelf {};
struct Self {};
struct AllFoes {};
struct AlliesAndFoes {};
struct AlliesAndSelf {};
struct FoeSide {};
struct AllySide {};
struct Field {};
struct AllyTeam {};
struct Retaliation {};
struct RandomFoe {};
}  // namespace tags

namespace added_targets::tags {
struct TargetAlly {};
struct SourceAlly {};
struct TargetSide {};
struct SourceSide {};
struct Field {};
}  // namespace added_targets::tags

namespace singles_target::tags {
struct Self {};
struct Foe {};
}  // namespace singles_target::tags
}  // namespace pokesim::move
