#include <fstream>  // Required for file operations
#include <iomanip>  // Required for setw
#include <string>
#include <vector>
#include <iostream>
using namespace std;

const string clientsFileName = "Clients.txt";

enum enChoiceOperation { ShowClientsData = 1, AddClient = 2, DeleteClient = 3, UpdateClient = 4, FindClient = 5, Transactions=6, Exit = 7 };
enum enTransactions { Deposit = 1, WithDraw = 2, TotalBalances=3,MainMenu=4 };

struct stClientData {
	string accountNumber;
	string pinCode;
	string clientName;
	string phoneNumber;
	double  accountBalance;
	bool markForDelete = false;// This is to show witch a member/client will delete.
};

//TopMain and Transactions Screens
void printMainMenue()
{
	cout << "==============================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "==============================================\n";
	cout << "[1] Show Client List.\n";
	cout << "[2] Add New Client.\n";
	cout << "[3] Delete Client.\n";
	cout << "[4] Update Client Info.\n";
	cout << "[5] Find Client.\n";
	cout << "[6] Transactions.\n";
	cout << "[7] Exit\n";
	cout << "==============================================\n";
}
enChoiceOperation readMainMenueChoice(short from, short to)
{
	short choice;
	do {
		cout << " Choose What do you want to do? [ 1 to 7]?  ";
		cin >> choice;
	} while (!(choice >= from && choice <= to));

	return (enChoiceOperation)choice;
}

enTransactions readTransactionsOption(short from, short to)
{
	short choice;
	do {
		cout << " Choose What do you want to do? [ 1 to 4]?  ";
		cin >> choice;
	} while (!(choice >= from && choice <= to));

	return (enTransactions)choice;
}

void printTransactionsMenue()
{
	system("cls");
	cout << "==============================================\n";
	cout << "\t\tTransactions Menue Screen\n";
	cout << "==============================================\n";
	cout << "[1] Deposit.\n";
	cout << "[2] Withdraw.\n";
	cout << "[3] Total Balance.\n";
	cout << "[4] Main Menu.\n";
	cout << "==============================================\n";
}

//required Functions to Rest of program
string convertRecordToLine(stClientData client, string seperator = "#//#")
{
	string stClientRecord = "";

	stClientRecord += client.accountNumber + seperator;
	stClientRecord += client.pinCode + seperator;
	stClientRecord += client.clientName + seperator;
	stClientRecord += client.phoneNumber + seperator;
	stClientRecord += to_string(client.accountBalance);

	return stClientRecord;
}
vector<string> splitString(string S1, string delimiter)
{
	short pos = 0; // to get delimiter position(index).

	string delim = delimiter; //to delete it in String step by step and find the next delimiter in string

	string sWord = ""; // to store the words.

	vector<string> vS1;
	while ((pos = S1.find(delim)) != string::npos) // npos= no position if there isn't a delimiter
	{
		sWord = S1.substr(0, pos);

		if (sWord != "")
			vS1.push_back(sWord); // Add value to the vector

		S1.erase(0, pos + delim.length());
	}

	if (S1 != "")
		vS1.push_back(S1); // Add value to the vector

	return vS1;
}
stClientData convertLineDataToRecord(string S1, string seperator = "#//#")
{
	stClientData client;

	vector <string> vClient = splitString(S1, seperator);

	client.accountNumber = vClient[0];
	client.pinCode = vClient[1];
	client.clientName = vClient[2];
	client.phoneNumber = vClient[3];
	client.accountBalance = stod(vClient[4]);

	return client;
}
// to get all client data from file and save them in one vector as a type stClientData.
vector <stClientData> loadAllClientsDataFromFile(string fileName)
{
	fstream MyFile;
	vector <stClientData> clientsData;

	MyFile.open(fileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		while (getline(MyFile, Line))
		{
			clientsData.push_back(convertLineDataToRecord(Line));
		}
	}

	MyFile.close();

	return clientsData;
}

