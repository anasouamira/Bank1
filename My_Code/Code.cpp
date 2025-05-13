#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const string ClientsFileName = "Clients.txt";

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false; 
};

// Reads client account number from user input
string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter Account Number: ";
    cin >> AccountNumber;
    return AccountNumber;
}

// Converts a client struct into a string line to be stored in file
string ConvertRecordToLine(sClient NewClient, string separator = "#//#") {
    string stClientRecord = "";
    stClientRecord += NewClient.AccountNumber + separator;
    stClientRecord += NewClient.PinCode + separator;
    stClientRecord += NewClient.Name + separator;
    stClientRecord += NewClient.Phone + separator;
    stClientRecord += to_string(NewClient.AccountBalance);
    return stClientRecord;
}

// Splits a string based on a given separator into a vector of strings
vector <string> SplitString(string line, string Separator) {
    vector <string> vClient;
    string word;
    short pos;
    while ((pos = line.find(Separator)) != std::string::npos) {
        word = line.substr(0, pos);
        if (word != "") {
            vClient.push_back(word);
        }
        line = line.erase(0, pos + Separator.length());
    }
    if (line != "") {
        vClient.push_back(line);
    }
    return vClient;
}

// Converts a line from the file into a client struct
sClient ConvertLineToRecord(string line, string Separator = "#//#") {
    vector <string> vClientData = SplitString(line, Separator);
    sClient Client;
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);
    return Client;
}

// Loads all clients from file into a vector
vector<sClient> LoadClientsDataFromFile(string FileName) {
    vector <sClient> vClient;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open()) {
        string line;
        sClient Client;
        while (getline(MyFile, line)) {
            Client = ConvertLineToRecord(line);
            vClient.push_back(Client);
        }
        MyFile.close();
    }
    return vClient;
}

// Searches for a client by account number
bool FindClientByAccountNumber(string AccontNumber, sClient& Client) {
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    for (sClient C : vClients) {
        if (C.AccountNumber == AccontNumber) {
            Client = C;
            return true;
        }
    }
    return false;
}

// Saves all clients (excluding marked for deletion) to the file
void SaveClientsDataToFile(vector <sClient> vClients) {
    fstream MyFile;
    string Dataline;
    MyFile.open(ClientsFileName, ios::out);
    if (MyFile.is_open()) {
        for (sClient C : vClients) {
            if (C.MarkForDelete == false) {
                Dataline = ConvertRecordToLine(C);
                MyFile << Dataline << endl;
            }
        }
        MyFile.close();
    }
}

