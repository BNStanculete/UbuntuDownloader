// Copyright 2024 HV0rtex

#include <json/json.h>
#include <iostream>
#include <memory>
#include <string>
#include "Httpclient.hpp"
#include "Parser.hpp"

/*
Possible arguments from the command line:
    -l  --list                  List all currently supported Ubuntu versions
    -c  --current               Print the current ubuntu version
    -h  --hash      RELEASE     Print the sha256 hash of desired release
*/
int main(int argc, char** argv) {
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        std::cout << "UbuntuDownloader Usage:\n";
        std::cout << "\t -h\t\t --help\t\t\t Prints this menu.\n";
        std::cout << "\t -l\t\t --list\t\t\t Lists all supported releases.\n";
        std::cout << "\t -c\t\t --current\t\t Lists the latest (current) Ubuntu LTS.\n";
        std::cout << "\t -H [VERSION]\t --Hash [VERSION]\t Prints the SHA256 hash of the desired version.\n";
        return 0;
    }

    std::shared_ptr<boost::asio::io_context> ioc =
        std::make_shared<boost::asio::io_context>();

    downloader::HttpClient client(ioc);

    HttpResponse response = client.GET(
        "cloud-images.ubuntu.com",
        "/releases/streams/v1/com.ubuntu.cloud:released:download.json");

    // -- Error retrieving JSON content --
    if (!response.has_content_length()) {
        return 1;
    }

    std::string string_response = beast::buffers_to_string(response.body().data());

    Parser parser = Parser(string_response);

    if (argc == 2) {
        if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--list") == 0) {
            // Print supported versions
            parser.printSupportedVersions();

        } else if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--current") == 0) {
            // Print the latest Ubuntu LTS version
            parser.printLatestLTSVersion();

        } else {
            std::cout << "[ERR] Invalid argument.\n";
            std::cout << "[INF] Please run UbuntuDownloader -h for a list of arguments.\n";
            return 1;
        }
    } else if (argc == 3) {
        if (strcmp(argv[1], "-H") == 0 || strcmp(argv[1], "--Hash") == 0) {
            // Print the hash of the desired release

            std::string desired_release(argv[2]);
            std::string subversion = "";

            size_t delimiter_pos = desired_release.find("-");
            if (delimiter_pos == std::string::npos) {
                std::cout << "[ERR] Please provide the Ubuntu version in the following format: [RELEASE]-[VERSION].\n";
                return 1;
            }

            subversion = desired_release.substr(delimiter_pos + 1, desired_release.length() - delimiter_pos - 1);
            desired_release = desired_release.substr(0, delimiter_pos);

            parser.printHashOfVersion(desired_release, subversion);
        } else {
            std::cout << "[ERR] Invalid argument.\n";
            std::cout << "[INF] Please run UbuntuDownloader -h for a list of arguments.\n";
            return 1;
        }
    } else {
        std::cout << "[ERR] Invalid number of arguments.\n";
        std::cout << "[INF] Please run UbuntuDownloader -h for a list of arguments.\n";
        return 1;
    }

    return 0;
}
