#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include <iomanip> // For Table Formatting 

using namespace std;

// ==========================================
// 0. UI HELPER FUNCTIONS (Colors Only)
// ==========================================
void printColor(string text, int colorCode) {
    // 31=Red, 32=Green, 33=Yellow, 34=Blue, 36=Cyan, 0=Reset
    cout << "\033[1;" << colorCode << "m" << text << "\033[0m" << endl;
}

void pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

// ==========================================
// 1. DATA OBJECT (Holds Patient Info)
// ==========================================
struct Patient {
    int id;
    string name;
    int age;
    string disease;
    int bedNumber; // -1 if not admitted

    Patient() { id = -1; bedNumber = -1; }

    Patient(int i, string n, int a, string d, int b = -1) {
        id = i; name = n; age = a; disease = d; bedNumber = b;
    }
};

// ==========================================
// 2. BINARY SEARCH TREE (Master Database)
// ==========================================
struct BSTNode {
    Patient data;
    BSTNode *left, *right;
    BSTNode(Patient p) { data = p; left = right = NULL; }
};

class PatientBST {
private:
    BSTNode* rot;

    BSTNode* insertRec(BSTNode* r, Patient p) {
        if (r == NULL) return new BSTNode(p);
        if (p.id < r->data.id) r->left = insertRec(r->left, p);
        else r->right = insertRec(r->right, p);
        return r;
    }

    BSTNode* searchRec(BSTNode* r, int key) {
        if (r == NULL || r->data.id == key) return r;
        if (key < r->data.id) return searchRec(r->left, key);
        return searchRec(r->right, key);
    }

    
    void inorderRec(BSTNode* r) {
        if (r == NULL) return;
        inorderRec(r->left);
        
        string bedStatus = (r->data.bedNumber == -1) ? "None" : to_string(r->data.bedNumber);
        
        cout << left << setw(8) << r->data.id 
             << setw(20) << r->data.name 
             << setw(20) << r->data.disease 
             << setw(10) << bedStatus << endl;
             
        inorderRec(r->right);
    }

    void deleteFromTree(BSTNode* &p) {
        BSTNode *current, *trailCurrent, *temp;
        if (p == NULL) return;

        if (p->left == NULL && p->right == NULL) { delete p; p = NULL; }
        else if (p->left == NULL) { temp = p; p = p->right; delete temp; }
        else if (p->right == NULL) { temp = p; p = p->left; delete temp; }
        else {
            current = p->left;
            trailCurrent = NULL;
            while (current->right != NULL) {
                trailCurrent = current;
                current = current->right;
            }
            p->data = current->data;
            if (trailCurrent == NULL) p->left = current->left;
            else trailCurrent->right = current->left;
            delete current;
        }
    }

    void deleteNodeRec(BSTNode* &p, int key) {
        if (p == NULL) { printColor("Error: Patient not found.", 31); return; }
        if (key < p->data.id) deleteNodeRec(p->left, key);
        else if (key > p->data.id) deleteNodeRec(p->right, key);
        else {
            deleteFromTree(p);
            printColor("Success: Patient record deleted.", 32);
        }
    }

    void saveRec(BSTNode* r, ofstream& outFile) {
        if (r == NULL) return;
        outFile << r->data.id << "," << r->data.name << "," 
                << r->data.age << "," << r->data.disease << "," 
                << r->data.bedNumber << endl;
        saveRec(r->left, outFile);
        saveRec(r->right, outFile);
    }

public:
    PatientBST() { rot = NULL; }
    void insert(Patient p) { rot = insertRec(rot, p); }
    BSTNode* search(int id) { return searchRec(rot, id); }
    void remove(int id) { deleteNodeRec(rot, id); }
    
    void displayAll() {
        if (rot == NULL) printColor("Database is Empty.", 33);
        else {
            printColor("\n--- PATIENT DATABASE (Sorted by ID) ---", 36);
            cout << "--------------------------------------------------------" << endl;
            cout << left << setw(8) << "ID" << setw(20) << "Name" << setw(20) << "Disease" << setw(10) << "Bed No" << endl;
            cout << "--------------------------------------------------------" << endl;
            inorderRec(rot);
        }
    }

