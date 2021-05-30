#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <cstddef>
#include "Hazel/Core/Base.h"

namespace Hazel {

	using File = std::vector<std::byte>;

	static File ReadBinaryFile(std::string location)
	{

		std::ifstream fs(location, std::ios::ate | std::ios::binary);
		
		if (!fs.is_open())
			HZ_CORE_ERROR("Failed to open file at location: {0}", location.c_str());

		size_t fileSize = (size_t)fs.tellg();

		File file(fileSize);

		fs.seekg(0);
		fs.read( (char*) file.data(), file.size());
		fs.close();

		return file;
	}

	// class FileSystem
	// {
	// public:
	// 	FileSystem(std::string defaultCacheDir);
	// 
	// 	std::string ReadTextFile(std::string path);
	// 	std::vector<std::byte> ReadFile(std::string path);
	// 
	// 	void WriteTextToFile(std::string path, std::string& text);
	// 	void WriteBinaryToFile(std::string path, std::vector<std::byte>& data);
	// 	void WriteBinaryToFile(std::string path, size_t size, std::byte* pData);
	// 
	// 	void CacheFile(size_t size, std::byte* pData);
	// 	void GetCachedFile(std::vector<std::byte>& data);
	// 
	// 	bool CheckIfModified(std::string path);
	// };


}
