#define _CRT_SECURE_NO_WARNINGS

#include "File.h"
#include "DirectoryNotFoundException.h"
#include <string>
#include <sstream>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>



using namespace std;

namespace fs = std::experimental::filesystem;

string getAppData() {
	char* appDataPath = nullptr; //Это сишная строка пути Appdata
	size_t size = 0;

	errno_t err = _dupenv_s(&appDataPath, &size, "APPDATA"); //Получаем код ошибки при получение окружения AppData

	if (err == 0 && appDataPath != nullptr) {
		string appDataPathStr = (string)appDataPath; //Здесь содержится путь к Roaming
		free(appDataPath);
		size_t pos = appDataPathStr.find("AppData"); //Находим позицию Appdata 
		return appDataPathStr.substr(0, pos + 7); //
	}
	else {
		return "";
	}
}

bool checkDirectory(const string& Path) {
	return   (fs::exists(Path) || fs::is_directory(Path));
}

void openFile(const string& Path) {
    ostringstream oss;
    string semicolon = "\"";
    oss << "start \"\" " <<  semicolon << Path << semicolon;
    system(oss.str().c_str());
}

vector<string> listFilesInDirectory(const std::string& directoryPath, bool Recursive) {
    vector<string> paths;
    try {
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (fs::is_regular_file(entry.path())) {
                paths.push_back(entry.path().string());
            }
            else if (Recursive) {
                vector<string> newPaths = listFilesInDirectory(entry.path().string(), Recursive);
                paths.insert(paths.end(), newPaths.begin(), newPaths.end());
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        return paths;
    }
    return paths;
}

const char* concat_c(const char* str1, const char* str2) {
    size_t length1 = strlen(str1);
    size_t length2 = strlen(str2);
    size_t total_length = length1 + length2 + 1; // +1 для нулевого символа
    char* result = (char*)malloc(total_length);
        if (result == NULL)
            return "";
    strcpy(result, str1); 
    strcat(result, str2); 

    return result;
}


void filesPathLogResult::pushSuccefullFilePath(const string& Dir) {
    succefullyFiles.push_back(Dir);
}

void filesPathLogResult::pushFailFilePath(const string& Dir, const char* error) {
    failedFiles.push_back(Dir + ": " + string(error));
}

vector<string> filesPathLogResult::getSuccefullyFiles() {
    return succefullyFiles;
}

vector<string> filesPathLogResult::getFailedFiles() {
    return failedFiles;
}

vector<string> splitString(const string& str) {
    vector<string> result;
    istringstream iss(str);
    string word;

    while (iss >> word) {
        result.push_back(word);
    }

    return result;
}

bool copyFile(const std::string& file, const std::string& destDir) {
    if (!fs::is_regular_file(file))
        return false;

    int index = destDir.find_last_of("\\");

    if (index != string::npos) {
        string dir = destDir.substr(0, index);

        if (!fs::exists(destDir)) {
            fs::create_directory(destDir);
        }
    }
    try
    {
        fs::copy(file, destDir, fs::copy_options::overwrite_existing | fs::copy_options::recursive); 
    }
    catch (const fs::filesystem_error& e)
    {
        return false;
    }
    return true;
}

filesPathLogResult* copyFiles(const std::string& sourceDir, const std::string& destDir,bool Recursive) {

    filesPathLogResult* logResult = new filesPathLogResult();

    if (!fs::exists(sourceDir) || !fs::is_directory(sourceDir)) {
        throw DirectoryNotFoundException(sourceDir);
        return logResult;
    }

    vector<string> paths = listFilesInDirectory(sourceDir,Recursive); //Эта функция может получать подкатологи путей файлов
    for (const string& path : paths) {
        string dest = destDir + "\\" + path.substr(path.find_last_of("\\"));
        try
        {
            fs::copy(path, dest, fs::copy_options::overwrite_existing | fs::copy_options::recursive);
            logResult->pushSuccefullFilePath(dest);
        }
        catch (const fs::filesystem_error& e)
        {
            logResult->pushFailFilePath(dest, e.what());
        }
    }
    return logResult;
}