// (to get all clients Data from File and print them on Screen)
void printClientDataFormat2(stClientData stClient)
{
	cout << left << setw(23) << stClient.accountNumber;
	cout << left << setw(20) << stClient.pinCode;
	cout << left << setw(40) << stClient.clientName;
	cout << left << setw(20) << stClient.phoneNumber;
	cout << left << setw(15) << stClient.accountBalance;
}
void printAllClientsData(vector <stClientData>  clients)
{
	cout << "\t\t\t\t\t\tClient List(" << clients.size() << ") Client(s).\n";
	cout << "----------------------------------------------------------------------------------------------------------------\n";
	cout << left << setw(23) << "| Account Number";
	cout << left << setw(20) << "| Pin Code";
	cout << left << setw(40) << "| Client Name";
	cout << left << setw(20) << "| phone";
	cout << left << setw(15) << "| Balance";
	cout << "\n----------------------------------------------------------------------------------------------------------------\n\n";

	for (stClientData& stClient : clients)
	{
		printClientDataFormat2(stClient);
		cout << endl;
	}
	cout << "\n----------------------------------------------------------------------------------------------------------------\n";

}

//(to read clients Data and save them in File)
bool checkClientByAccountNumber(string accountNumber, vector <stClientData> vClients)
{
	// I take it later by Parametre
	//vector <stClientData> vClients = loadAllClientsDataFromFile(clientsFileName);

	for (stClientData C : vClients)
	{
		if (C.accountNumber == accountNumber)
		{
			return true;
		}
	}
	return false;
}
void addDataLineToFile(string fileName, string stdataLine)
{
	fstream MyFile;
	MyFile.open(fileName, ios::out | ios::app); //ios::out | ios::app

	if (MyFile.is_open())
	{
		MyFile << stdataLine << endl;

		MyFile.close();
	}

}
stClientData readClientData()
{
	stClientData client;

	// Usage of std::ws will extract allthe whitespace character
	cout << "Enter Account Number? ";
	getline(cin >> ws, client.accountNumber);
	
	vector <stClientData> vClients = loadAllClientsDataFromFile(clientsFileName);

	if (!checkClientByAccountNumber(client.accountNumber, vClients))
	{
		cout << "Enter PinCode? ";
		getline(cin, client.pinCode);
		cout << "Enter Name? ";
		getline(cin, client.clientName);
		cout << "Enter Phone? ";
		getline(cin, client.phoneNumber);
		cout << "Enter AccountBalance? ";
		cin >> client.accountBalance;

		return client;
	}
	else
	{
		cout << "\nClient with ["<<client.accountNumber<<"] already exists, Enter another account Number...\n";
		return readClientData();
	}
}
void addNewClient()
{
	stClientData client;
	client = readClientData();
	addDataLineToFile(clientsFileName, convertRecordToLine(client));
}
void addClients()
{
	char addMore = 'Y';
	do {
		system("cls");
		cout << "\n---------------------------------------\n";
		cout << "\t Add New Client Screen";
		cout << "\n---------------------------------------\n\n";
		addNewClient();
		cout << "\nClient added successfully, Do you want to add more clients? y/n? \n";
		cin >> addMore;
	} while (toupper(addMore) == 'Y');
}

// (find client From File by AccountNumber)
bool findClientByAccountNumber(string accountNumber, vector <stClientData> vClients, stClientData& client)
{
	// I take it later by Parametre
	//vector <stClientData> vClients = loadAllClientsDataFromFile(clientsFileName);

	for (stClientData C : vClients)
	{
		if (C.accountNumber == accountNumber)
		{
			client = C;
			return true;
		}
	}
	return false;
}
string readClientAccountNumber()
{
	string accountNumber;
	cout << "Please Enter AccountNumber: ";
	cin >> accountNumber;

	return accountNumber;
}
void printClientData(stClientData client)
{
	cout << "\n\nThe following is the extracted client record: \n";

	cout << "\nAccount Number  :" << client.accountNumber;
	cout << "\nPin code        :" << client.pinCode;
	cout << "\nName            :" << client.clientName;
	cout << "\nPhone           :" << client.phoneNumber;
	cout << "\nAccount Palance :" << client.accountBalance;
}
void showFindClient(vector<stClientData> vClients)
{
	system("cls");
	cout << "\n---------------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n---------------------------------------\n\n";

	string accountNumber = readClientAccountNumber();
	stClientData client;
	if (findClientByAccountNumber(accountNumber, vClients, client))
		printClientData(client);
	else
		cout << "\nClient with Account Number (" << accountNumber << ") Not found!";
}

// (Delete a Client from File)

