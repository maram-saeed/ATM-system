
#include <iostream>
#include <string>
#include <vector>
#include <fstream>


using namespace std;

namespace ATM_Library {

enum enATMMenuOptions {
	 eQuickWithdraw= 1, eNormalWithdraw = 2, eDeposit = 3, 
	 eCheckBalance = 4, eChangePinCode = 5, eShowMyInfo = 6,
	 eLogout = 7
};

void ShowMainMenu();
void ShowQuickWithdrawScreen();
void ShowNormalWithdrawScreen();
void ShangeClientPinCodeScreen();

struct stBankClientData {
	stBankClientData(): MarkForDelete(false) {}
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete;
};

const string ClientsFileName = "Clients.txt";
stBankClientData CurrentClient;

short ReadATMOption() {

	cout<<"Choose what do you want to do? [1 to 7]? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}

string ReadAccountNumber() {
	string AccountNumber = "";

	cout<<"Please Enter Account Number? ";
	cin>> AccountNumber;

	return AccountNumber;
}

double ReadDepositAmount() {
	double Amount = 0;
	cout << "\nEnter a positive Deposit Amount? ";
	cin >> Amount;

	while(Amount <= 0) {
		cout << "\nEnter a positive Deposit Amount? ";
		cin >> Amount;
	}

	return Amount;
}

short ReadQuickWithdrawAmount() {
	short Choice = 0;

	while(Choice < 1 || Choice > 9) 
	{
		cout << "Choose what to do from [1] to [9] ? ";
		cin >> Choice;
	}

	return Choice;
}

int ReadWithdrawAmount() {
	int Amount = 0;

	cout << "\nEnter an amount multiple of 5's ? ";
	cin >> Amount;

	while(Amount % 5 != 0) {
		cout << "\nEnter an amount multiple of 5's ? ";
		cin >> Amount;
	}

	return Amount;
}

void GoBackToMainMenu() {
	cout<<"\n\nPress any key to go back to Main Menu...";
	system("pause>0");
	ShowMainMenu();
}

void PrintClientCard(stBankClientData Client) {

	cout<<"\nThe following are the client details:\n";
	cout<<"--------------------------------\n";
	cout<<"Account Number  : "<<Client.AccountNumber<<endl;
	cout<<"Pin Code        : "<<Client.PinCode<<endl;
	cout<<"Name            : "<<Client.Name<<endl;
	cout<<"Phone           : "<<Client.Phone<<endl;
	cout<<"Account Balance : "<<Client.AccountBalance<<endl;
	cout<<"--------------------------------\n\n";
}

vector<string> SplitEachWordOfString(string Line, string Delimiter) {
	vector<string> vString;
	short pos = 0;
	string sWord;

	while ((pos = Line.find(Delimiter)) != std::string::npos)
	{
		sWord = Line.substr(0, pos);
		if(sWord != "") {
			vString.push_back(sWord);
		}
		Line.erase(0, pos + Delimiter.length()); 
	}
	if(Line != "") {
		 // it count last word of the string.
		vString.push_back(Line);
	}

	return vString;

}

stBankClientData ConvertLineDataToRecord(string Line, string Seperator = "#//#") {
	stBankClientData Client;
	vector<string> vString;

	vString = SplitEachWordOfString(Line, Seperator);

	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.Name = vString[2];
	Client.Phone = vString[3];
	Client.AccountBalance = stod(vString[4]);

	return Client;
}

vector<stBankClientData> LoadClientsDataFromFile(string FileName) {
	fstream MyFile;
	vector<stBankClientData> vClients;
	MyFile.open(FileName, ios::in);

	if(MyFile.is_open()) {
		string Line = "";
		stBankClientData Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineDataToRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}

	return vClients;
}

string ConvertRecordToOneLine(stBankClientData Client, string Seperator = "#//#") {
	string stringClientRecord = "";

	stringClientRecord+= Client.AccountNumber + Seperator;
	stringClientRecord+= Client.PinCode + Seperator;
	stringClientRecord+= Client.Name + Seperator;
	stringClientRecord+= Client.Phone + Seperator;
	stringClientRecord+= std::to_string(Client.AccountBalance);

	return stringClientRecord;
}

void SaveClientsDataToFile(string FileName, vector<stBankClientData> vClients) {
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite

	string DataLine;

	if(MyFile.is_open()) {
		for(stBankClientData C: vClients) {
			if(C.MarkForDelete == false) 
			{
				DataLine = ConvertRecordToOneLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	//return vClients;
}

bool FindClientByAccountNumber(string AccountNumber, vector<stBankClientData> vClients , stBankClientData& Client) {
	for(stBankClientData& C : vClients) 
	{
		if(C.AccountNumber == AccountNumber) 
		{
			Client = C;
			return true;
		}
	}

	return false;
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stBankClientData& Client) {

	vector<stBankClientData> vClients = LoadClientsDataFromFile(ClientsFileName);

	for(stBankClientData& C : vClients) 
	{
		if(C.AccountNumber == AccountNumber && C.PinCode == PinCode ) 
		{
			Client = C;
			return true;
		}
	}

	return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode) {
	if(FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
		return true;
	else
		return false;
}

void Login() {

	bool LoginFaild = false;
	string AccountNumber, PinCode;

	do 
	{
		system("cls");
		cout<<"\n--------------------------------\n";
		cout<<"\tLogin Screen";
		cout<<"\n--------------------------------\n";

		if(LoginFaild)  
		{
			cout << "Invalid Account Number/PinCode!\n";
		}

		cout << "Enter Account Number ? ";
		cin >> AccountNumber;

		cout << "Enter PinCode ? ";
		cin >> PinCode;

		LoginFaild = !LoadClientInfo(AccountNumber, PinCode);

	} while (LoginFaild);

	ShowMainMenu();

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<stBankClientData> vClients) {
	char Answer = 'n';

	cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
	cin >> Answer;

	if(Answer == 'y' || Answer == 'Y') {
		for(stBankClientData& C: vClients) {
			if(C.AccountNumber == AccountNumber) {
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfully. New balance is: "<< C.AccountBalance;
				return true;
			}
		}
	}
	return false;
}

void PerfromDepositOption() {

	double DepositeAmount = ReadDepositAmount();

	vector<stBankClientData> vClients = LoadClientsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositeAmount, vClients);
	CurrentClient.AccountBalance += DepositeAmount;

}

void ShowDepositScreen() {

	system("cls");
	cout<<"\n=================================\n";
	cout<< "\tDeposit Screen\n";
	cout<<"=================================\n";

	PerfromDepositOption();
}

short GetQuickWithdrawAmount(short QuickWithdrawOption) {
	switch (QuickWithdrawOption)
	{
	case 1:
		return 20;
	case 2:
		return 50;
	case 3:
		return 100;
	case 4:
		return 200;
	case 5:
		return 400;
	case 6:
		return 600;
	case 7:
		return 800;
	case 8:
		return 1000;
	default:
		return 0;
	}
}

void PerfromQuickWithdrawOption(short QuickWithdrawOption) {

	if(QuickWithdrawOption == 9)
		return;

	short WithDrawBalance = GetQuickWithdrawAmount(QuickWithdrawOption);

	if(WithDrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make another choice. \n";
		cout << "Press Anykey to continue...";
		system("pause>0");
		ShowQuickWithdrawScreen();
		return;
	}

	vector<stBankClientData> vClients = LoadClientsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);
	CurrentClient.AccountBalance -= WithDrawBalance;
}

void ShowQuickWithdrawScreen() {

	system("cls");
	cout<<"\n=================================\n";
	cout<< "     Quick Withdraw Screen\n";
	cout<<"=================================\n";
	cout << "\t[1] 20\t\t[2] 50\n";
	cout << "\t[3] 100\t\t[4] 200\n";
	cout << "\t[5] 400\t\t[6] 600\n";
	cout << "\t[7] 800\t\t[8] 1000\n";
	cout << "\t[9] Exit\n";
	cout<<"=================================\n";

	cout<< "Your Balance is : " << CurrentClient.AccountBalance <<endl;
	PerfromQuickWithdrawOption(ReadQuickWithdrawAmount());
}

void PerfromNormalWithdrawOption() {
	int WithDrawBalance = ReadWithdrawAmount();

	if(WithDrawBalance > CurrentClient.AccountBalance) {
		cout << "\nThe amount exceeds your balance, make another choice." << endl;
		cout<<"\nPress Anykey to continue...";
		system("pause>0");
		ShowNormalWithdrawScreen();
		return;
	}

	vector<stBankClientData> vClients = LoadClientsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);
	CurrentClient.AccountBalance -= WithDrawBalance;
}

void ShowNormalWithdrawScreen() {

	system("cls");
	cout<<"\n=================================\n";
	cout<< "     Normal Withdraw Screen\n";
	cout<<"=================================\n";
	PerfromNormalWithdrawOption();
}

void ShowCheckBalanceScreen() {

	system("cls");
	cout<<"\n=================================\n";
	cout<< "     Check Balance Screen\n";
	cout<<"=================================\n";
	cout<< "Your Balance is : " << CurrentClient.AccountBalance <<endl;
}

void PerfromChangeClientPinCode(string AccountNumber) {
	short countTries = 0; 
	string OldPinCode, NewPinCode;
	vector<stBankClientData> vClients = LoadClientsDataFromFile(ClientsFileName);

	do 
	{
		cout<< "\nNumber of Tries : " << countTries + 1 << endl;
		cout << "\nEnter Old Pin Code : ";
		cin >> OldPinCode;
		countTries++;

		if(countTries == 3) {
		cout << "\nMax Tries is " << countTries 
			<< " Please enter Pin Code later..." <<endl;
		system("pause>0");
		return;
	}

	} while(OldPinCode != CurrentClient.PinCode && countTries <= 3);


	if(OldPinCode == CurrentClient.PinCode) {
	for(stBankClientData& C: vClients) {
			if(C.AccountNumber == AccountNumber) {
				cout << "\nEnter New Pin Code : ";
				cin >> NewPinCode;
				C.PinCode = NewPinCode;
				SaveClientsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfully. New Pin Code is: "<< C.PinCode;
				//return true;
			}
		}
		CurrentClient.PinCode = NewPinCode;
	}
}

void ShangeClientPinCodeScreen() {
	system("cls");
	cout<<"\n=================================\n";
	cout<< "     Change My Pin Code Screen\n";
	cout<<"=================================\n";
	PerfromChangeClientPinCode(CurrentClient.AccountNumber);
}

void ShowClientInfoScreen() {
	system("cls");
	cout<<"\n=================================\n";
	cout<< "     My Inforamation Screen\n";
	cout<<"=================================\n";
	PrintClientCard(CurrentClient);
}

void ChangeLanguageScreen() {
	system("cls");
	cout<<"\n=================================\n";
	cout<< "     Change Language Screen\n";
	cout<<"=================================\n";
}

void PerfromMainMenueOption(enATMMenuOptions MainMenueOption) {
	switch (MainMenueOption)
	{

	case enATMMenuOptions::eQuickWithdraw:
		system("cls");
		ShowQuickWithdrawScreen();
		GoBackToMainMenu();
		break;

	case enATMMenuOptions::eNormalWithdraw:
		system("cls");
		ShowNormalWithdrawScreen();
		GoBackToMainMenu();
		break;

	case enATMMenuOptions::eDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToMainMenu();
		break;

	case enATMMenuOptions::eCheckBalance:
		system("cls");
		ShowCheckBalanceScreen();
		GoBackToMainMenu();
		break;

	case enATMMenuOptions::eChangePinCode:
		system("cls");
		ShangeClientPinCodeScreen();
		GoBackToMainMenu();
		break;

	case enATMMenuOptions::eShowMyInfo:
		system("cls");
		ShowClientInfoScreen();
		GoBackToMainMenu();
		break;

	case enATMMenuOptions::eLogout:
		system("cls");
		Login();
		break;
	}
}

void ShowMainMenu() {

	system("cls");
	cout<<"=================================================\n";
	cout<<"\t\ATM Main Menu Screen\n";
	cout<<"=================================================\n";
	cout<<"\t[1] Quick Withdraw.\n";
	cout<<"\t[2] Normal Withdraw.\n";
	cout<<"\t[3] Deposit.\n";
	cout<<"\t[4] Check Balance.\n";
	cout<<"\t[5] Change My Pin Code.\n";
	cout<<"\t[6] Show My Information.\n";
	cout<<"\t[7] Logout.\n";
	cout<<"=================================================\n";
	PerfromMainMenueOption((enATMMenuOptions)ReadATMOption());
}
}