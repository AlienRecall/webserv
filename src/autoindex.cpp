#include "../include/Response.hpp"

std::string complete_page(const std::string &path, const std::string &list) {
    std::string start =
        "<!doctypehtml><html><title>Index of " + path + "</title><h1>Index of" + path +
        "</h1><table><tr><th valign=top><th><a>Name</a><th><a>Last "
        "modified</a><th><a>Size</a><th><a>Description</a><tr><th colspan=5><hr>";
    return start + list + "<tr><th colspan=5><hr></table></html>";
}

std::string direntry_to_html(const std::string &path, std::string &name,
                             struct stat *buf) {
    std::string last_mod = "-", size = "-";
    if (buf) {
        last_mod = std::ctime(&(buf->st_mtime));
        size = Response::itoa(buf->st_size / 1000);
    }

    return "<tr><td valign=top> <td><a href=" + path + "/" + name + ">" + name +
           "</a><td align=right>" + last_mod + "<td align=right>" + size + " KB<td>";
}

Error list_dir(const std::string &path, std::string &list) {
    struct dirent *de;
    struct stat buf;
    std::vector<std::string> arr;

    DIR *dr = opendir(path.c_str());
    if (dr == NULL)
        return CUSTOM;

    while ((de = readdir(dr)) != NULL) {
        arr.push_back(de->d_name);
    }
    closedir(dr);

    std::sort(arr.begin(), arr.end());
    for (std::vector<std::string>::iterator it = arr.begin(); it != arr.end(); it++)
        list += direntry_to_html(path, *it,
                                 stat((path + *it).c_str(), &buf) == -1 ? 0 : &buf);

    return OK;
}

void Response::make_autoindex(const std::string &path, Config *config) {
    std::string list = "";

    Error err = list_dir(path, list);
    if (err != OK) {
        std::cout << "webserv: make_autoindex: cannot opendir: " << path << std::endl;
        return make_500(config);
    }

    set_protocol(PROTOCOL_11);
    set_status(STATUS_OK);
    set_body(complete_page(path, list));
    set_header("Content-Type", "text/html; charset=utf-8");
    add_default_headers();
}
