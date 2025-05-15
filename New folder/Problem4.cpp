// => ** FCAI-CU - Data Structure – 2025 - Assignment ( 2 ) - Problem ( 4 ) ** <=
// => Last Modification Date: 15/05/2025
// => Under supervision of  : Dr.Basheer Abdel Fatah
// => TA                    : Eng.Abdalrahman Roshdi & Eng.Ashraf Mohy
// => File Name             : Problem4.cpp
// => Authors               : Malak Mohamed Saad Fahmy Al-Aabiad     => ID: ( 20230417 )  => SECTION: S3,4
// => Emails                : malakkalaabiadd@gmail.com
//----------------------------------------------------------------------------------------------------------------------

#include "bits/stdc++.h"

using namespace std;

// Forward declaration if needed, but not strictly necessary here
// class MaxHeap;

const int MAX_NAME_LEN = 50;

// Structure to represent a patient
struct Patient {
    char name[MAX_NAME_LEN];
    int severity;
    int arrival_time;

    // Default constructor
    Patient() : severity(0), arrival_time(0) {
        name[0] = '\0';
    }

    // Parameterized constructor
    Patient(const char* n, int s, int at) : severity(s), arrival_time(at) {
        if (n) {
            strncpy(this->name, n, MAX_NAME_LEN - 1);
            this->name[MAX_NAME_LEN - 1] = '\0'; // Ensure null termination
        } else {
            this->name[0] = '\0';
        }
    }

    // Copy constructor (important for array reallocations and returns by value)
    Patient(const Patient& other) : severity(other.severity), arrival_time(other.arrival_time) {
        strncpy(this->name, other.name, MAX_NAME_LEN - 1);
        this->name[MAX_NAME_LEN - 1] = '\0';
    }

    // Assignment operator (important for array reallocations and assignments)
    Patient& operator=(const Patient& other) {
        if (this != &other) {
            severity = other.severity;
            arrival_time = other.arrival_time;
            strncpy(this->name, other.name, MAX_NAME_LEN - 1);
            this->name[MAX_NAME_LEN - 1] = '\0';
        }
        return *this;
    }

    // Helper to check if this is a valid patient vs. a placeholder for "None"
    bool isValid() const {
        // Assuming EMPTY_PATIENT will have severity -1
        return severity != -1;
    }
};

// Comparison function for patients
// Returns true if p1 has higher priority than p2
bool hasHigherPriority(const Patient& p1, const Patient& p2) {
    if (p1.severity > p2.severity) {
        return true;
    }
    if (p1.severity < p2.severity) {
        return false;
    }
    // Severities are equal, compare arrival times (lower arrival_time is higher priority)
    return p1.arrival_time < p2.arrival_time;
}

class MaxHeap {
private:
    Patient* heapArray;
    int currentSize;
    int capacity;

    // Helper to get parent index
    int parent(int i) {
        return (i - 1) / 2;
    }

    // Helper to get left child index
    int leftChild(int i) {
        return (2 * i) + 1;
    }

    // Helper to get right child index
    int rightChild(int i) {
        return (2 * i) + 2;
    }

    // Helper to swap two patients
    void swap(Patient& p1, Patient& p2) {
        Patient temp = p1;
        p1 = p2;
        p2 = temp;
    }

    // Resize the internal array when it's full
    void resize() {
        int newCapacity = (capacity == 0) ? 1 : capacity * 2;
        Patient* newArray = new Patient[newCapacity];
        for (int i = 0; i < currentSize; ++i) {
            newArray[i] = heapArray[i];
        }
        delete[] heapArray;
        heapArray = newArray;
        capacity = newCapacity;
    }

    // Restore heap property upwards
    void heapifyUp(int index) {
        while (index > 0 && hasHigherPriority(heapArray[index], heapArray[parent(index)])) {
            swap(heapArray[index], heapArray[parent(index)]);
            index = parent(index);
        }
    }

    // Restore heap property downwards
    void heapifyDown(int index) {
        int largest = index;
        int l = leftChild(index);
        int r = rightChild(index);

        if (l < currentSize && hasHigherPriority(heapArray[l], heapArray[largest])) {
            largest = l;
        }
        if (r < currentSize && hasHigherPriority(heapArray[r], heapArray[largest])) {
            largest = r;
        }

        if (largest != index) {
            swap(heapArray[index], heapArray[largest]);
            heapifyDown(largest);
        }
    }

public:
    // Static member to represent "None" or an empty/invalid patient
    // Initialized outside the class
    static Patient EMPTY_PATIENT;

    MaxHeap(int initialCapacity = 10) {
        capacity = (initialCapacity > 0) ? initialCapacity : 10; // Ensure positive capacity
        heapArray = new Patient[capacity];
        currentSize = 0;
    }

    ~MaxHeap() {
        delete[] heapArray;
    }