bool martkClientForDeleteByAccountNumber(string accountNumber, vector <stClientData>& vClients)
{
	for (stClientData& client : vClients)
	{
		if (client.accountNumber == accountNumber)
		{
			client.markForDelete = true;
			return true;
		}
	}

	return false;
}
void saveClientsDataToFile(string FileName, vector <stClientData> vClinets)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	if (MyFile.is_open())
	{
		for (stClientData& C : vClinets)
		{
			if (C.markForDelete == false)
			{
				MyFile << convertRecordToLine(C) << endl;
			}
		}

		MyFile.close();
	}
}
bool deleteClientByAccountNumber(string accountNumber, vector <stClientData> vClients)
{
	stClientData client;
	if (findClientByAccountNumber(accountNumber, vClients, client))
	{
		printClientData(client);

		char choice;
		cout << "\n\nAre you sure you want delete this Client? y/n ?";
		cin >> choice;
		if (toupper(choice) == 'Y')
		{
			martkClientForDeleteByAccountNumber(accountNumber, vClients);
			saveClientsDataToFile(clientsFileName, vClients);

			//refresh Clients
			vClients = loadAllClientsDataFromFile(clientsFileName);

			cout << "\n\nClient deleted Successfully.\n\n";

			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << accountNumber << ") Not found!\n\n";
		return false;

	}
}
void showDeleteClient(vector <stClientData> vClients) 
{
	system("cls");
	cout << "\n---------------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n---------------------------------------\n\n";
	string accountNumber = readClientAccountNumber();
	deleteClientByAccountNumber(accountNumber, vClients);
}

// Update Client Data By Account Number
stClientData ChangeClientRecord(string accountNumber)
{
	stClientData C;
	C.accountNumber = accountNumber;
	// Usage of std::ws will extract allthe whitespace character
	cout << "Enter PinCode? ";
	getline(cin >> ws, C.pinCode);
	cout << "Enter Name? ";
	getline(cin, C.clientName);
	cout << "Enter Phone? ";
	getline(cin, C.phoneNumber);
	cout << "Enter AccountBalance? ";
	cin >> C.accountBalance;
	return C;


}
bool updateClientByAccountNumber(string accountNumber, vector <stClientData>& vClients)
{
	stClientData client;
	if (findClientByAccountNumber(accountNumber, vClients, client))
	{
		printClientData(client);

		char choice;
		cout << "\n\nAre you sure you want to update this Client? y/n ?";
		cin >> choice;
		if (toupper(choice) == 'Y')
		{
			for (stClientData& C : vClients)
			{
				if (C.accountNumber == accountNumber)
				{
					C = ChangeClientRecord(accountNumber);
					break; // to not check all lines of file
				}
			}			saveClientsDataToFile(clientsFileName, vClients);

			//refresh Clients
			vClients = loadAllClientsDataFromFile(clientsFileName);

			cout << "\n\nClient Update Successfully.\n\n";

			return true;
		}

	}
	else
	{
		cout << "\nClient with Account Number (" << accountNumber << ") Not found!\n\n";
		return false;
	}
}
void showUpdateClient(vector <stClientData> vClients)
{
	system("cls");
	cout << "\n---------------------------------------\n";
	cout << "\tUpdate Client Screen";
	cout << "\n---------------------------------------\n\n";
	string accountNumber = readClientAccountNumber();
	updateClientByAccountNumber(accountNumber, vClients);
}

//Transactions Section

//Deposit Section
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <stClientData>& vClients)
{
	char Answer = 'n';

	cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{

		for (stClientData& C : vClients)
		{
			if (C.accountNumber == AccountNumber)
			{
				C.accountBalance += Amount;
				saveClientsDataToFile(clientsFileName, vClients);
				cout << "\n\nDone Successfully. New balance is: " << C.accountBalance<<endl;

				return true;
			}

		}
		return false;
	}

}
void showDepositSecreen(vector <stClientData>& vClients)
{
	system("cls");
	cout << "\n---------------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n---------------------------------------\n\n";

	string 	 accountNumber = readClientAccountNumber();
	stClientData client;

	while (!findClientByAccountNumber(accountNumber, vClients, client))
	{
	cout << "Client With [" << accountNumber << "] does not exist.";
	 accountNumber = readClientAccountNumber();
	};

	printClientData(client);

	int depositAmount;
	cout << "\nPlease Enter deposit amount? ";
	cin >> depositAmount;

	DepositBalanceToClientByAccountNumber(accountNumber, depositAmount, vClients);
}
//Withdraw Section
void showWithdrawSecreen(vector <stClientData>& vClients)
{
	system("cls");
	cout << "\n---------------------------------------\n";
	cout << "\Withdraw Screen";
	cout << "\n---------------------------------------\n\n";

	string 	 accountNumber = readClientAccountNumber();
	stClientData client;

	while (!findClientByAccountNumber(accountNumber, vClients, client))
	{
		cout << "Client With [" << accountNumber << "] does not exist.";
		accountNumber = readClientAccountNumber();
	};

	printClientData(client);

	int withdrawAmount;
	cout << "\nPlease Enter Withdraw amount? ";
	cin >> withdrawAmount;

	while (withdrawAmount > client.accountBalance)
	{
		cout << "\nAmount Exceeds The Balance, you can withdraw up to: " << client.accountBalance;
		cout << "\nPlease Enter Withdraw amount? ";
		cin >> withdrawAmount;
	}

	DepositBalanceToClientByAccountNumber(accountNumber, withdrawAmount*-1, vClients);
}

