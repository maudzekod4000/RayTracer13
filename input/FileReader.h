#ifndef FILE_READER_H
#define FILE_READER_H

#include <filesystem>
#include <expected>
#include <string>
#include <vector>

class FileReader final {
public:
	static std::expected<std::vector<char>, std::string> readFile(const std::filesystem::path& filePath) noexcept;
	FileReader() = delete;
};

#endif // !FILE_READER_H
