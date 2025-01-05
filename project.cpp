#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// Node structure for Binary Search Tree
struct BSTNode {
    string tankName;
    int usageRate;
    float refillTime;
    string category;
    BSTNode* left;
    BSTNode* right;

    BSTNode(string name, int rate, float time, string cat)
        : tankName(name), usageRate(rate), refillTime(time), category(cat), left(nullptr), right(nullptr) {}
};

// Function prototype for suggestTankRefills
void suggestTankRefills(const vector<int>& rates, int threshold);

// Function to compute LPS array for KMP algorithm
void computeLPSArray(const string& pattern, vector<int>& lps) {
    int length = 0;
    int i = 1;
    lps[0] = 0;

    while (i < pattern.length()) {
        if (pattern[i] == pattern[length]) {
            length++;
            lps[i] = length;
            i++;
        } else {
            if (length != 0) {
                length = lps[length - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// KMP String matching function
bool KMPSearch(const string& text, const string& pattern) {
    int m = pattern.length();
    int n = text.length();

    vector<int> lps(m, 0);
    computeLPSArray(pattern, lps);

    int i = 0;
    int j = 0;
    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == m) {
            return true;
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return false;
}

// Function to insert a node into the BST
BSTNode* insert(BSTNode* root, string name, int rate, float time, string category) {
    if (root == nullptr) {
        return new BSTNode(name, rate, time, category);
    }
    if (name < root->tankName) {
        root->left = insert(root->left, name, rate, time, category);
    } else {
        root->right = insert(root->right, name, rate, time, category);
    }
    return root;
}

// Function to search for tank details by tank name using KMP
void searchTankDetails(BSTNode* root, const string& tankName) {
    if (root == nullptr) {
        cout << "Tank named \"" << tankName << "\" not found.\n";
        return;
    }

    // Use KMP to search for the tank name in the current node's name
    if (KMPSearch(root->tankName, tankName)) {
        cout << "Tank \"" << root->tankName << "\" details:\n";
        cout << "  Usage Rate: " << root->usageRate << "\n";
        cout << "  Refill Time: " << root->refillTime << " hours\n";
        cout << "  Category: " << root->category << "\n";
        return;
    }

    if (tankName < root->tankName) {
        searchTankDetails(root->left, tankName);
    } else {
        searchTankDetails(root->right, tankName);
    }
}

// Function to build BST from tank details
BSTNode* buildBST(const vector<string>& names, const vector<int>& rates, const vector<string>& categories, const vector<float>& times) {
    BSTNode* root = nullptr;
    for (int i = 0; i < names.size(); i++) {
        root = insert(root, names[i], rates[i], times[i], categories[i]);
    }
    return root;
}

// Function to categorize tanks based on usage rate
vector<string> categorizeTanks(const vector<int>& rates) {
    vector<string> categories(rates.size());
    int sum = 0;
    for (int rate : rates) {
        sum += rate;
    }
    float avgUsage = float(sum) / rates.size();

    for (int i = 0; i < rates.size(); i++) {
        if (rates[i] > avgUsage) {
            categories[i] = "High Usage";
        } else if (rates[i] < avgUsage) {
            categories[i] = "Low Usage";
        } else {
            categories[i] = "Average Usage";
        }
    }
    return categories;
}

// Function to estimate refill time for each tank assume capacity is 10000
vector<float> estimateRefillTime(const vector<int>& rates) {
    vector<float> times(rates.size());
    for (int i = 0; i < rates.size(); i++) {
        times[i] = (10000.0f / max(rates[i], 1)) * 0.1f;
    }
    return times;
}

// Insertion Sort to sort the tanks by usage rate
void sortTanksByUsage(vector<int>& rates) {
    int n = rates.size();
    for (int i = 1; i < n; i++) {
        int key = rates[i];
        int j = i - 1;
        while (j >= 0 && rates[j] > key) {
            rates[j + 1] = rates[j];
            j--;
        }
        rates[j + 1] = key;
    }
}

void showSortedTanks(const vector<int>& rates) {
    cout << "\nSorted usage rates:\n";
    for (int rate : rates) {
        cout << rate << " ";
    }
    cout << endl;
}

// Function to suggest tank refills
void suggestTankRefills(const vector<int>& rates, int threshold) {
    cout << "\nTanks needing refills (usage above " << threshold << "):\n";
    for (int i = 0; i < rates.size(); i++) {
        if (rates[i] > threshold) {
            cout << "Tank \"" << i + 1 << "\" (Usage: " << rates[i] << ")\n";
        }
    }
}

// Function to generate random single-word tank names
string generateTankName(int tankIndex) {
    string names[] = {"blue", "black", "white", "orange", "yellow", "red", "pink", "grey", "green", "brown"};
    srand(time(0) + tankIndex);
    return names[rand() % 10];
}

// Function to display the menu
void displayMenu() {
    cout << "\nMenu:\n";
    cout << "1. Show sorted tanks by usage rate\n";
    cout << "2. Search for tank details\n";
    cout << "3. Suggest tanks needing refills\n";
    cout << "4. Exit\n";
}

int main() {
    int tankCount;
    cout << "How many water tanks do you have? ";
    cin >> tankCount;

    vector<int> usageRates(tankCount);
    vector<string> tankNames(tankCount);

    for (int i = 0; i < tankCount; i++) {
        tankNames[i] = generateTankName(i);
        cout << "Tank #" << i + 1 << ": " << tankNames[i] << endl;
    }

    cout << "Enter the water usage rates for each tank:\n";
    for (int i = 0; i < tankCount; i++) {
        cout << tankNames[i] << ": ";
        cin >> usageRates[i];
    }

    sortTanksByUsage(usageRates);
    vector<string> categories = categorizeTanks(usageRates);
    vector<float> refillTimes = estimateRefillTime(usageRates);

    BSTNode* bstRoot = buildBST(tankNames, usageRates, categories, refillTimes);

    int choice;
    string tankName;
    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                showSortedTanks(usageRates);
                break;
            case 2:
                cout << "\nEnter a tank name to find its details: ";
                cin >> tankName;
                searchTankDetails(bstRoot, tankName);
                break;
            case 3:
                int threshold;
                cout << "\nEnter the threshold for suggesting refills: ";
                cin >> threshold;
                suggestTankRefills(usageRates, threshold);
                break;
            case 4:
                cout << "Exiting the program.\n";
                return 0;
            default:
                cout << "Invalid choice, please try again.\n";
        }
    }

    return 0;
}
