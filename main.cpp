#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <fstream>

using namespace std;

class Appliance {
private:
    string name;
    double powerRating;
    double usageHours;

public:
    Appliance() {
        name = "";
        powerRating = 0;
        usageHours = 0;
    }

    Appliance(string n, double p, double u) {
        name = n;
        powerRating = p;
        usageHours = u;
    }

    string getName() const { return name; }
    double getPower() const { return powerRating; }
    double getUsage() const { return usageHours; }

    double calculateEnergy() const {
        return (powerRating * usageHours) / 1000.0;
    }

    void display() const {
        cout << left << setw(15) << name
             << setw(12) << powerRating
             << setw(12) << usageHours
             << setw(12) << fixed << setprecision(2)
             << calculateEnergy() << endl;
    }

    string toFileString() const {
        return name + "," + to_string(powerRating) + "," + to_string(usageHours);
    }
};

vector<Appliance> appliances;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void saveToFile() {
    ofstream file("appliances.txt");
    if (!file) return;

    for (size_t i = 0; i < appliances.size(); i++)
        file << appliances[i].toFileString() << endl;

    file.close();
}

void loadFromFile() {
    ifstream file("appliances.txt");
    if (!file) return;

    appliances.clear();
    string line;

    while (getline(file, line)) {
        size_t firstComma = line.find(",");
        size_t secondComma = line.rfind(",");

        if (firstComma == string::npos || secondComma == string::npos)
            continue;

        string name = line.substr(0, firstComma);
        double power = stod(line.substr(firstComma + 1, secondComma - firstComma - 1));
        double hours = stod(line.substr(secondComma + 1));

        appliances.push_back(Appliance(name, power, hours));
    }

    file.close();
}

void registerAppliance() {
    string name;
    double power;
    double hours;

    cout << "Enter appliance name: ";
    clearInput();
    getline(cin, name);

    if (name.empty()) {
        cout << "Name cannot be empty!\n";
        return;
    }

    cout << "Enter power rating (Watts): ";
    cin >> power;
    if (cin.fail() || power <= 0) {
        cout << "Power must be greater than 0!\n";
        clearInput();
        return;
    }

    cout << "Enter daily usage hours (0-24): ";
    cin >> hours;
    if (cin.fail() || hours < 0 || hours > 24) {
        cout << "Usage hours must be between 0 and 24!\n";
        clearInput();
        return;
    }

    appliances.push_back(Appliance(name, power, hours));
    cout << "Appliance registered successfully!\n";
}

void viewAppliances() {
    if (appliances.empty()) {
        cout << "No appliances registered.\n";
        return;
    }

    cout << "\n-------------------------------------------------------------\n";
    cout << left << setw(15) << "Name"
         << setw(12) << "Power(W)"
         << setw(12) << "Hours"
         << setw(12) << "Energy(kWh)" << endl;
    cout << "-------------------------------------------------------------\n";

    for (size_t i = 0; i < appliances.size(); i++)
        appliances[i].display();
}

void searchAppliance() {
    string searchName;

    cout << "Enter appliance name to search: ";
    clearInput();
    getline(cin, searchName);

    bool found = false;

    for (size_t i = 0; i < appliances.size(); i++) {
        if (appliances[i].getName() == searchName) {
            appliances[i].display();
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Appliance not found.\n";
}

double calculateTotalEnergy() {
    double total = 0;
    for (size_t i = 0; i < appliances.size(); i++)
        total += appliances[i].calculateEnergy();
    return total;
}

void calculateBilling() {
    double tariff;

    cout << "Enter electricity tariff per kWh: ";
    cin >> tariff;

    if (cin.fail() || tariff <= 0) {
        cout << "Tariff must be positive!\n";
        clearInput();
        return;
    }

    double totalEnergy = calculateTotalEnergy();
    double totalCost = totalEnergy * tariff;

    cout << "\n=========== BILLING SUMMARY ===========\n";
    cout << "Total Energy (kWh): " << fixed << setprecision(2) << totalEnergy << endl;
    cout << "Tariff per kWh:     " << tariff << endl;
    cout << "Total Cost:         " << totalCost << endl;
}

void menu() {
    int choice;

    do {
        cout << "\n===== ELECTRICAL LOAD MONITORING SYSTEM =====\n";
        cout << "1. Register Appliance\n";
        cout << "2. View Appliances\n";
        cout << "3. Search Appliance\n";
        cout << "4. Calculate Billing\n";
        cout << "5. Save & Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (cin.fail()) {
            cout << "Invalid input! Please enter a number.\n";
            clearInput();
            continue;
        }

        switch (choice) {
            case 1: registerAppliance(); break;
            case 2: viewAppliances(); break;
            case 3: searchAppliance(); break;
            case 4: calculateBilling(); break;
            case 5:
                saveToFile();
                cout << "Data saved successfully. Exiting program...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 5);
}

int main() {
    loadFromFile();
    menu();
    return 0;
}