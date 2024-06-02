#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

using namespace std;
namespace fs = std::experimental::filesystem;

string getAppData();

bool checkDirectory(const string& Path);

vector<string> listFilesInDirectory(const std::string& directoryPath, bool Recursive = false);

class filesPathLogResult {
private:
    std::vector<std::string> succefullyFiles;
    std::vector<std::string> failedFiles;

public:
    void pushSuccefullFilePath(const std::string& Dir);
    void pushFailFilePath(const std::string& Dir, const char* error);
    std::vector<std::string> getSuccefullyFiles();
    std::vector<std::string> getFailedFiles();
};

filesPathLogResult* copyFiles(const std::string& sourceDir, const std::string& destDir, bool Recursive = false);

const char* concat_c(const char* str1, const char* str2);