    // Inserts a new patient into the priority queue
    void insert(const Patient& patient) {
        if (currentSize == capacity) {
            resize();
        }
        heapArray[currentSize] = patient;
        heapifyUp(currentSize);
        currentSize++;
    }

    // Returns and removes the patient with the highest priority
    Patient extract_max() {
        if (isEmpty()) {
            return EMPTY_PATIENT;
        }
        Patient maxPatient = heapArray[0];
        heapArray[0] = heapArray[currentSize - 1];
        currentSize--;
        if (currentSize > 0) { // Only heapify if there are elements left
            heapifyDown(0);
        }
        return maxPatient;
    }

    // Returns, but does not remove, the patient with the highest priority
    Patient peek() const {
        if (isEmpty()) {
            return EMPTY_PATIENT;
        }
        return heapArray[0];
    }

    // Checks if the heap is empty
    bool isEmpty() const {
        return currentSize == 0;
    }

    // Prints the heap as an array (names only)
    void print_heap() const {
        std::cout << "Heap (Array Order): [";
        for (int i = 0; i < currentSize; ++i) {
            std::cout << "'" << heapArray[i].name << "'";
            if (i < currentSize - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }
};

// Initialize static member EMPTY_PATIENT
Patient MaxHeap::EMPTY_PATIENT = Patient(nullptr, -1, -1);


int main() {
    MaxHeap erQueue;
    const char* inputFileame = "input.txt";
    ifstream inputFile(inputFileame);

    if (!inputFile.is_open()) {
        cerr << "Error: Could not open file " << inputFileame << endl;
        return 1;
    }

    cout << "--- Simulating Patient Arrivals ---" << endl;
    char nameBuffer[MAX_NAME_LEN];
    int severity, arrivalTime;
    int patientCount = 0;

    while (inputFile >> nameBuffer >> severity >> arrivalTime) {
        patientCount++;
        Patient p(nameBuffer, severity, arrivalTime);
        cout << "Step " << patientCount << " Inserted " << p.name << endl;
        erQueue.insert(p);
        erQueue.print_heap();
        // Clear buffer for next read, not strictly necessary with >> but good practice
        nameBuffer[0] = '\0';
    }
    inputFile.close();

    if (patientCount == 0) {
        cout << "No patients read from file." << endl;
    }

    cout << "\n--- Simulating Patient Treatment ---" << endl;
    int treatedCount = 0;
    while (!erQueue.isEmpty()) {
        Patient treatedPatient = erQueue.extract_max();
        if (treatedPatient.isValid()) { // Check if it's not the EMPTY_PATIENT
            treatedCount++;
            cout << "Treating patient " << treatedCount << ": " << treatedPatient.name
                      << " (Severity: " << treatedPatient.severity
                      << ", Arrival: " << treatedPatient.arrival_time << ")" << endl;
        } else {
            // This case should not happen if isEmpty() is checked, but good for robustness
            cerr << "Error: Extracted an invalid patient from a non-empty queue." << endl;
            break;
        }
    }

    if (treatedCount == 0 && patientCount > 0) {
        cout << "No patients were treated (queue might have been empty or contained only invalid patients)." << endl;
    } else if (treatedCount == 0 && patientCount == 0) {
        cout << "Emergency room is empty. No patients to treat." << endl;
    }


    cout << "\n--- Testing Peek and Empty Operations ---" << endl;
    MaxHeap testHeap;
    Patient p = testHeap.peek();
    if (!p.isValid()) {
        cout << "Peek on empty heap: Correctly returned None (Invalid Patient)" << endl;
    } else {
        cout << "Peek on empty heap: Error, returned " << p.name << endl;
    }
    p = testHeap.extract_max();
    if (!p.isValid()) {
        cout << "Extract_max on empty heap: Correctly returned None (Invalid Patient)" << endl;
    } else {
        cout << "Extract_max on empty heap: Error, returned " << p.name << endl;
    }

    testHeap.insert(Patient("Test1", 10, 1));
    p = testHeap.peek();
    if (p.isValid() && strcmp(p.name, "Test1") == 0) {
        cout << "Peek on heap with 1 element: Correctly returned " << p.name << endl;
    } else {
        cout << "Peek on heap with 1 element: Error" << endl;
    }
    testHeap.print_heap();
    p = testHeap.extract_max();
    if (p.isValid() && strcmp(p.name, "Test1") == 0) {
        cout << "Extract_max on heap with 1 element: Correctly returned " << p.name << endl;
    } else {
        cout << "Extract_max on heap with 1 element: Error" << endl;
    }
    if (testHeap.isEmpty()) {
        cout << "Heap is empty after extracting last element: Correct" << endl;
    } else {
        cout << "Heap is not empty after extracting last element: Error" << endl;
    }

    return 0;
}