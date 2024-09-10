#pragma once

#include <string>
#include <vector>
#include "Math/MathUtils.h"
#include "Math/Geometry.h"

#include <fstream>
#include <sstream>

namespace Utility {

    std::wstring ConvertString(const std::string& str);
    std::string ConvertString(const std::wstring& str);

    void Positions(const std::string& fileName, std::vector<Vector3>& positions);
    void Spheres(const std::string& fileName, std::vector<Math::Sphere>& spheres);

}