#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <limits>
using namespace std;

// Patient Class
class Patient {
public:
    int patientID;
    string name;
    int age;
    string doctor;
};
// Appointment Class
class Appointment {
public:
    int appointmentID;
    int patientID;
    string patientName;
    string doctor;
    int priority;
    int arrivalOrder;
    string status;
};
// Comparator Class
class CompareAppointment {
public:
    bool operator()(const Appointment& a, const Appointment& b) const {
        if (a.priority != b.priority)
            return a.priority > b.priority;

        return a.arrivalOrder > b.arrivalOrder;
    }
};
// Main MediQueue Class
class MediQueue {
private:
    vector<Patient> patients;
    vector<Appointment> appointments;
    priority_queue<Appointment, vector<Appointment>, CompareAppointment> patientQueue;

    int nextPatientID = 1001;
    int nextAppointmentID = 501;
    int nextArrivalOrder = 1;

    // Find Patient
    int findPatientIndex(int id) {
        for (int i = 0; i < patients.size(); i++) {
            if (patients[i].patientID == id)
                return i;
        }
        return -1;
    }
    // Find Appointment
    int findAppointmentIndex(int id) {
        for (int i = 0; i < appointments.size(); i++) {
            if (appointments[i].appointmentID == id)
                return i;
        }
        return -1;
    }
    // Check Active Appointment
    bool isActive(int id) {
        int index = findAppointmentIndex(id);
        return index != -1 && appointments[index].status == "Waiting";
    }

    // Clean Queue
    void cleanQueueTop() {
        while (!patientQueue.empty() &&
               !isActive(patientQueue.top().appointmentID)) {
            patientQueue.pop();
        }
    }
public:
    // Register Patient
    void registerPatient() {
        Patient p;
        p.patientID = nextPatientID++;
        cout << "Enter patient name: ";
        getline(cin >> ws, p.name);
        cout << "Enter age: ";
        cin >> p.age;

        cout << "Enter preferred doctor/specialty: ";
        getline(cin >> ws, p.doctor);

        patients.push_back(p);

        cout << "\nPatient registered successfully!\n";
        cout << "Patient ID: " << p.patientID << "\n";
    }
    // Book Appointment
    void bookAppointment() {
        if (patients.empty()) {
            cout << "No patients registered yet.\n";
            return;
        }
        int id;
        cout << "Enter Patient ID: ";
        cin >> id;
        int index = findPatientIndex(id);
        if (index == -1) {
            cout << "Patient not found.\n";
            return;
        }
        Appointment a;
        a.appointmentID = nextAppointmentID++;
        a.patientID = patients[index].patientID;
        a.patientName = patients[index].name;
        a.doctor = patients[index].doctor;

        cout << "\nSelect scheduling priority:\n";
        cout << "1. Highest priority\n";
        cout << "2. Medium priority\n";
        cout << "3. Routine priority\n";
        cout << "Enter priority (1-3): ";

        cin >> a.priority;

        if (a.priority < 1 || a.priority > 3) {
            cout << "Invalid priority.\n";
            return;
        }

        a.arrivalOrder = nextArrivalOrder++;
        a.status = "Waiting";
        appointments.push_back(a);
        patientQueue.push(a);
        cout << "\nAppointment booked successfully!\n";
        cout << "Appointment ID: " << a.appointmentID << "\n";
        cout << "Patient: " << a.patientName << "\n";
        cout << "Doctor: " << a.doctor << "\n";
        cout << "Priority: " << a.priority << "\n";
    }
    // Display All Appointments
    void displayAllAppointments() {
        if (appointments.empty()) {
            cout << "No appointments found.\n";
            return;
        }
        cout << "\n--- ALL APPOINTMENTS ---\n";
        for (const Appointment& a : appointments) {
            cout << "Appointment ID: " << a.appointmentID
                 << " | Patient ID: " << a.patientID
                 << " | Name: " << a.patientName
                 << " | Doctor: " << a.doctor
                 << " | Priority: " << a.priority
                 << " | Status: " << a.status << "\n";
        }
    }
    // View Next Patient
    void viewNextPatient() {
        cleanQueueTop();

        if (patientQueue.empty()) {
            cout << "No patients are currently waiting.\n";
            return;
        }
        Appointment a = patientQueue.top();

        cout << "\n--- NEXT PATIENT ---\n";
        cout << "Appointment ID: " << a.appointmentID << "\n";
        cout << "Patient: " << a.patientName << "\n";
        cout << "Doctor: " << a.doctor << "\n";
        cout << "Priority: " << a.priority << "\n";

        cout << "(This only views the patient.)\n";
    }

