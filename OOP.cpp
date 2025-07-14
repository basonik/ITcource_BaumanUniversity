#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

const char DefaultVehicleName[] = "Untyped vehicle";
const char DefaultCoachName[] = "Default Coach";
const char DefaultAutomobileName[] = "Default Automobile";
const char DefaultAeroplaneName[] = "Default Aeroplane";
const double DefaultVehicleSpeed = -1.0;
const double DefaultCoachSpeed = 10.0;
const double DefaultAutomobileSpeed = 100.0;
const double DefaultAeroplaneSpeed = 500.0;
const double DefaultTimeToBoot = 0.01;

class Vehicle {
public:
    Vehicle() : totalDistance(0), totalTime(0), baseSpeed(DefaultVehicleSpeed) {
        SetName(DefaultVehicleName);
    }

    //Параметризованый конструтор ТС 
    Vehicle(const char inNameCString[], double inBaseSpeed, unsigned order) : totalDistance(0),
        totalTime(0), baseSpeed(inBaseSpeed), order(order) {
        SetName(inNameCString);
    }

    virtual ~Vehicle() {
    }

    //Метод получения информации об имени ТС 
    const char* const GetName() const {
        return nameCString;
    }

    //Метод получения информации о скорости при покупке 
    double GetBaseSpeed() const {
        return baseSpeed;
    }

    //Метод получения информации о текущем пробеге 
    double GetTotalDistance() const {
        return totalDistance;
    }

    double GetOrder() const {
        return order;
    }

    //Метод получения информации о текущем времени эксплуатации 
    double GetTotalTime() const {
        return totalTime;
    }

    //Абстрактный метод совершения поездки 
    //Возвращает на true, если поезка удачна и false, если случилась авария. 
    virtual bool MakeTrip(double distance) = 0;

    //Абстрактный метод получения информации о текущей скорости 
    virtual double GetSpeed() const = 0;

    //Оператор сравнения по умолчанию 
    bool operator< (Vehicle& rhs) const {
        if (GetSpeed() < rhs.GetSpeed()) {
            return true;
        }
        return false;
    }

    //Константа - общее максимальное количество символов в имени ТС 
    static const int MAX_NAME_LENGTH = 50;

protected:
    double totalDistance;
    double totalTime;
    unsigned order;


private:
    void SetName(const char inNameCString[]) {
        int i = 0;
        for (i = 0; (inNameCString[i] != 0) && (i < MAX_NAME_LENGTH); ++i) {
            nameCString[i] = inNameCString[i];
        }
        nameCString[i] = 0;
    }

    char nameCString[MAX_NAME_LENGTH];
    double baseSpeed;//теперь не статическая
};

class Battery {
public:
    Battery(double capacity, unsigned maxRecharges)
        : capacity(capacity), currentCharge(capacity), rechargesRemaining(maxRecharges) {
    }

    bool Charge(double amount) {
        if (rechargesRemaining > 0 && currentCharge < capacity) {
            currentCharge += amount;
            if (currentCharge > capacity) {
                currentCharge = capacity;
            }
            --rechargesRemaining;
            return true;
        }
        return false; 
    }

    void Discharge(double amount) {
        if (currentCharge >= amount) {
            currentCharge -= amount;
        }
        else {
            currentCharge = 0;
        }
    }

    double GetCurrentCharge() const {
        return currentCharge;
    }

    bool IsEmpty() const {
        return currentCharge <= 0;
    }

private:
    double capacity;
    double currentCharge;
    unsigned rechargesRemaining;
};

class ElectricVehicle : public Vehicle {
public:
    ElectricVehicle(const char inNameCString[], double inBaseSpeed, unsigned order,
        double batteryCapacity, unsigned maxRecharges)
        : Vehicle(inNameCString, inBaseSpeed, order), battery(batteryCapacity, maxRecharges) {
    }

    bool MakeTrip(double distance) override {
        double requiredCharge = distance * energyConsumption;
        if (battery.IsEmpty()) {
            return false;
        }
        battery.Discharge(requiredCharge);
        totalDistance += distance;
        return true;
    }

    double GetSpeed() const override {
        return GetBaseSpeed();
    }

private:
    Battery battery;
    static const double energyConsumption;
};

