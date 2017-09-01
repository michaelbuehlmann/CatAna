#ifndef CATANA_APP_STRING_FORMAT_HPP
#define CATANA_APP_STRING_FORMAT_HPP

#include <memory>
#include <iostream>
#include <string>
#include <cstdio>


namespace catana {

  //! A String formating function, similar to Python .format.
  template<typename ... Args>
  std::string string_format(const std::string& format, Args ... args) {
    size_t size = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
  }

}
#endif //CATANA_APP_STRING_FORMAT_HPP