// Prints one client’s information in table format
void PrintClientRecord(sClient Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

// Prints the full list of clients in a formatted table
void PrintClientsData(vector <sClient> vClients) {
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (sClient Client : vClients) {
        PrintClientRecord(Client);
        cout << endl;
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

// Loads and prints all clients data from file
void PrintAllClientsData() {
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    PrintClientsData(vClients);
}

// Adds a new line (client) to the file
void AddDatalineToFile(string ClientsFileName, string line) {
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::out | ios::app);
    if (MyFile.is_open()) {
        MyFile << line << endl;
        MyFile.close();
    }
}

// Checks if a client account number already exists
bool isClientAccountNumberExist(string AccontNumber) {
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    for (sClient C : vClients) {
        if (C.AccountNumber == AccontNumber) {
            return true;
        }
    }
    return false;
}

// Reads new client information from user input
sClient ReadNewClient() {
    sClient Client;
    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);
    while (isClientAccountNumberExist(Client.AccountNumber)) {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number : ";
        getline(cin >> ws, Client.AccountNumber);
    }
    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

// Adds a new client and saves it to the file
void AddNewClient() {
    sClient NewClient = ReadNewClient();
    AddDatalineToFile(ClientsFileName, ConvertRecordToLine(NewClient));
}

// Loops to add multiple clients
void AddClient() {
    char AddMore = 'Y';
    do {
        system("cls");
        cout << "Adding New Client:\n\n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

// Prints a client in a readable "card" format
void PrintClientCard(sClient Client) {
    cout << "\nThe following are the client details:\n";
    cout << "\nAccout Number   : " << Client.AccountNumber;
    cout << "\nPin Code        : " << Client.PinCode;
    cout << "\nName            : " << Client.Name;
    cout << "\nPhone           : " << Client.Phone;
    cout << "\nAccount Balance : " << Client.AccountBalance;
}

// Finds and prints a specific client
void FindClient() {
    string AccountNumber = ReadClientAccountNumber();
    sClient Client;
    if (FindClientByAccountNumber(AccountNumber, Client)) {
        PrintClientCard(Client);
    } else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }
}

// Marks a client as deleted
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient> &vClients) {
    for (sClient &C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

// Deletes a client by account number
void DeleteClient(string AccountNumber, vector <sClient> &vClients) {
    sClient Client;
    char Answer = 'N';
    if (FindClientByAccountNumber(AccountNumber, Client)) {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want to delete this client? (y/n): ";
        cin >> Answer;
        if (Answer == 'Y' || Answer == 'y') {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(vClients);
            vClients = LoadClientsDataFromFile(ClientsFileName);
            cout << "\n\nClient Deleted Successfully.";
        } else {
            cout << "\n\nDeletion Cancelled.";
        }
    } else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }
}

// Loads clients and deletes the one with matching account number
void DeleteClientByAccountNumber() {
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClient(AccountNumber, vClients);
}

// Reads updated client data
sClient ChangeClientRecord(string AccountNumber) {
    sClient Client;
    Client.AccountNumber = AccountNumber;
    cout << "\n\nEnter PinCode: ";
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name: ";
    getline(cin, Client.Name);
    cout << "Enter Phone: ";
    getline(cin, Client.Phone);
    cout << "Enter Account Balance: ";
    cin >> Client.AccountBalance;
    return Client;
}

// Updates a client’s information
void UpdateClient(string AccountNumber, vector <sClient>& vClients) {
    sClient Client;
    char Answer = 'n';
    if (FindClientByAccountNumber(AccountNumber, Client)) {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want to update this client? (y/n): ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y') {
            for (sClient& C : vClients) {
                if (C.AccountNumber == AccountNumber) {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveClientsDataToFile(vClients);
            cout << "\n\nClient Updated Successfully.";
        } else {
            cout << "\n\nUpdate Cancelled.";
        }
    } else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }
}

// Loads client list and updates the selected client
void UpdateClientByAccountNumber() {
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClient(AccountNumber,vClients);
}

// Shows the main menu screen
void ShowMainMenue() {
    cout << "========================================\n";
    cout << "\tMain Menue Screen\n";
    cout << "========================================\n";
    cout << "\t [1] Show Client list.\n";
    cout << "\t [2] Add New Client.\n";
    cout << "\t [3] Delete Client.\n";
    cout << "\t [4] Update Client Info.\n";
    cout << "\t [5] Find Client.\n";
    cout << "\t [6] Exit.\n";
    cout << "========================================\n";
}

// Executes the selected menu option
void ShowOptionOfMenue(short &Option) {
    switch (Option)
    {
    case 1:
        system("cls");
        PrintAllClientsData();
        system("pause");
        break;
    case 2:
        system("cls");
        AddClient();
        system("pause");
        break;
    case 3:
        system("cls");
        DeleteClientByAccountNumber();
        system("pause");
        break;
    case 4:
        system("cls");
        UpdateClientByAccountNumber();
        system("pause");
        break;
    case 5:
        system("cls");
        FindClient();
        system("pause");
        break;
    case 6:
        break;
    }
}

// Runs the main banking menu in a loop until user exits
void StartBank() {
    short Option;
    do {
        system("cls");
        ShowMainMenue();
        cout << "Choose what do you want to do? [1 to 6] : ";
        cin >> Option;
        ShowOptionOfMenue(Option);
    } while (Option != 6);
}

// Main function – program entry point
int main()
{
    StartBank();
    system("pause>0");
    return 0;
}
