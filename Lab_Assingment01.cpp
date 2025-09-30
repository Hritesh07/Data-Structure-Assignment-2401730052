#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

class InventoryItem {
public:
    int ItemID;
    string ItemName;
    int Quantity;
    float Price;
    InventoryItem() : ItemID(-1), Quantity(0), Price(0.0) {}
    InventoryItem(int id, string name, int qty, float price) : ItemID(id), ItemName(name), Quantity(qty), Price(price) {}
};

class InventorySystem {
    vector<InventoryItem> items;
    unordered_map<int, int> idIndexMap;
    unordered_map<string, int> nameIndexMap;
    vector<vector<float>> priceQuantityTable;
    unordered_map<int, pair<int, float>> sparseStorage;

public:
    InventorySystem() {
        priceQuantityTable = vector<vector<float>>(0, vector<float>(2, 0));
    }

    void insertItem(int id, string name, int qty, float price) {
        if (idIndexMap.count(id)) return;
        InventoryItem newItem(id, name, qty, price);
        items.push_back(newItem);
        int index = items.size() - 1;
        idIndexMap[id] = index;
        nameIndexMap[name] = index;
        priceQuantityTable.push_back({price, (float)qty});
    }

    void deleteItem(int id) {
        if (!idIndexMap.count(id)) return;
        int index = idIndexMap[id];
        items[index].ItemID = -1;
        items[index].Quantity = 0;
        items[index].Price = 0;
        priceQuantityTable[index][0] = 0;
        priceQuantityTable[index][1] = 0;
        idIndexMap.erase(id);
        nameIndexMap.erase(items[index].ItemName);
    }

    InventoryItem* searchItemByID(int id) {
        if (!idIndexMap.count(id)) return nullptr;
        int index = idIndexMap[id];
        if (items[index].ItemID == -1) return nullptr;
        return &items[index];
    }

    InventoryItem* searchItemByName(string name) {
        if (!nameIndexMap.count(name)) return nullptr;
        int index = nameIndexMap[name];
        if (items[index].ItemID == -1) return nullptr;
        return &items[index];
    }

    vector<float> rowMajorAccess() {
        vector<float> result;
        for (int i = 0; i < priceQuantityTable.size(); i++) {
            for (int j = 0; j < 2; j++) {
                result.push_back(priceQuantityTable[i][j]);
            }
        }
        return result;
    }

    vector<float> columnMajorAccess() {
        vector<float> result;
        for (int j = 0; j < 2; j++) {
            for (int i = 0; i < priceQuantityTable.size(); i++) {
                result.push_back(priceQuantityTable[i][j]);
            }
        }
        return result;
    }

    void optimizeSparseStorage() {
        sparseStorage.clear();
        for (auto &item : items) {
            if (item.ItemID != -1 && item.Quantity < 5) {
                sparseStorage[item.ItemID] = {item.Quantity, item.Price};
            }
        }
    }

    void printSparseStorage() {
        cout << "Sparse Storage (Rarely restocked items):\n";
        for (auto &entry : sparseStorage) {
            cout << "ItemID: " << entry.first << ", Quantity: " << entry.second.first << ", Price: " << entry.second.second << "\n";
        }
    }

    void complexityAnalysis() {
        cout << "Complexity Analysis:\n";
        cout << "Insert Item: Average O(1)\n";
        cout << "Delete Item: Average O(1)\n";
        cout << "Search Item: Average O(1)\n";
        cout << "Row-Major and Column-Major Access: O(n)\n";
        cout << "Sparse Storage Optimization: O(n)\n";
        cout << "Space Complexity: O(n)\n";
    }
};

int main() {
    InventorySystem invSys;
    invSys.insertItem(101, "Milk", 20, 1.5);
    invSys.insertItem(102, "Eggs", 10, 0.2);
    invSys.insertItem(103, "Bread", 3, 2.0);
    invSys.insertItem(104, "Butter", 1, 3.5);

    InventoryItem* it = invSys.searchItemByID(102);
    if (it) cout << "Found: " << it->ItemName << ", Qty: " << it->Quantity << ", Price: " << it->Price << "\n";

    it = invSys.searchItemByName("Bread");
    if (it) cout << "Found: " << it->ItemID << ", Qty: " << it->Quantity << ", Price: " << it->Price << "\n";

    cout << "Row-Major Access (Price, Quantity): ";
    for (auto val : invSys.rowMajorAccess()) cout << val << " ";
    cout << "\n";

    cout << "Column-Major Access (Price, Quantity): ";
    for (auto val : invSys.columnMajorAccess()) cout << val << " ";
    cout << "\n";

    invSys.deleteItem(104);

    invSys.optimizeSparseStorage();
    invSys.printSparseStorage();

    invSys.complexityAnalysis();

    return 0;
}