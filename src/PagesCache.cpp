#include "../include/PagesCache.hpp"
#include <string>

void Pages::init() {
    push("./assets/default_pages/400.html", "400");
    push("./assets/default_pages/401.html", "401");
    push("./assets/default_pages/403.html", "403");
    push("./assets/default_pages/404.html", "404");
    push("./assets/default_pages/405.html", "405");
    push("./assets/default_pages/500.html", "500");
}

Error Pages::push(const std::string &path, std::string key) {
    std::ostringstream buff;
    std::ifstream file(path.c_str());

    if (!file.is_open())
        return OPEN_FILE;

    buff << file.rdbuf();
    cache[key.empty() ? path : key] = buff.str();
    return OK;
}

const std::string Pages::get(const std::string &key) {
    iterator it = cache.find(key);
    if (it != cache.end())
        return it->second;

    Error err = push(key);
    if (err != OK) {
        std::cout << "webserv: PagesCache: cannot find page file: `" << key << "`."
                  << std::endl;
        return get_500();
    }
    return get(key);
}

const std::string Pages::get_302(const std::string &url) {
    std::string page = (std::string)get("302");
    while (size_t pos = page.find("$REDIRECT") != std::string::npos) {
        page.replace(pos, 9, url);
    }
    return page;
}

const std::string Pages::get_400() { return get("400"); }

const std::string Pages::get_401() { return get("401"); }

const std::string Pages::get_403() { return get("403"); }

const std::string Pages::get_404() { return get("404"); }

const std::string Pages::get_405() { return get("405"); }

const std::string Pages::get_500() { return get("500"); }
