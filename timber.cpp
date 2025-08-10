#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>

// Pozycje pikseli i kliknięć (musisz dostosować do własnego ekranu)
const POINT PIXEL_LEFT = { 780, 550 };
const POINT PIXEL_RIGHT = { 1080, 550 };
const POINT PIXEL_CENTER = { 989, 681 };
const POINT PIXEL_LEFT_B = { 814, 636 };
const POINT PIXEL_RIGHT_B = { 1106, 636 };

int i = 0;

// Funkcja do pobrania koloru piksela
COLORREF getPixelColor(POINT pt) {
    HDC hdcScreen = GetDC(NULL);
    COLORREF color = GetPixel(hdcScreen, pt.x, pt.y);
    ReleaseDC(NULL, hdcScreen);
    return color;
}


// Symulacja naciśnięcia klawisza
void pressKey(int vk) {
    INPUT KEY_B;

    KEY_B.type = INPUT_KEYBOARD;
    KEY_B.ki.time = 0;
    KEY_B.ki.wVk = 0;
    KEY_B.ki.dwExtraInfo = 0;
    KEY_B.ki.dwFlags = KEYEVENTF_SCANCODE;
    if (vk)
    {
        KEY_B.ki.wScan = 0x20;
    }
    else KEY_B.ki.wScan = 0x21;

    SendInput(1, &KEY_B, sizeof(INPUT));
    Sleep((rand() % 15 + 1));
    KEY_B.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    SendInput(1, &KEY_B, sizeof(INPUT));
    KEY_B.ki.dwFlags = KEYEVENTF_SCANCODE;
}

void pressKeySrodek() {
    INPUT KEY_B;

    KEY_B.type = INPUT_KEYBOARD;
    KEY_B.ki.time = 0;
    KEY_B.ki.wVk = 0;
    KEY_B.ki.dwExtraInfo = 0;
    KEY_B.ki.dwFlags = KEYEVENTF_SCANCODE;
    KEY_B.ki.wScan = 0x20;

    SendInput(1, &KEY_B, sizeof(INPUT));
    Sleep((rand() % 15 + 1));
    pressKey(0);
    KEY_B.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    SendInput(1, &KEY_B, sizeof(INPUT));
    KEY_B.ki.dwFlags = KEYEVENTF_SCANCODE;

}

std::string checkBranch() {
    COLORREF rgb_left = getPixelColor(PIXEL_LEFT);
    COLORREF rgb_right = getPixelColor(PIXEL_RIGHT);
    COLORREF rgb_center = getPixelColor(PIXEL_CENTER);
    COLORREF rgb_left_b = getPixelColor(PIXEL_LEFT_B);
    COLORREF rgb_right_b = getPixelColor(PIXEL_RIGHT_B);

    std::cout << i++ << " " << (int)GetRValue(rgb_left);
    std::cout << " " << (int)GetRValue(rgb_right) << std::endl;

    if (GetGValue(rgb_center) < 100) {
        return "Center";
    }
    else if (GetGValue(rgb_left_b) > 240 && abs(GetRValue(rgb_left) - 74) >= 25) {
        return "Right";
    }
    else if (GetGValue(rgb_right_b) > 240 && abs(GetRValue(rgb_right) - 33) >= 25) {
        return "Left";
    }
    else if (abs(GetRValue(rgb_left) - 74) <= 24) {
        return "Left";
    }
    else if (abs(GetRValue(rgb_right) - 33) <= 24) {
        return "Right";
    }

    return "None";
}


int main() {
    std::cout << "Start za 2 sekundy..." << std::endl;
    Sleep(2000);

    bool lastPressedLeft = true;

    const int frame_interval_ms = 190;  // 1000 ms / 30 fps (czyli co 2 klatki z 60 fps)
    auto next_frame = std::chrono::steady_clock::now();

    while (!GetAsyncKeyState(VK_ESCAPE)) {
        next_frame += std::chrono::milliseconds(frame_interval_ms);
        std::string side = checkBranch();
        std::cout << side << std::endl;
        if (side == "Center") {
            pressKeySrodek();
        }
        else if (side == "Left") {
            pressKey(0);
            pressKey(0);
            lastPressedLeft = false;
        }
        else if (side == "Right") {
            pressKey(1);
            pressKey(1);
            lastPressedLeft = true;
        }
        else {
            pressKey(lastPressedLeft ? 1 : 0);
        }

          
        std::this_thread::sleep_until(next_frame);
        //std::cout << "Next frame: " << std::chrono::duration_cast<std::chrono::milliseconds>(
        //    next_frame.time_since_epoch()
        //    ).count() << " ms" << std::endl;
    }

    std::cout << "Zatrzymano program (ESC)." << std::endl;
    return 0;
}
