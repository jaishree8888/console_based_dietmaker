#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept> // for std::invalid_argument
using namespace std;

class FoodItem {
public:
    string name;
    int calories;
    double protein;  // in grams
    double fat;      // in grams
    double carbs;    // in grams

    FoodItem(string n, int c, double p, double f, double carb)
        : name(n), calories(c), protein(p), fat(f), carbs(carb) {}
};

class Meal {
public:
    string name;
    vector<FoodItem> foodItems;

    Meal(string n) : name(n) {}

    void addFoodItem(const FoodItem& item) {
        foodItems.push_back(item);
    }

    void displayMeal() const {
        cout << "Meal: " << name << endl;
        for (const auto& item : foodItems) {
            cout << "  Food: " << item.name << ", Calories: " << item.calories
                 << ", Protein: " << item.protein << "g"
                 << ", Fat: " << item.fat << "g"
                 << ", Carbs: " << item.carbs << "g" << endl;
        }
    }
};

class DietPlan {
public:
    vector<Meal> meals;

    void addMeal(const Meal& meal) {
        meals.push_back(meal);
    }

    void displayDietPlan() const {
        for (const auto& meal : meals) {
            meal.displayMeal();
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file " << filename << " for writing." << endl;
            return;
        }

        for (const auto& meal : meals) {
            file << meal.name << endl;
            for (const auto& item : meal.foodItems) {
                file << item.name << "," << item.calories << ","
                     << item.protein << "," << item.fat << "," << item.carbs << endl;
            }
            file << endl; // Separate meals with an empty line
        }
        file.close();
        cout << "Diet routine saved to " << filename << " successfully." << endl;
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file " << filename << " for reading." << endl;
            return;
        }

        meals.clear(); // Clear existing meals before loading

        string line;
        while (getline(file, line)) {
            Meal meal(line);
            while (getline(file, line) && !line.empty()) {
                size_t pos = 0;
                vector<string> tokens;
                while ((pos = line.find(',')) != string::npos) {
                    tokens.push_back(line.substr(0, pos));
                    line.erase(0, pos + 1);
                }
                tokens.push_back(line);

                try {
                    FoodItem item(tokens[0], stoi(tokens[1]), stod(tokens[2]),
                                  stod(tokens[3]), stod(tokens[4]));
                    meal.addFoodItem(item);
                } catch (const invalid_argument& e) {
                    cerr << "Error: Invalid data format in file " << filename << ". Skipping this item." << endl;
                }
            }
            addMeal(meal);
        }

        file.close();
        cout << "Diet routine loaded from " << filename << " successfully." << endl;
    }
};

int main() {
    DietPlan dietPlan;

    while (true) {
        cout << "Welcome to DietBuddy - Your Personal Diet Routine Maker!" << endl;
        cout << "Please choose an option:" << endl;
        cout << "1. Add Meal" << endl;
        cout << "2. Display Diet Plan" << endl;
        cout << "3. Save Diet Plan" << endl;
        cout << "4. Load Diet Plan" << endl;
        cout << "5. Exit" << endl;

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter meal name: ";
                string mealName;
                cin.ignore();
                getline(cin, mealName);
                Meal meal(mealName);

                while (true) {
                    cout << "Enter food name (or 'done' to finish): ";
                    string foodName;
                    getline(cin, foodName);
                    if (foodName == "done") break;

                    int calories;
                    double protein, fat, carbs;

                    cout << "Enter calories: ";
                    cin >> calories;
                    cout << "Enter protein (g): ";
                    cin >> protein;
                    cout << "Enter fat (g): ";
                    cin >> fat;
                    cout << "Enter carbs (g): ";
                    cin >> carbs;
                    cin.ignore();

                    FoodItem foodItem(foodName, calories, protein, fat, carbs);
                    meal.addFoodItem(foodItem);
                }

                dietPlan.addMeal(meal);
                break;
            }
            case 2:
                dietPlan.displayDietPlan();
                break;
            case 3: {
                cout << "Enter filename to save: ";
                string filename;
                cin >> filename;
                dietPlan.saveToFile(filename);
                break;
            }
            case 4: {
                cout << "Enter filename to load: ";
                string filename;
                cin >> filename;
                dietPlan.loadFromFile(filename);
                break;
            }
            case 5:
                cout << "Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
