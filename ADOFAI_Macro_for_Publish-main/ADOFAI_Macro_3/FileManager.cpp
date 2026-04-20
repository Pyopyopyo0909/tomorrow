#include "FileManager.h"
#include <fstream>
#include <iostream>

bool FileManager::LoadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "파일 열기 실패\n";
        return false;
    }

    file >> jsonData;

    // 핵심 데이터 추출
    pathData = jsonData["pathData"].get<std::string>();

    bpm = jsonData["settings"]["bpm"].get<double>();
    offset = jsonData["settings"]["offset"].get<double>();

    // actions 파싱
    if (jsonData.contains("actions")) {
        actions = jsonData["actions"];
    }

    return true;
}