#include <iostream>
#include <sstream>
#include <xmsgrid/geometry/geoms.h>

int main() {
  std::stringstream ss;

  ss << "Left: " << xms::Turn_enum::TURN_LEFT << std::endl;
  ss << "Right: " << xms::Turn_enum::TURN_RIGHT << std::endl;
  ss << "Turn 180: " << xms::Turn_enum::TURN_COLINEAR_180 << std::endl;
  ss << "Turn 0: " << xms::Turn_enum::TURN_COLINEAR_0 << std::endl;

  std::cout << ss.str() << std::endl;
}
