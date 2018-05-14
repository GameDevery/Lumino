
#include "Internal.hpp"
#include <Lumino/Json/JsonDiagnostics.hpp>

namespace ln {

JsonDiag::JsonDiag()
    : code(JsonDiagCode::None)
    , line(0)
    , column(0)
    , message()
{
}

} // namespace ln
