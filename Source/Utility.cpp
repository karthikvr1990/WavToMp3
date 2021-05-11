#include "Utility.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>


#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <dirent.h>
#endif

using namespace Utils;
using namespace std;


		
	std::vector<std::string> Utility::GetFilesInDirectory(std::string path)
	{
#ifdef _WIN32
		return GetFilesInDirectoryWindows(path);
#else 
		return GetFilesInDirectoryLinux(path);
#endif
	}

#ifdef _WIN32
	std::vector<std::string> Utility::GetFilesInDirectoryWindows(std::string path)
	{
		std::vector<std::string> files;
		std::string filePathPrefix = path + "\\";
		path.append("\\*");
		
		WIN32_FIND_DATA fileData;
		HANDLE hFind;
		if ((hFind = FindFirstFile(path.c_str(), &fileData)) != INVALID_HANDLE_VALUE) {
			do {
				files.push_back(filePathPrefix + fileData.cFileName);
			} while (FindNextFile(hFind, &fileData) != 0);
			FindClose(hFind);
		}

		return files;
	}
#else
	std::vector<std::string> Utility::GetFilesInDirectoryLinux(std::string path)
	{
		path += '/';
		std::vector<std::string> files;
		DIR* dirp = opendir(path.c_str());
		struct dirent* dp;
		while ((dp = readdir(dirp)) != NULL) {
			files.push_back(path + dp->d_name);
		}
		closedir(dirp);

		return files;
	}
#endif

	std::vector<std::string> Utility::GetFilesWithMatchingExtension(std::vector<std::string> files, std::string extension)
	{
		std::vector<std::string> filesWithMatchingExtension;
		int extensionSize = extension.size();

		for (auto &file : files)
		{			
			if(file.size() > extensionSize && file.substr(file.size() - extensionSize) == extension)
			{
				filesWithMatchingExtension.push_back(file);
			}
		}

		return filesWithMatchingExtension;

	}



	bool Utility::IsBigEndian()
	{
		int num = 1;
		char* c = (char*)&num;

		if (*c)
		{
			return false;
		}
		else
		{
			return true;
		}
		
		return true;

	}


	unsigned long long  Utility::GetStreamSize(std::istream& is)
	{
		std::streampos pos = is.tellg();
		is.seekg(0, std::ios::end);
		unsigned long long len = is.tellg() - pos;
		is.seekg(pos);
		return len;
	}

