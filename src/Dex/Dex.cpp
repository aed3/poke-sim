#include "Dex.hpp"

namespace pokesim {
std::string Dex::toID(const std::string& name) {
  std::string idName;
  for (auto character : name) {
    if (character <= 'Z' && character >= 'A') {
      idName += (char)(character + ' ');
    }
    else if ((character <= '9' && character >= '0') || (character <= 'z' && character >= 'a')) {
      idName += character;
    }
  }

  return idName;
}
}  // namespace pokesim