    void saveToFile() {
        ofstream outFile("patients_data.txt");
        if (!outFile) return;
        saveRec(rot, outFile);
        outFile.close();
    }
};

// ==========================================
// 3. WAITING QUEUE (Standard Linked List)
// ==========================================
struct QueueNode {
    Patient data;
    QueueNode* next;
    QueueNode(Patient p) { data = p; next = NULL; }
};

class WaitingQueue {
private:
    QueueNode *front, *rear;
public:
    WaitingQueue() { front = rear = NULL; }

    bool isEmpty() { return front == NULL; }

    void enqueue(Patient p) {
        QueueNode* newNode = new QueueNode(p);
        if (isEmpty()) front = rear = newNode;
        else { rear->next = newNode; rear = newNode; }
        // Success Message handled in Main
    }

    Patient dequeue() {
        if (isEmpty()) return Patient();
        QueueNode* temp = front;
        Patient p = temp->data;
        front = front->next;
        if (front == NULL) rear = NULL;
        delete temp;
        return p;
    }

    void display() {
        if (isEmpty()) { printColor("Waiting Queue is Empty.", 33); return; }
        QueueNode* temp = front;
        printColor("\n--- WAITING ROOM (FIFO) ---", 36);
        cout << "Doctor <--- ";
        while (temp != NULL) {
            cout << "[" << temp->data.name << "] <--- ";
            temp = temp->next;
        }
        cout << "END" << endl;
    }
};

// ==========================================
// 4. HISTORY STACK (Standard Linked List)
// ==========================================
struct StackNode {
    Patient data;
    StackNode* next;
    StackNode(Patient p) { data = p; next = NULL; }
};

class HistoryStack {
private:
    StackNode* top;
public:
    HistoryStack() { top = NULL; }

    void push(Patient p) {
        StackNode* newNode = new StackNode(p);
        newNode->next = top;
        top = newNode;
    }

    void showLast() {
        if (top == NULL) printColor("History is Empty.", 33);
        else {
            printColor("\n--- LAST TREATED PATIENT ---", 36);
            cout << "Name:    " << top->data.name << endl;
            cout << "ID:      " << top->data.id << endl;
            cout << "Disease: " << top->data.disease << endl;
        }
    }
};

// ==========================================
// 5. WARD ARRAY (Manages Bed IDs)
// ==========================================
class WardArray {
private:
    int bedIDs[50];
public:
    WardArray() { for(int i=0; i<50; i++) bedIDs[i] = 0; }

    void setBed(int index, int id) { if(index >= 0 && index < 50) bedIDs[index] = id; }

    void assignBed(BSTNode* pNode) {
        if (pNode->data.bedNumber != -1) {
            printColor("Error: Already admitted to Bed " + to_string(pNode->data.bedNumber), 31);
            return;
        }
        for(int i=0; i<50; i++) {
            if (bedIDs[i] == 0) {
                bedIDs[i] = pNode->data.id;
                pNode->data.bedNumber = i;
                printColor("Success: Admitted " + pNode->data.name + " to Bed " + to_string(i), 32);
                return;
            }
        }
        printColor("Error: Ward is Full!", 31);
    }

    bool isAdmitted(int id) {
        for(int i=0; i<50; i++) if(bedIDs[i] == id) return true;
        return false;
    }

    void discharge(BSTNode* pNode) {
        int b = pNode->data.bedNumber;
        if(b != -1) {
            bedIDs[b] = 0;
            pNode->data.bedNumber = -1;
            printColor("Success: Patient Discharged from Ward.", 32);
        } else {
            printColor("Info: Patient was not in Ward.", 33);
        }
    }

    void showAvailable() {
        int count = 0;
        for(int i=0; i<50; i++) if(bedIDs[i] == 0) count++;
        printColor("\n--- WARD STATUS ---", 36);
        cout << "Available Beds: " << count << " / 50" << endl;
    }
};

