#ifndef TOOLS_H
#define TOOLS_H
#include<chrono>
#include<sstream>
inline std::string gmt_time_now() {
  /**
   * Generate a UTC ISO8601-formatted timestamp
   * and return as std::string
   */
  auto now = std::chrono::system_clock::now();
  auto itt = std::chrono::system_clock::to_time_t(now);

  std::ostringstream ss;
  ss << std::put_time(gmtime(&itt), "%FT%TZ");
  return ss.str();
}

#endif
