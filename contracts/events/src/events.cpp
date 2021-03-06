#include <events/events.hpp>
#include <events/version.hpp>
#include <platform/platform.hpp>

namespace events {

events::events(name receiver, name code, eosio::datastream<const char*> ds):
    contract(receiver, code, ds),
    version(_self, _self.value),
    global(_self, _self.value)
{
    version.set(version_row {CONTRACT_VERSION}, _self);
}

void events::set_platform(name platform_name) {
    require_auth(get_self());

    auto gl = global.get_or_default();
    gl.platform = platform_name;
    global.set(gl, get_self());
}

void events::send(name sender, uint64_t casino_id, uint64_t game_id, uint64_t req_id, uint32_t event_type, bytes data) {
    require_auth(sender);

    const auto platform = get_platform();
    platform::read::get_casino(platform, casino_id);

    auto game = platform::read::get_game(platform, game_id);
    eosio::check(game.contract == sender, "incorrect sender(sender should be game's contract)");
}

} // namespace events
