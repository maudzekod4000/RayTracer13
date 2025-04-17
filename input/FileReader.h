#ifndef FILE_READER_H
#define FILE_READER_H

#include <filesystem>
#include <expected>
#include <string>

class FileReader final {
public:
	FileReader() = delete;
	static std::expected<std::vector<char>, std::string> readFile(const std::filesystem::path& filePath) noexcept;
};

#endif // !FILE_READER_H