//Total Balance section
void printClientDataFormat3(stClientData stClient)
{
	cout << left << setw(23) << stClient.accountNumber;
	cout << left << setw(40) << stClient.clientName;
	cout << left << setw(15) << stClient.accountBalance;
}
void printTotalBalanceScreen(vector <stClientData>  clients)
{
	cout << "\t\t\t\t\t\tClient List(" << clients.size() << ") Client(s).\n";
	cout << "------------------------------------------------------------------------------------------------------\n";
	cout << left << setw(23) << "| Account Number";
	cout << left << setw(40) << "| Client Name";
	cout << left << setw(15) << "| Balance";
	cout << "\n------------------------------------------------------------------------------------------------------\n\n";

	for (stClientData& stClient : clients)
	{
		printClientDataFormat3(stClient);
		cout << endl;
	}
	cout << "\n------------------------------------------------------------------------------------------------------\n";

}

void showTotalBalanceScreen(vector <stClientData> vClients)
{
	system("cls");
	printTotalBalanceScreen(vClients);

	int totalBalances = 0;
	for (stClientData C : vClients)
	{
		totalBalances += C.accountBalance;

	}	cout << "\t\t\t\tTotal Balances = " << totalBalances << endl;
}

void showTransactionsScreen()
{
	vector <stClientData> vClients = loadAllClientsDataFromFile(clientsFileName);
	enTransactions choice;
	do {
		printTransactionsMenue();
	    choice = readTransactionsOption(1, 4);

		switch (choice)
		{
		case Deposit:
		{
			showDepositSecreen(vClients);
			system("pause");
			break;
		}
		case WithDraw:
		{
			showWithdrawSecreen(vClients);
			system("pause");
			break;
		}
		case TotalBalances:
		{
			showTotalBalanceScreen(vClients);
			system("pause");
			break;
		}
		case  MainMenu:
			break;
		}
	} while (choice != enTransactions::MainMenu);
}

//Programs End
void showProgramsEnd()
{
	system("cls");
	cout << "\n---------------------------------------\n";
	cout << "\tPrograms End :-) ";
	cout << "\n---------------------------------------\n\n";
}
void getChoicedOperation(enChoiceOperation choice)
{
	vector <stClientData> vClients = loadAllClientsDataFromFile(clientsFileName);

	switch (choice)
	{
	case ShowClientsData:
	{
		printAllClientsData(vClients);
		system("pause");
		break;
	}
	case AddClient:
	{
		addClients();
		system("pause");
		break;
	}
	case DeleteClient:
	{
		showDeleteClient(vClients);
		system("pause");
		break;
	} 
	case UpdateClient:
	{
		showUpdateClient(vClients);
		system("pause");
		break;
	} 
	case FindClient:
	{
		showFindClient(vClients);
		system("pause");
		break;
	}

	case Transactions:
	{
		showTransactionsScreen();
		break;
	}
	case Exit:
		showProgramsEnd();
		break;
	}
}
void mainMenueOperation()
{
	enChoiceOperation choice;

	do {
		system("cls");
		printMainMenue();
	    choice = readMainMenueChoice(1, 7);
		getChoicedOperation(choice);
	} while (choice != 7);

}
int main()
{
	mainMenueOperation();
	
	system("pause");

}

