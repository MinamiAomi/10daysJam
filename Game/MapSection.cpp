#include "MapSection.h"

#include <cassert>
#include <fstream>
#include <sstream>

void MapSection::Load(const std::filesystem::path& path) {
    std::ifstream file(path);
    assert(file.is_open());

    std::string line;
    while (std::getline(file, line)) {
        auto& rowData = tileData_.emplace_back();

        std::istringstream iss(line);
        std::string data;
        for (uint32_t i = 0; i < MapProperty::kMapColumn; ++i) {
            if (std::getline(iss, data, ',')) {
                rowData[i] = static_cast<Tile::Enum>(std::stoi(data));
            }
            else {
                rowData[i] = Tile::Air;
            }
        }
    }

    file.close();

}
