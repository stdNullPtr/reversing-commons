#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <optional>
#include "xor.hpp"

namespace commons::file
{
    inline std::optional<std::vector<char>> read_file(const std::filesystem::path& full_file_path)
    {
        if (!exists(full_file_path))
        {
            std::wcerr << XORW(L"Cannot open file: ") << full_file_path << '\n';
            return std::nullopt;
        }

        std::ifstream file(full_file_path, std::ios::binary | std::ios::ate);

        if (!file)
        {
            std::wcerr << XORW(L"Failed to open file: ") << full_file_path << '\n';
            return std::nullopt;
        }

        const std::streampos size = file.tellg();
        std::vector<char> buffer(static_cast<size_t>(size));
        file.seekg(0, std::ios::beg);
        file.read(buffer.data(), size);

        return buffer;
    }
}