class FuelTank {
public:
    FuelTank(double capacity) : capacity(capacity), currentFuel(0) {}

    bool Refuel(double amount) {
        if (currentFuel + amount <= capacity) {
            currentFuel += amount;
            return true;
        }
        return false;
    }

    double GetCurrentFuel() const {
        return currentFuel;
    }

    bool IsEmpty() const {
        return currentFuel <= 0;
    }

private:
    double capacity;
    double currentFuel;
};

class GasolineVehicle : public Vehicle {
public:
    GasolineVehicle(const char inNameCString[], double inBaseSpeed, unsigned order, double fuelTankCapacity)
        : Vehicle(inNameCString, inBaseSpeed, order), fuelTank(fuelTankCapacity) {
    }

    bool MakeTrip(double distance) override {
        double requiredFuel = distance * fuelConsumption;
        if (fuelTank.IsEmpty()) {
            return false;
        }
        fuelTank.Refuel(requiredFuel);
        totalDistance += distance;
        return true;
    }

    double GetSpeed() const override {
        return GetBaseSpeed();
    }

private:
    FuelTank fuelTank;
    static const double fuelConsumption;
};

class Computer {
public:
    Computer() : baseTimeToBoot(DefaultTimeToBoot) {}

    double GetTimeToBoot() { //возвращает время загрузки компьютера 
        return baseTimeToBoot;
    }

protected:
    double baseTimeToBoot;
};

class Aeroplane : public Vehicle, private Computer {
public:
    Aeroplane(const char inNameCString[] = DefaultAeroplaneName, double inBaseSpeed = DefaultAeroplaneSpeed,
        unsigned order = 0) : Vehicle(inNameCString, inBaseSpeed, order) {
    }
    void ComputerUpdate(double newtime) {
        baseTimeToBoot = newtime;
    }
    double GetTimeToBoot() {
        return baseTimeToBoot;
    }
    void Repair() {
        timeSinceLastRepair = 0;
    }
    double GetTimeSinceLastRepair() const {
        return timeSinceLastRepair;
    }

    virtual bool MakeTrip(double distanceOfTrip) {
        double timeOfTrip = distanceOfTrip / GetSpeed() + GetTimeToBoot();
        if (timeSinceLastRepair + timeOfTrip > MAX_FLY_TIME) {
            return false;
        }
        timeSinceLastRepair += timeOfTrip;
        totalDistance += distanceOfTrip;
        totalTime += timeOfTrip;
        return true;
    }

    static const int MAX_FLY_TIME = 100;

    double GetSpeed() const {
        return GetBaseSpeed();
    }

private:
    int timeSinceLastRepair = 0;
};

class Automobile : public Vehicle {
public:
    Automobile(const char inNameCString[] = DefaultAutomobileName, double inBaseSpeed = DefaultAutomobileSpeed,
        unsigned order = 0) : Vehicle(inNameCString, inBaseSpeed, order) {
    }
    void Repair() {
        totalDistance = 0;
    }
    double GetDistanceSinceLastRepair() const {
        return totalDistance;
    }
    virtual bool MakeTrip(double distanceOfTrip) {
        double timeOfTrip = distanceOfTrip / GetSpeed();
        double distance = distanceOfTrip + GetDistanceSinceLastRepair();
        if (distance > MAX_DISTANCE) {
            return false;
        }
        totalDistance += distanceOfTrip;
        totalTime += timeOfTrip;
        return true;
    }
    static const int MAX_DISTANCE = 10000;
    double GetSpeed() const override{
        return GetBaseSpeed() * exp(-totalDistance / 500.0);
    }
};

void CommitRandomTrips(vector<Vehicle*>& vehicles) {
    for (auto vehicle : vehicles) {
        double randomDistance = double(rand() % 20001) / 10.0;
        vehicle->MakeTrip(randomDistance);
    }
}

void DisplayVehicles(const vector<Vehicle*>& vehicles) {
    cout << setw(20) << left << "Name"
        << setw(10) << "Speed"
        << setw(10) << "Distance"
        << setw(10) << "Time" << endl;

    for (const auto& vehicle : vehicles) {
        cout << setw(20) << left << vehicle->GetName()
            << setw(10) << vehicle->GetSpeed()
            << setw(10) << vehicle->GetTotalDistance()
            << setw(10) << vehicle->GetTotalTime() << endl;
    }
}

