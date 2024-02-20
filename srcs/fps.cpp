//
// Created by pirexa on 16-02-2024.
//
#include <iostream>
#include <chrono>

void    fpsCounter(std::chrono::time_point<std::chrono::high_resolution_clock> &lastFPSTime, int &frameCount, double &fps)
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    double deltaFPSMeasureTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastFPSTime).count();
    frameCount++;
    if (deltaFPSMeasureTime >= 1e6) {
        // Calculate FPS
        fps = frameCount / deltaFPSMeasureTime * 1e6;

        // Print FPS
//        std::cout << "FPS: " << fps << std::endl;

        // Reset counters
        frameCount = 0;
        lastFPSTime = currentTime;
    }
}
