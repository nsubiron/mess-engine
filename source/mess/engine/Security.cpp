#include "mess/engine/Security.h"

#include "crypto/random.h"

namespace mess {
namespace engine {

  static constexpr uint8_t cast(Access a) {
    return static_cast<uint8_t>(a);
  }

#define ACCESS(level) cast(Access:: level)

  Security::Security(const secure_string &game_password)
    : _players(),
      _self_id(GenerateId()) {
    _players.insert({_self_id._id, PlayerData{0u, MakeDigest(_self_id, game_password)}});
  }

  PlayerId Security::AddMasterPlayer(
      const secure_string &game_password,
      const secure_string &player_password) {
    const auto access = ACCESS(Master)|ACCESS(Player)|ACCESS(Faction)|ACCESS(Everyone);
    return MakePlayer(game_password, player_password, access);
  }

  PlayerId Security::AddPlayer(
      const secure_string &game_password,
      const secure_string &player_password) {
    return MakePlayer(game_password, player_password, ACCESS(Everyone));
  }

  bool Security::Check(const PlayerId &id, const secure_string &password) const {
    return (GetPlayer(id).password == MakeDigest(id, password));
  }

  bool Security::CheckDataAccess(
      const PlayerId &requester,
      const PlayerId &data_owner,
      const Access data_access) const {
    uint8_t access = GetPlayer(requester).access;
    if (requester == data_owner)
      access |= ACCESS(Player);
    // if ( same faction )
    //   access |= ACCESS(Faction);
    return access & cast(data_access);
  }

  PlayerId Security::MakePlayer(
      const secure_string &game_password,
      const secure_string &player_password,
      const uint8_t access) {
    Require(_self_id, game_password);
    PlayerId new_id = GenerateId();
    _players.insert({new_id._id, {access, MakeDigest(new_id, player_password)}});
    return new_id;
  }

  const Security::PlayerData &Security::GetPlayer(const PlayerId &id) const {
    auto it = _players.find(id._id);
    if (it == _players.end()) {
      throw std::invalid_argument("invalid player id");
    }
    return it->second;
  }

  PlayerId Security::GenerateId() const {
    static thread_local crypto::mt19937 PLAYER_RNG;
    using T = PlayerId::value_type;
    std::uniform_int_distribution<T> distribution(
        std::numeric_limits<T>::lowest(),
        std::numeric_limits<T>::max());
    T id = 0u;
    auto has_key = [](auto map, auto id){ return (map.find(id) != map.end()); };
    while ((id == 0u) || has_key(_players, id)) {
      id = distribution(PLAYER_RNG);
    }
    return id;
  }

  Security::PasswordDigest Security::MakeDigest(const PlayerId &id, const secure_string &password) {
    auto hex = string::to_hex_string(reinterpret_cast<const unsigned char *>(&id._id), sizeof(id._id));
    return PasswordDigest(secure_string::unsafe_make(hex) + password);
  }

} // namespace engine
} // namespace mess
