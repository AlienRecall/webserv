#ifndef PAGESCACHE_HPP
#define PAGESCACHE_HPP

#include "Logger.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class Pages {
  private:
    typedef std::map<const std::string, std::string>::iterator iterator;

  public:
    static std::map<const std::string, std::string> cache;

    static const std::string get(const std::string &);
    static const std::string get_302(const std::string &);
    static const std::string get_400();
    static const std::string get_401();
    static const std::string get_403();
    static const std::string get_404();
    static const std::string get_405();
    static const std::string get_500();
    static const std::string get_timeout();

    static Error push(const std::string &, std::string = "");
    static void init();
};

#endif
