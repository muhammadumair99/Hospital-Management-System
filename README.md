# 🏥 Hospital Management System (C++)

A comprehensive console-based application designed to streamline hospital operations using efficient **Data Structures and Algorithms (DSA)**. This project implements a modular architecture to handle patient records, waiting lines, doctor consultations, and ward admissions.

---

## 📖 Project Overview
This system moves away from manual record-keeping, offering a digital solution for:
* **Patient Registration** (Database Management)
* **OPD Flow** (First-Come-First-Serve)
* **Doctor's History** (Recent Treatments)
* **Ward Management** (Bed Allocation)

The system ensures data persistence using **File Handling**, saving records to `patients_data.txt` automatically.

---

## 🏗️ System Architecture & Data Structures
The project follows a **Modular Approach**, utilizing specific data structures optimized for each task:

| Component | Data Structure | Why it was used? | Key Operations |
| :--- | :--- | :--- | :--- |
| **Master Database** | **Binary Search Tree (BST)** | Fast searching ($O(\log n)$) and sorting by ID. | `insert`, `search`, `delete` |
| **Waiting Room** | **Linked Queue** | Ensures fairness (FIFO - First In, First Out). | `enqueue`, `dequeue` |
| **History Log** | **Linked Stack** | Tracks the most recently treated patient (LIFO). | `push`, `top` |
| **Ward System** | **Array** (Size 50) | Manages fixed resources (Beds) with $O(1)$ access. | Index-based assignment |

> **Core Entity:** The `Patient` struct holds raw data (ID, Name, Age, Disease, BedNo) and flows through all these structures.

---

## 🚀 Features

### 1. Patient Registration
* Registers new patients with unique IDs.
* Automatically adds them to the **Database (BST)** and the **Waiting Queue**.
* *Validation:* Prevents duplicate IDs.

### 2. Consultation (OPD)
* **"Treat Next Patient"**: Removes the patient from the front of the Queue.
* Displays patient details in **Cyan** for visibility.
* Moves the record to the **History Stack**.

### 3. Ward Management
* **Admit:** specific patients to the Ward (Array). Assigns the first available bed ID.
* **Discharge:** Removes patient from the Ward and deletes their record from the Database.
* *Status Check:* Shows total available beds (e.g., 45/50).

### 4. Search & Dashboard
* **Search:** Find any patient instantly by ID.
* **Dashboard:** Displays the entire database (Sorted), current Waiting Queue, and Ward Status in one view.

### 5. Data Persistence
* Automatically loads data from `patients_data.txt` on startup.
* Saves all modifications back to the file upon exit.

---

## 🛠️ Tech Stack
* **Language:** C++
* **Concepts:** Pointers, Recursion, File I/O, Structs, Classes.
* **UI:** Color-coded console output (ANSI Escape Codes).

---

## 💻 How to Run

1.  **Clone the Repository**
    ```bash
    git clone [https://github.com/YourUsername/Hospital-Management-System-CPP.git](https://github.com/YourUsername/Hospital-Management-System-CPP.git)
    cd Hospital-Management-System-CPP
    ```

2.  **Compile the Code**
    Open your terminal and run:
    ```bash
    g++ main.cpp -o hospital
    ```

3.  **Run the Application**
    * **Windows:**
        ```bash
        hospital.exe
        ```
    * **Mac/Linux:**
        ```bash
        ./hospital
        ```

---

**Menu Interface:**
```text
    HOSPITAL MANAGEMENT SYSTEM
    ==========================
    1. Register New Patient
    2. Treat Next Patient
    3. Admit Patient to Ward
    4. Discharge & Delete Record
    5. View Doctor's History
    6. Search Patient by ID
    7. View Dashboard
    8. Save & Exit
