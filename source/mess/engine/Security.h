#pragma once

#include "crypto/password_digest.h"
#include "mess/NonCopyable.h"
#include "mess/engine/Access.h"
#include "mess/engine/Exception.h"
#include "mess/engine/PlayerId.h"

#include <unordered_map>
#include <stdexcept>

namespace mess {
namespace engine {

  using secure_string = crypto::secure_string;

  class Security : public NonCopyable {
  public:

    explicit Security(const secure_string &game_password);

    PlayerId AddMasterPlayer(
        const secure_string &game_password,
        const secure_string &player_password);

    PlayerId AddPlayer(
        const secure_string &game_password,
        const secure_string &player_password);

    bool Check(const PlayerId &id, const secure_string &password) const;

    inline void Require(const PlayerId &id, const secure_string &password) const {
      if (!Check(id, password))
        throw access_denied_error();
    }

    bool CheckDataAccess(
        const PlayerId &requester,
        const PlayerId &data_owner,
        Access data_access) const;

  private:

    using PasswordDigest = crypto::password_digest<crypto::sha512_digest>;

    struct PlayerData {
      const uint8_t access;
      PasswordDigest password;
    };

    PlayerId MakePlayer(
        const secure_string &game_password,
        const secure_string &player_password,
        uint8_t access);

    const PlayerData &GetPlayer(const PlayerId &id) const;

    PlayerId GenerateId() const;

    static PasswordDigest MakeDigest(const PlayerId &id, const secure_string &password);

    std::unordered_map<PlayerId::value_type, PlayerData> _players;

    PlayerId _self_id;
  };

} // namespace engine
} // namespace mess
