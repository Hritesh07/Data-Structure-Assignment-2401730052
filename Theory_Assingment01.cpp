#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

struct Date {
    int day;
    int month;
    int year;
    Date(int d = 1, int m = 1, int y = 2025) : day(d), month(m), year(y) {}
    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }
};

struct DateHash {
    size_t operator()(const Date& d) const {
        return hash<int>()(d.day) ^ hash<int>()(d.month << 1) ^ hash<int>()(d.year << 2);
    }
};

class WeatherRecord {
public:
    Date date;
    string city;
    double temperature;
    WeatherRecord(Date d, string c, double t) : date(d), city(c), temperature(t) {}
};

class WeatherDataStorage {
    vector<string> years;
    vector<string> cities;
    vector<vector<double>> data;
    unordered_map<Date, unordered_map<string, double>, DateHash> sparseData;
    const double SENTINEL = -9999.0;

public:
    WeatherDataStorage(vector<string> y, vector<string> c) {
        years = y;
        cities = c;
        data = vector<vector<double>>(y.size(), vector<double>(c.size(), SENTINEL));
    }

    int getYearIndex(const string& year) {
        for (int i = 0; i < years.size(); i++)
            if (years[i] == year)
                return i;
        return -1;
    }

    int getCityIndex(const string& city) {
        for (int i = 0; i < cities.size(); i++)
            if (cities[i] == city)
                return i;
        return -1;
    }

    void insertDense(string year, string city, double temperature) {
        int yIdx = getYearIndex(year);
        int cIdx = getCityIndex(city);
        if (yIdx != -1 && cIdx != -1)
            data[yIdx][cIdx] = temperature;
    }

    void deleteDense(string year, string city) {
        int yIdx = getYearIndex(year);
        int cIdx = getCityIndex(city);
        if (yIdx != -1 && cIdx != -1)
            data[yIdx][cIdx] = SENTINEL;
    }

    double retrieveDense(string year, string city) {
        int yIdx = getYearIndex(year);
        int cIdx = getCityIndex(city);
        if (yIdx != -1 && cIdx != -1)
            return data[yIdx][cIdx];
        return SENTINEL;
    }

    vector<double> rowMajorAccess() {
        vector<double> result;
        for (auto& row : data)
            for (auto& val : row)
                result.push_back(val);
        return result;
    }

    vector<double> columnMajorAccess() {
        vector<double> result;
        int rows = data.size();
        int cols = cities.size();
        for (int c = 0; c < cols; c++)
            for (int r = 0; r < rows; r++)
                result.push_back(data[r][c]);
        return result;
    }

    void insertSparse(Date date, string city, double temperature) {
        sparseData[date][city] = temperature;
    }

    void deleteSparse(Date date, string city) {
        if (sparseData.count(date) && sparseData[date].count(city))
            sparseData[date].erase(city);
    }

    double retrieveSparse(Date date, string city) {
        if (sparseData.count(date) && sparseData[date].count(city))
            return sparseData[date][city];
        return SENTINEL;
    }

    void analyzeComplexity() {
        cout << "Insert: O(1) for both dense and sparse storage\n";
        cout << "Delete: O(1) for both dense and sparse storage\n";
        cout << "Retrieve: O(1) for both dense and sparse storage\n";
        cout << "Space:\n";
        cout << "- Dense matrix requires O(years * cities) space\n";
        cout << "- Sparse map requires O(k) space, where k is number of stored entries\n";
    }
};

int main() {
    vector<string> years = {"2023", "2024", "2025"};
    vector<string> cities = {"Delhi", "Mumbai", "Chennai"};

    WeatherDataStorage storage(years, cities);
    storage.insertDense("2024", "Delhi", 32.5);
    storage.insertDense("2025", "Mumbai", 30.2);
    storage.insertDense("2023", "Chennai", 33.1);

    cout << "Retrieved (Dense) 2024 Delhi: " << storage.retrieveDense("2024", "Delhi") << "\n";
    cout << "Retrieved (Dense) 2023 Mumbai: " << storage.retrieveDense("2023", "Mumbai") << "\n";

    vector<double> rowData = storage.rowMajorAccess();
    cout << "Row-major data: ";
    for (double val : rowData) cout << val << " ";
    cout << "\n";

    vector<double> colData = storage.columnMajorAccess();
    cout << "Column-major data: ";
    for (double val : colData) cout << val << " ";
    cout << "\n";

    Date d1(15, 7, 2025);
    storage.insertSparse(d1, "Delhi", 34.7);
    cout << "Retrieved (Sparse) 15/7/2025 Delhi: " << storage.retrieveSparse(d1, "Delhi") << "\n";

    storage.analyzeComplexity();

    return 0;
}