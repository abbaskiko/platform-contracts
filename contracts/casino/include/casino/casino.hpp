#pragma once

#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>
#include <platform/version.hpp>
#include <platform/platform.hpp>

namespace casino {

using eosio::name;

using game_params_type = std::vector<std::pair<uint16_t, uint32_t>>;

struct [[eosio::table("game"), eosio::contract("casino")]] game_row {
    uint64_t game_id;
    game_params_type params;

    uint64_t primary_key() const { return game_id; }
};

using game_table = eosio::multi_index<"game"_n, game_row>;

struct [[eosio::table("version"), eosio::contract("casino")]] version_row {
    std::string version;
};
using version_singleton = eosio::singleton<"version"_n, version_row>;

struct [[eosio::table("owner"), eosio::contract("casino")]] owner_row {
    name owner;
};
using owner_singleton = eosio::singleton<"owner"_n, owner_row>;

class [[eosio::contract("casino")]] casino: public eosio::contract {
public:
    using eosio::contract::contract;

    static constexpr name platform_account{"dao.platform"_n};

    casino(name receiver, name code, eosio::datastream<const char*> ds):
        contract(receiver, code, ds),
        version(_self, _self.value),
        games(_self, _self.value),
        owner_account(_self, _self.value)
    {
        version.set(version_row{::platform::CONTRACT_VERSION}, _self);
        owner_account.set(owner_row{_self}, _self);
    }

    [[eosio::action("addgame")]]
    void add_game(uint64_t game_id, game_params_type params);
    [[eosio::action("rmgame")]]
    void remove_game(uint64_t game_id);
    [[eosio::action("setowner")]]
    void set_owner(name new_owner);
private:
    version_singleton version;
    game_table games;
    owner_singleton owner_account;

    name get_owner() {
        return owner_account.get().owner;
    }
};

} // namespace casino