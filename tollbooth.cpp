#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>
#include<ctime>
namespace TollBoothSimulator{
struct car{};
struct truck{};
struct Bus{};
struct Jeep{};

template <typename VehicleType>
class TollBooth {
public:
    TollBooth() {} // Default constructor
    TollBooth(const std::string& id) : id(id) {}

    void registerVehicle(const std::string& plateNumber);
    void collectToll(const std::string& plateNumber, int entryBooth);
    void exitToll(const std::string& plateNumber, int exitBooth);
    ~TollBooth(){}
private:
    std::string id;
    std::map<std::string, double> tollAmounts;
    friend void displayDateTime();
};

void displayDateTime() {
    std::time_t now = std::time(nullptr);
    std::tm localTime = *std::localtime(&now);
    
    std::cout << "Current Date and Time: " << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << std::endl;
}


void displayMenu() {
    std::cout << "\n** Toll Booth Simulator **\n";
    std::cout << "1. Register a Vehicle\n";
    std::cout << "2. Collect Toll\n";
    std::cout << "3. Exit\n";
    std::cout << "Enter your choice: ";
}

bool isVehicleRegistered(const std::string& plateNumber) {
    std::ifstream inFile("registered_vehicles.txt");
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            if (line == plateNumber) {
                inFile.close();
                return true;
            }
        }
        inFile.close();
    }
    return false;
}
template <typename VehicleType>
void TollBooth<VehicleType>::registerVehicle(const std::string& plateNumber) {
    if (isVehicleRegistered(plateNumber)) {
        std::cout << "Vehicle with plate number " << plateNumber << " is already registered.\n";
        return;
    }

    std::ofstream outFile("registered_vehicles.txt", std::ios::app);
    if (outFile.is_open()) {
        outFile << plateNumber << "\n";
        std::cout << "Vehicle registered successfully!\n";
        outFile.close();
    } else {
        std::cout << "Error: Unable to open the file for writing.\n";
    }
}
template <typename VehicleType>
void TollBooth<VehicleType>::collectToll(const std::string& plateNumber,int entryBooth) {
    if (!isVehicleRegistered(plateNumber)) {
        std::cout << "Error: Vehicle not registered.\n";
        return;
    }

    std::ifstream inFile("registered_vehicles.txt");
    if (inFile.is_open()) {
        std::string line;
        bool found = false;

        while (std::getline(inFile, line)) {
            if (line == plateNumber) {
                found = true;
                break;
            }
        }

        inFile.close(); // Close the file after reading

        if (found) {
            int boothChoice;
            std::cout << "Choose a toll booth (1, 2, 3, 4): ";
            std::cin >> boothChoice;

            std::ifstream boothsFile("toll_booths.txt");
            if (boothsFile.is_open()) {
                std::vector<TollBooth> booths;
                std::string boothLine;

                while (std::getline(boothsFile, boothLine)) {
                    std::istringstream iss(boothLine);
                    TollBooth booth;
                    iss >> booth.id;

                    // Read toll amounts for each vehicle type for this booth
                    std::string vehicleType;
                    double tollAmount;
                    while (iss >> vehicleType >> tollAmount) {
                        booth.tollAmounts[vehicleType] = tollAmount;
                    }

                    booths.push_back(booth);
                }

                boothsFile.close();

                if (boothChoice > 0 && boothChoice <= booths.size()) {
                    TollBooth selectedBooth = booths[boothChoice - 1];

                    std::string vehicleType;
                    std::cout << "Enter the vehicle type (car, truck, Bus, Jeep): ";
                    std::cin >> vehicleType;

                    double tollAmount = selectedBooth.tollAmounts[vehicleType];
                    if (tollAmount != 0) {
                        std::cout << "Toll amount for booth " << selectedBooth.id << " and vehicle type " << vehicleType << " is Rs " << std::fixed << std::setprecision(2) << tollAmount << std::endl;

                      
                       
                        std::ofstream outFile(("toll_collection.txt"), std::ios::app);

                        if (outFile.is_open()) {
                            outFile << plateNumber << " " << selectedBooth.id << " " << vehicleType << " " << std::fixed << std::setprecision(2) << tollAmount << "\n";
                            std::cout << "Toll collected successfully!\n";
                            outFile.close(); // Close the file after writing
                        } else {
                            std::cout << "Error: Unable to open the file for writing.\n";
                        }
                    } else {
                        std::cout << "Error: Invalid vehicle type for this booth.\n";
                    }
                } else {
                    std::cout << "Error: Invalid toll booth choice.\n";
                }
            } else {
                std::cout << "Error: Unable to open the file for reading toll booths.\n";
            }
        } else {
            std::cout << "Error: Vehicle not registered.\n";
        }
    } else {
        std::cout << "Error: Unable to open the file for reading registered vehicles.\n";
    }
}
}

int main() {
   TollBoothSimulator::displayDateTime();
    int choice;
   TollBoothSimulator::TollBooth<TollBoothSimulator::car> cartollBooth;
   TollBoothSimulator::TollBooth<TollBoothSimulator::truck> trucktollBooth;
   TollBoothSimulator::TollBooth<TollBoothSimulator::Bus> BustollBooth;
   TollBoothSimulator::TollBooth<TollBoothSimulator::Jeep> JeeptollBooth;
    int entryBooth;

    do {
       TollBoothSimulator::displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string plateNumber;
                std::cout << "Enter the vehicle's license plate number: ";
                std::cin >> plateNumber;
                cartollBooth.registerVehicle(plateNumber);
                break;
            }
            case 2: {
                std::string plateNumber;
                std::cout << "Enter the vehicle's license plate number: ";
                std::cin >> plateNumber;
                std::cout << "Choose an entry toll booth (1, 2, 3, 4): ";
                std::cin >> entryBooth;
                cartollBooth.collectToll(plateNumber,entryBooth);
                break;
            }
            case 3:
                std::cout << "Exiting the Toll Booth Simulator.\n";
                break;
            case 4:
                std::cout << "Exiting the Toll Booth Simulator.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}