#include "mess/engine/I18n.h"

namespace mess {
namespace engine {
namespace detail {

#define TR(key, access, priority, str) std::string TR_STR_ ## key = str;
#include "Messages.in"
#undef TR

} // namespace detail
} // namespace engine
} // namespace mess
