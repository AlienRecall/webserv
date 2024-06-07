#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>
#include <list>

class Route {
  private:
    std::list<std::string> _methods;
    std::string _redirect;
    std::string _base_dir;
    bool _dir_listing;
    std::string _base_file;
    std::list<std::string> _cgi_extensions;
    std::string _upload_location;

  public:
    Route();
    ~Route();
    Route(const Route &);
    Route &operator=(const Route &);
};

#endif
