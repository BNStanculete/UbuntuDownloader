// Copyright HVortex 2024.

#include "functions.hpp"
#include <json/json.h>
#include <string>

void printSupportedVersions(const Json::Value& root) {
    std::cout << "All supported Ubuntu releases:\n";

    for (const std::string& key : root.getMemberNames()) {
        if (key.rfind("amd64") == std::string::npos ||
            !traverseKeys(root, key, "supported").asBool()) {
            continue;
        }

        std::cout << "\t- " << traverseKeys(root, key, "release_title").asString() << "\n";
    }
}

void printLatestLTSVersion(const Json::Value& root) {
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

void printHashOfNode(const Json::Value& root,
                     const std::string& release,
                     const std::string& version) {
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
