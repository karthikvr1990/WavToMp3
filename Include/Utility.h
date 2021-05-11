#ifndef UTILITY_H
#define UTILITY_H


#include <string>
#include <vector>

namespace Utils {


	class Utility {
	public:
		static std::vector<std::string> GetFilesInDirectory(std::string path);
		static std::vector<std::string> GetFilesWithMatchingExtension(std::vector<std::string> files, std::string extension);
		static bool IsBigEndian();
		static unsigned long long  GetStreamSize(std::istream& is);
	private:
#ifdef _WIN32
		static std::vector<std::string> GetFilesInDirectoryWindows(std::string path);
#else
		static std::vector<std::string> GetFilesInDirectoryLinux(std::string path);
#endif
	};


} // namespace  Utility


#endif // UTILITY_H