    // Attend Next Patient
    void attendNextPatient() {
        cleanQueueTop();

        if (patientQueue.empty()) {
            cout << "No patients are currently waiting.\n";
            return;
        }
        Appointment a = patientQueue.top();
        patientQueue.pop();
        int index = findAppointmentIndex(a.appointmentID);
        if (index != -1)
            appointments[index].status = "Completed";
        cout << "\nNow attending: " << a.patientName
             << " (Appointment ID: " << a.appointmentID << ")\n";
        cout << "Appointment marked as Completed.\n";
    }
    // Cancel Appointment
    void cancelAppointment() {
        int id;
        cout << "Enter Appointment ID to cancel: ";
        cin >> id;
        int index = findAppointmentIndex(id);
        if (index == -1) {
            cout << "Appointment not found.\n";
            return;
        }
        if (appointments[index].status != "Waiting") {
            cout << "Only waiting appointments can be cancelled.\n";
            return;
        }
        appointments[index].status = "Cancelled";
        cout << "Appointment " << id << " cancelled successfully.\n";
    }
    // Search Patient
    void searchPatient() {
        int id;
        cout << "Enter Patient ID to search: ";
        cin >> id;
        int index = findPatientIndex(id);
        if (index == -1) {
            cout << "Patient not found.\n";
            return;
        }
        Patient p = patients[index];
        cout << "\n--- PATIENT DETAILS ---\n";
        cout << "Patient ID: " << p.patientID << "\n";
        cout << "Name: " << p.name << "\n";
        cout << "Age: " << p.age << "\n";
        cout << "Preferred doctor/specialty: " << p.doctor << "\n";

        cout << "Appointments:\n";
        bool found = false;
        for (const Appointment& a : appointments) {
            if (a.patientID == id) {
                found = true;

                cout << "Appointment ID: " << a.appointmentID
                     << " | Priority: " << a.priority
                     << " | Status: " << a.status << "\n";
            }
        }
        if (!found)
            cout << "No appointments booked yet.\n";
    }
    // Display Waiting List
    void displayWaitingList() {
        cleanQueueTop();

        if (patientQueue.empty()) {
            cout << "Waiting list is empty.\n";
            return;
        }
        auto temp = patientQueue;
        cout << "\n--- WAITING LIST ---\n";
        while (!temp.empty()) {
            Appointment a = temp.top();
            temp.pop();
            if (isActive(a.appointmentID)) {
                cout << "Appointment ID: " << a.appointmentID
                     << " | Patient: " << a.patientName
                     << " | Doctor: " << a.doctor
                     << " | Priority: " << a.priority << "\n";
            }
        }
    }

    void menu() {
        int choice;

        do {
            cout << "\n========================================\n";
            cout << "                MEDIQUEUE\n";
            cout << " Hospital Appointment Management System\n";
            cout << "========================================\n";

            cout << "1. Register Patient\n";
            cout << "2. Book Appointment\n";
            cout << "3. View All Appointments\n";
            cout << "4. View Next Patient\n";
            cout << "5. Attend Next Patient\n";
            cout << "6. Cancel Appointment\n";
            cout << "7. Search Patient\n";
            cout << "8. Display Waiting List\n";
            cout << "9. Exit\n";

            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    registerPatient();
                    break;

                case 2:
                    bookAppointment();
                    break;

                case 3:
                    displayAllAppointments();
                    break;

                case 4:
                    viewNextPatient();
                    break;

                case 5:
                    attendNextPatient();
                    break;

                case 6:
                    cancelAppointment();
                    break;

                case 7:
                    searchPatient();
                    break;

                case 8:
                    displayWaitingList();
                    break;

                case 9:
                    cout << "Thank you for using MediQueue!\n";
                    break;

                default:
                    cout << "Invalid choice. Please select 1-9.\n";
            }

        } while (choice != 9);
    }
};

int main() {
    MediQueue hospital;

    hospital.menu();

    return 0;
}