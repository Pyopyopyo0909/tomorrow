#include "BeatManager.h"
#include <windows.h>
#include <cmath>

// -------------------------
// 각도 변환
// -------------------------
double BeatManager::CharToAngle(char c) {
    switch (c) {
    case 'R': return 0;
    case 'p': return 15;
    case 'U': return 90;
    case 'L': return 180;
    case 'D': return 270;
    case 'E': return 45;
    case 'Q': return 135;
    case 'Z': return 225;
    case 'C': return 315;
    default: return 0;
    }
}

// -------------------------
// 비트 계산
// -------------------------
double BeatManager::CalcBeat(double prev, double curr) {
    double diff = fmod((curr - prev + 360.0), 360.0);
    return diff / 180.0;
}

// -------------------------
// 키 입력
// -------------------------
void BeatManager::SendKey() {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = VK_SPACE;

    SendInput(1, &input, sizeof(INPUT));

    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

// -------------------------
// 정밀 슬립
// -------------------------
void BeatManager::PreciseSleep(double ms) {
    LARGE_INTEGER freq, start, now;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    double elapsed = 0;
    while (elapsed < ms) {
        QueryPerformanceCounter(&now);
        elapsed = (double)(now.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
    }
}

// -------------------------
// 메인 실행
// -------------------------
void BeatManager::Run(FileManager& file) {

    double bpm = file.bpm;
    double offset = file.offset;

    std::vector<double> angles;

    // pathData → 각도
    for (char c : file.pathData) {
        angles.push_back(CharToAngle(c));
    }

    PreciseSleep(offset);

    for (int i = 1; i < angles.size(); i++) {

        // BPM 변경 처리
        for (auto& act : file.actions) {
            if (act["floor"] == i &&
                act["eventType"] == "SetSpeed") {

                if (act.contains("bpm"))
                    bpm = act["bpm"];
            }
        }

        double beat = CalcBeat(angles[i - 1], angles[i]);

        double delay = beat * (60000.0 / bpm);

        PreciseSleep(delay);

        SendKey();
    }
}