#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "json.hpp"  // Include JSON library
using json = nlohmann::json;

struct Employee {
    std::string name;
    int id;
    double performanceScore;
};

// Read data from CSV file
std::vector<Employee> readCSV(const std::string& filename) {
    std::vector<Employee> employees;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        exit(1);
    }
    std::string line;
    std::getline(file, line); // Skip header row
    while (std::getline(file, line)) {
        size_t comma1 = line.find(',');
        size_t comma2 = line.find(',', comma1 + 1);
        if (comma1 != std::string::npos && comma2 != std::string::npos) {
            Employee emp;
            emp.name = line.substr(0, comma1);
            emp.id = std::stoi(line.substr(comma1 + 1, comma2 - comma1 - 1));
            emp.performanceScore = std::stod(line.substr(comma2 + 1));
            employees.push_back(emp);
        }
    }
    file.close();
    return employees;
}

// Sort employees by performance score (descending order)
void sortEmployees(std::vector<Employee>& employees) {
    std::sort(employees.begin(), employees.end(), [](const Employee& a, const Employee& b) {
        return a.performanceScore > b.performanceScore;
    });
}

// Convert employees to JSON format
json employeesToJson(const std::vector<Employee>& employees) {
    json result;
    for (const auto& emp : employees) {
        json empJson;
        empJson["name"] = emp.name;
        empJson["id"] = emp.id;
        empJson["performanceScore"] = emp.performanceScore;
        result.push_back(empJson);
    }
    return result;
}

int main() {
    std::vector<Employee> employees = readCSV("employee_data.csv");
    sortEmployees(employees);
    json sortedEmployees = employeesToJson(employees);
    std::ofstream file("sorted_employees.json");
    file << sortedEmployees.dump(4); // Pretty print with 4-space indentation
    file.close();
    std::cout << "Sorted employee data saved to sorted_employees.json!" << std::endl;
    return 0;
}