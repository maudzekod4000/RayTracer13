#include "FileReader.h"

#include <fstream>
#include <format>

// TODO: Add a alias for the std::expected type with string error.
std::expected<std::vector<char>, std::string> FileReader::readFile(const std::filesystem::path& filePath) noexcept
{
    std::ifstream sceneFile(filePath);

    if (!sceneFile) {
        return std::unexpected(std::format("Failed to open file: {}", std::filesystem::absolute(filePath).string()));
    }

    sceneFile.seekg(0, std::ios::end);
    const size_t fileSize = sceneFile.tellg();
    sceneFile.seekg(0, std::ios::beg);

    std::vector<char> buff(fileSize);

    sceneFile.read(buff.data(), fileSize);

    return buff;
}
