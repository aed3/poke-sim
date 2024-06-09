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
struct UserAlly {};
struct TargetSide {};
struct UserSide {};
struct Field {};
}  // namespace added_targets::tags
}  // namespace pokesim::move
