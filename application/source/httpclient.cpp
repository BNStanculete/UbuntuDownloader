// Copyright HVortex 2024.

#include "httpclient.hpp"

#include <string>
#include <iostream>

namespace downloader {

HttpResponse HttpClient::GET(const std::string& host, const std::string& url) {
    HttpResponse response;

    const auto address = resolver_.resolve(host, "80", ec_);

    if (ec_.failed()) {
        std::cout << "[ERR] Could not resolve host: " << host << "\n";
        std::cout << "[ERR] Error code: " << ec_.message() << "\n";

        return response;
    }

    stream_.connect(address, ec_);

    if (ec_.failed()) {
        std::cout << "[ERR] Could not establish connection to: " << host << "\n";
        std::cout << "[ERR] Error code: " << ec_.message() << "\n";

        return response;
    }

    HttpRequest request{http::verb::get, url, 12};
    request.set(http::field::host, host);
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    http::write(stream_, request);
    http::read(stream_, buffer_, response);

    stream_.socket().shutdown(tcp::socket::shutdown_both, ec_);

    if (ec_.failed()) {
        std::cout << "[WRN] Could not close connection to: " << host << "\n";
        std::cout << "[WRN] Error code: " << ec_.message() << "\n";
    }

    return response;
}

}  // namespace downloader