// ==========================================
// 6. MAIN & HELPERS
// ==========================================
void loadData(PatientBST& tree, WardArray& ward) {
    ifstream inFile("patients_data.txt");
    if (!inFile) return;

    string line, temp, name, disease;
    int id, age, bed;

    while (getline(inFile, line)) {
        stringstream ss(line);
        getline(ss, temp, ','); id = stoi(temp);
        getline(ss, name, ',');
        getline(ss, temp, ','); age = stoi(temp);
        getline(ss, disease, ',');
        getline(ss, temp, ','); bed = stoi(temp);

        Patient p(id, name, age, disease, bed);
        tree.insert(p);
        if(bed != -1) ward.setBed(bed, id);
    }
    inFile.close();
}

int main() {
    PatientBST tree;
    WaitingQueue queue;
    HistoryStack stack;
    WardArray ward;

    loadData(tree, ward);

    int choice;
    do {
        cout << endl;
        printColor("======================================", 34); // Blue
        cout << "      HOSPITAL MANAGEMENT SYSTEM    " << endl;
        printColor("======================================", 34);
        cout << "1. Register New Patient" << endl;
        cout << "2. Treat Next Patient" << endl;
        cout << "3. Admit Patient to Ward" << endl;
        cout << "4. Discharge & Delete Record" << endl;
        cout << "5. View Doctor's History" << endl;
        cout << "6. Search Patient by ID" << endl;
        cout << "7. View Dashboard (All Lists)" << endl;
        cout << "8. Save & Exit" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Enter Choice: ";
        
        while (!(cin >> choice)) {
            cout << "Enter number only: ";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (choice == 1) {
            int id, age; string name, disease;
            cout << "\nEnter ID: "; cin >> id;
            
            if (tree.search(id) != NULL) {
                printColor("Error: ID Already Exists!", 31);
            } else {
                cin.ignore();
                cout << "Name: "; getline(cin, name);
                cout << "Age: "; cin >> age;
                cin.ignore();
                cout << "Disease: "; getline(cin, disease);

                Patient p(id, name, age, disease);
                tree.insert(p);
                queue.enqueue(p);
                printColor("Success: Patient Registered & Added to Queue.", 32);
            }
            pause();
        }
        else if (choice == 2) {
            if (queue.isEmpty()) {
                printColor("Queue is empty. No patients to treat.", 33);
            } else {
                Patient p = queue.dequeue();
                printColor(">>> TREATING: " + p.name + " (" + p.disease + ")", 36); // Cyan
                stack.push(p);
            }
            pause();
        }
        else if (choice == 3) {
            int id;
            cout << "Enter ID to Admit: "; cin >> id;
            BSTNode* node = tree.search(id);
            if (node) ward.assignBed(node);
            else printColor("Error: Patient Not Found.", 31);
            pause();
        }
        else if (choice == 4) {
            int id;
            cout << "Enter ID to Delete: "; cin >> id;
            BSTNode* node = tree.search(id);
            
            if (node != NULL) {
                if (ward.isAdmitted(id)) {
                    char confirm;
                    printColor("Warning: Patient is currently in Ward!", 33);
                    cout << "Discharge and Delete? (y/n): ";
                    cin >> confirm;
                    if (confirm == 'y' || confirm == 'Y') {
                        ward.discharge(node);
                        tree.remove(id);
                    }
                } else {
                    tree.remove(id);
                }
            } else {
                printColor("Error: Patient Not Found.", 31);
            }
            pause();
        }
        else if (choice == 5) {
            stack.showLast();
            pause();
        }
        else if (choice == 6) {
            int id; cout << "Enter ID: "; cin >> id;
            BSTNode* r = tree.search(id);
            if (r) {
                printColor("\n--- PATIENT FOUND ---", 32);
                cout << "Name:    " << r->data.name << endl;
                cout << "Age:     " << r->data.age << endl;
                cout << "Disease: " << r->data.disease << endl;
                string b = (r->data.bedNumber == -1) ? "None" : to_string(r->data.bedNumber);
                cout << "Bed No:  " << b << endl;
            } else {
                printColor("Error: Not Found.", 31);
            }
            pause();
        }
        else if (choice == 7) {
            tree.displayAll();
            queue.display();
            ward.showAvailable();
            pause();
        }
        else if (choice == 8) {
            tree.saveToFile();
            printColor("Data Saved. Exiting System...", 32);
        }

    } while (choice != 8);

    return 0;
}