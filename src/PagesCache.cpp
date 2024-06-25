#include "../include/PagesCache.hpp"
#include <string>

void Pages::init() {
    push("./html/200.html", "200");
    push("./html/302.html", "302");
    push("./html/400.html", "400");
    push("./html/401.html", "401");
    push("./html/403.html", "403");
    push("./html/404.html", "404");
    push("./html/405.html", "405");
    push("./html/500.html", "500");
    push("./html/timeout.html", "timeout");
}

Error Pages::push(const std::string &path, std::string key)
{
    // Controlla se il file contiene "_custom" prima dell'estensione ".html"
    size_t pos = path.rfind(".html");
    if (pos != std::string::npos)
    {
        size_t custom_pos = path.rfind("_custom", pos);
        if (custom_pos != std::string::npos && custom_pos + 7 == pos)
        {
            std::cerr << "Error: Custom error files are not accepted. File: " << path << std::endl;
            return INVALID_CUSTOM_FILE;
        }
    }
    std::ifstream file(path.c_str());

    if (!file.is_open())
    {
        std::cerr << "Error: File " << path << " not found." << std::endl;
        // return OPEN_FILE;
    }

    std::ostringstream buff;
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

const std::string Pages::get_200() { return get("200"); }

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

const std::string Pages::get_timeout() { return get("timeout"); }
