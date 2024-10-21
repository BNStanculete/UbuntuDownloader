// Copyright HVortex 2024.

#include "Parser.hpp"

#include <json/json.h>

#include <iostream>
#include <string>

Parser::Parser(const std::string& jsonstring) {
    std::istringstream stringStream(jsonstring);
    std::string errors = "";

    Json::CharReaderBuilder builder;

    if (!Json::parseFromStream(builder, stringStream, &root, &errors)) {
        std::cout << "[ERR] Could not parse JSON object received from remote server.\n";
        std::cout << "[ERR] Error: " << errors << "\n";
        return;
    }

    if (!root.isMember("products")) {
        std::cout << "[ERR] Could not locate products in JSON file.\n";
        return;
    }

    root = root["products"];
}

void Parser::printSupportedVersions() const {
    std::cout << "All supported Ubuntu releases:\n";

    for (const std::string& key : root.getMemberNames()) {
        if (key.rfind("amd64") == std::string::npos ||
            !traverseKeys(root, key, "supported").asBool()) {
            continue;
        }

        std::cout << "\t- " << traverseKeys(root, key, "release_title").asString() << "\n";
    }
}

void Parser::printLatestLTSVersion() const {
    std::string latest_version = "";
    std::string version = "";

    for (const std::string& key : root.getMemberNames()) {
        if (key.rfind("amd64") == std::string::npos ||
            !traverseKeys(root, key, "supported").asBool() ||
            traverseKeys(root, key, "release_title").asString().rfind("LTS") == std::string::npos) {
            continue;
        }

        version = traverseKeys(root, key, "version").asString();
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

                version1 = version1 * 10 + static_cast<uint32_t>(latest_version[index] - '0');
                version2 = version2 * 10 + static_cast<uint32_t>(version[index] - '0');
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

void Parser::printHashOfVersion(const std::string& release,
                                const std::string& version) const {
    std::cout << "Looking for release " << release << " version " << version << "\n";

    for (const std::string& key : root.getMemberNames()) {
        if (key.rfind("amd64") == std::string::npos ||
            traverseKeys(root, key, "version").asString().find(release) == std::string::npos) {
            continue;
        }
        std::cout << "Hash of version: " << traverseKeys(root, key, "release_title").asString() << ": ";

        Json::Value node = traverseKeys(root, key, "versions");

        if (!node.isMember(version)) {
            std::cout << "NON-EXISTENT VERSION.\n";
            return;
        }

        std::cout << traverseKeys(node, version, "items", "disk1.img", "sha256").asString() << "\n";
        break;
    }
}
