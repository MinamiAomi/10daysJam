#include "Utility.h"

#include <Windows.h>

namespace Utility {

    std::wstring ConvertString(const std::string& str) {
		if (str.empty()) {
			return std::wstring();
		}
		int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
		if (sizeNeeded == 0) {
			return std::wstring();
		}
		std::wstring result(sizeNeeded, 0);
		MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
		return result;
    }

    std::string ConvertString(const std::wstring& str) {
		if (str.empty()) {
			return std::string();
		}
		int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
		if (sizeNeeded == 0) {
			return std::string();
		}
		std::string result(sizeNeeded, 0);
		WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
		return result;
    }

	void Positions(const std::string& fileName, std::vector<Vector3>& positions)
	{
		std::string directoryPath = "Resources/";
		std::string filePath = directoryPath + fileName;
		std::ifstream file(filePath);

		std::string line;

		if (!file.is_open()) {
			assert(false);
		}

		while (std::getline(file, line)) {
			std::stringstream ss(line);
			char comma;
			float x, y, z;

			if (ss >> x >> comma >> y >> comma >> z) {
				positions.emplace_back(Vector3{ x, y, z });
			}
		}

		file.close();
	}

	void Spheres(const std::string& fileName, std::vector<Math::Sphere>& spheres)
	{
		std::string directoryPath = "Resources/";
		std::string filePath = directoryPath + fileName;
		std::ifstream file(filePath);

		std::string line;

		if (!file.is_open()) {
			assert(false);
		}

		while (std::getline(file, line)) {
			std::stringstream ss(line);
			char comma;
			float x, y, z;
			float radius;

			if (ss >> x >> comma >> y >> comma >> z >> comma >> radius) {
				spheres.emplace_back(Vector3{ x, y, z }, radius);
			}
		}

		file.close();
	}

}