class Coach : public Vehicle {
public:
    static const double MAX_DISTANCE;
    Coach(const char inNameCString[] = DefaultAutomobileName, double inBaseSpeed = DefaultAutomobileSpeed,
        unsigned order = 0) : Vehicle(inNameCString, inBaseSpeed, order) {}

    bool MakeTrip(double distance) override {
        if (distance > MAX_DISTANCE) {
            return false;
        }
        totalDistance += distance;
        totalTime += distance / GetSpeed();
        return true;
    }

    double GetSpeed() const override {
        return GetBaseSpeed() * exp(-totalTime / 500.0);
    }
};

const double Coach::MAX_DISTANCE = 100.0;

template <class MyType>
void MySwap(MyType& v1, MyType& v2) {
    MyType v3 = v1;
    v1 = v2;
    v2 = v3;
}

template <class ArrayType, class LessFunctionType>
int FindMinimumIndex(ArrayType& data_array, int beginIndex, int endIndex, LessFunctionType LessFunction) {
    int minimumIndex = beginIndex;
    for (int element_number = beginIndex + 1; element_number <= endIndex; ++element_number) {
        if (LessFunction(data_array[element_number], data_array[minimumIndex])) {
            minimumIndex = element_number;
        }
    }
    return minimumIndex;
}

template <class ArrayType, class LessFunctionType>
void SelectionSort(ArrayType& data_array, int beginIndex, int endIndex, LessFunctionType LessFunction) {
    for (int element_number = beginIndex; element_number < endIndex; ++element_number) {
        int minimumIndex = FindMinimumIndex(data_array, element_number, endIndex, LessFunction);
        MySwap(data_array[minimumIndex], data_array[element_number]);
    }
}

template <class ArrayType, class LessFunctionType>
void InsertionSort(ArrayType& data_array, int beginIndex, int endIndex, LessFunctionType LessFunction) {
    for (int i = beginIndex + 1; i < endIndex + 1; i++) {
        for (int j = i; j > 0; j--) {
            if (LessFunction(data_array[j], data_array[j - 1])) {
                MySwap(data_array[j - 1], data_array[j]);
            }
        }
    }
}

bool CompareTime(Vehicle* lhs, Vehicle* rhs) {
    return lhs->GetTotalTime() < rhs->GetTotalTime();
}

bool CompareSpeed(Vehicle* ch, Vehicle* ch2) {
    return ch->GetSpeed() < ch2->GetSpeed();
}

bool CompareOrder(Vehicle* ch, Vehicle* ch2) {
    return ch->GetOrder() < ch2->GetOrder();
}

int main() {
    srand(0);

    vector<Coach> coaches;
    vector<Automobile> automobiles;
    vector<Aeroplane> aeroplanes;
    
    coaches.emplace_back("Coach 1", 9.0);
    coaches.emplace_back("Coach 2", 11.0);
    automobiles.emplace_back("Automobile 1", 90.0);
    automobiles.emplace_back("Automobile 2", 120.0);
    aeroplanes.emplace_back("Aeroplane 1", 1030.0);
    aeroplanes.emplace_back("Aeroplane 2", 560.0);

    vector<Vehicle*> vehiclePointers;

    for (auto& coach : coaches) {
        vehiclePointers.push_back(&coach);
    }
    for (auto& automobile : automobiles) {
        vehiclePointers.push_back(&automobile);
    }
    for (auto& aeroplane : aeroplanes) {
        vehiclePointers.push_back(&aeroplane);
    }

    //совершение случайных поездок
    CommitRandomTrips(vehiclePointers);

    //отображение информации о ТС
    cout << "All Vehicles After Random Trips:" << endl;
    DisplayVehicles(vehiclePointers);

    //сортировка по скорости
    SelectionSort(vehiclePointers, 0, vehiclePointers.size() - 1, CompareSpeed);

    cout << "\nAll Vehicles Sorted by Speed:" << endl;
    DisplayVehicles(vehiclePointers);
}