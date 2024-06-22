#include "../include/Response.hpp"

std::string gen_random(const int len) {
    static const char alphanum[] = "0123456789"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

std::string filename(std::string ct) {
    int i = 0;
    std::string name = gen_random(8);

    for (; i < 2; i++) {
        if (ct == supported_types[i])
            break;
    }

    switch (i) {
    case 0:
        return name + ".png";
    case 1:
        return name + ".jpeg";
    default:
        return name;
    }
}

void parse_multipart(std::string &ct, const std::string &body, t_file &file_data) {

    size_t pos = ct.find("boundary=");
    if (pos == std::string::npos)
        return;

    int start = 0;
    std::string boundary = "--" + ct.substr(pos + 9);
    std::string line;
    std::stringstream ss(body);

    while (std::getline(ss, line)) {
        start += line.size() + 1;
        if (line == boundary || line.find("Content-Type") != std::string::npos)
            continue;
        if ((pos = line.find("filename=\"")) != std::string::npos) {
            pos += 10;
            size_t end = line.find("\"", pos);
            std::string fname = line.substr(pos, end - pos);
            pos = fname.find_last_of('.');
            file_data.file_name += gen_random(8) + "." + fname.substr(pos + 1);
            continue;
        }
        if (!file_data.file_name.empty()) {
            start -= line.size();
            size_t end = body.find(boundary, start);
            if (end == std::string::npos) {
                end = body.find(boundary + "--", start);
                if (end == std::string::npos)
                    return;
            }
            file_data.data = body.substr(start + 1, end - start);
            break;
        }
    }
    std::cout << "filename: " << file_data.file_name
              << " data size: " << file_data.data.size() << std::endl;
}

void Response::save_file(Request &req, const std::string &dir) {
    t_file f;

    f.file_name = dir + "/";
    Request::iterator it = req.get_header("Content-Type");
    if (it->second.find("multipart/form-data") != std::string::npos)
        parse_multipart(it->second, req.get_body(), f);
    else {
        f.file_name += filename(it->second);
        f.data = req.get_body();
    }

    int out = open(f.file_name.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(out, f.data.c_str(), f.data.size());
    close(out);

    set_body(Pages::get_200());
    return make_page();
}
