/*Task 2: Simulate an Instrument Cluster Data Display
Objective: Create a simplified data display system to simulate speed, fuel level, and engine temperature.
Requirements:
Implement a class-based design:
Create a VehicleData class to store parameters like speed, fuel level, and engine temperature.
Create a Display class to show these parameters on the console.
Simulate real-time updates:
Use a random number generator to update speed, fuel level, and temperature every second.
Display the updated data in a formatted manner (e.g., speed: 80 km/h, fuel: 50%, temperature: 90°C).
Add constraints:
Display warnings when parameters exceed thresholds (e.g., temperature > 100°C or fuel < 10%).
Deliverables:
A C++ program using multithreading (std::thread) to update and display data.
Output showcasing real-time updates and warnings.*/
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <mutex>
#include <iomanip>

class VehicleData {
private:
    double speed;
    double fuelLevel;
    double engineTemperature;
    std::mutex dataMutex; 

public:
    VehicleData() : speed(0.0), fuelLevel(5.0), engineTemperature(90.0) {}

    void updateData() {
        std::lock_guard<std::mutex> lock(dataMutex);
        speed = getRandomValue(0, 200);
        fuelLevel = std::max(0.0, fuelLevel - getRandomValue(0, 1));
        engineTemperature = getRandomValue(70, 120);
    }

    void getData(double& currentSpeed, double& currentFuel, double& currentTemp) {
        std::lock_guard<std::mutex> lock(dataMutex);
        currentSpeed = speed;
        currentFuel = fuelLevel;
        currentTemp = engineTemperature;
    }

private:
    double getRandomValue(double min, double max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }
};

class Display {
public:
    static void showData(VehicleData& vehicleData) {
        while (true) {
            double speed, fuelLevel, engineTemperature;
            vehicleData.getData(speed, fuelLevel, engineTemperature);
            system("clear");
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "==== Instrument Cluster ====" << std::endl;
            std::cout << "Speed: " << speed << " km/h" << std::endl;
            std::cout << "Fuel Level: " << fuelLevel << " %" << std::endl;
            std::cout << "Engine Temperature: " << engineTemperature << " °C" << std::endl;

            if (fuelLevel < 10) {
                std::cout << "WARNING: Low fuel level!" << std::endl;
            }
            if (engineTemperature > 100) {
                std::cout << "WARNING: Engine overheating!" << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

void updateVehicleData(VehicleData& vehicleData) {
    while (true) {
        vehicleData.updateData();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    VehicleData vehicleData;
    std::thread dataUpdater(updateVehicleData, std::ref(vehicleData));
    std::thread dataDisplayer(Display::showData, std::ref(vehicleData));

    dataUpdater.join();
    dataDisplayer.join();

    return 0;
}