// Copyright 2024 HV0rtex

#include <json/json.h>
#include <iostream>
#include <memory>
#include <string>
#include "httpclient.hpp"

/*
Possible arguments from the command line:
    -l  --list                  List all currently supported Ubuntu versions
    -c  --current               Print the current ubuntu version
    -h  --hash      RELEASE     Print the sha256 hash of desired release
*/

int main(int argc, char** argv) {
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
    std::istringstream stringStream(string_response);
    std::string errors = "";

    Json::Value root;
    Json::CharReaderBuilder builder;

    if (!Json::parseFromStream(builder, stringStream, &root, &errors)) {
        std::cout << "[ERR] Could not parse JSON object received from remote server.\n";
        std::cout << "[ERR] Error: " << errors << "\n";
        return 1;
    }

    if (argc == 2) {
        if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--list") == 0) {
            // Print supported versions

            std::cout << "All supported Ubuntu releases:\n";
            for (const std::string& key : root["products"].getMemberNames()) {
                if (key.rfind("amd64") == std::string::npos ||
                    !root["products"][key]["supported"].asBool()) {
                    continue;
                }

                std::cout << "\t- " << root["products"][key]["release_title"] << "\n";
            }
        } else if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--current") == 0) {
            // Print the latest Ubuntu LTS version

            std::string latest_version = "";
            std::string version = "";
            for (const std::string& key : root["products"].getMemberNames()) {
                if (key.rfind("amd64") == std::string::npos ||
                    !root["products"][key]["supported"].asBool() ||
                    root["products"][key]["release_title"].asString().rfind("LTS") == std::string::npos) {
                    continue;
                }

                version = root["products"][key]["version"].asString();
                if (latest_version == "") {
                    latest_version = version;
                } else {
                    // Compare the two versions
                    // ---
                    // For that we assume that the versions are split into subversions
                    // and that the MAJOR subversion is first, followed by MINOR , PATCH, etc.

                    uint32_t version1 = 0, version2 = 0;
                    uint32_t index = 0;

                    while (index < latest_version.length()) {
                        if (latest_version[index] == '.') {
                            // Compare current subversion

                            if (version[index] != '.' || version1 < version2) {
                                // Definetely found version is newer, as subversion is biggere.
                                latest_version = version;
                                break;
                            }

                            if (version1 > version2) {
                                // Current subversion is greater, no need to proceed further.
                                break;
                            }

                            version1 = 0;
                            version2 = 0;
                        }

                        if (index >= version.length() || version[index] == '.') {
                            // We have a number in latest version and end of string / terminator
                            // in current version.

                            // as such latest version is newer
                            break;
                        }

                        version1 = version1 * 10 + (uint32_t)(latest_version[index] - '0');
                        version2 = version2 * 10 + (uint32_t)(version[index] - '0');
                        index += 1;
                    }

                    if (latest_version != version && index < version.length()) {
                        // We didn't update the latest version
                        // and we still have characters in current version
                        latest_version = version;
                    }
                }
            }

            std::cout << "Latest Ubuntu LTS version: " << latest_version << "\n";
        }
    } else if (argc == 3) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--hash") == 0) {
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

            std::cout << "Looking for release " << desired_release << " version " << subversion << "\n";

            for (const std::string& key : root["products"].getMemberNames()) {
                if (key.rfind("amd64") == std::string::npos ||
                    root["products"][key]["version"].asString().find(desired_release) == std::string::npos) {
                    continue;
                }

                std::cout << "Hash of version: " << root["products"][key]["release_title"].asString() << ": ";

                Json::Value node = root["products"][key]["versions"];

                if (!node.isMember(subversion)) {
                    std::cout << "NON-EXISTENT VERSION.\n";
                    return 0;
                }

                std::cout << node[subversion]["items"]["disk1.img"]["sha256"].asString() << "\n";
            }
        }
    } else {
        std::cout << "[ERR] Invalid number of arguments.\n";
        return 1;
    }

    return 0;
}
