#include <iostream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <conio.h>
#include <ctime>
#include <chrono>
#include <thread>
using namespace std;

/* Exceptii:
 * 1. Verificare format invalid - data de nastere, adresa de mail;
 * 2. Verificare nullptr
 * 3. Data calendaristica invalida
*/

class checkDateFormat: public exception {
public:
    virtual const char* what() const throw ()
    {return "\nThe format of your date is invalid - please input a date with the folowing format: dd/mm/yyyy\n";}
} e1;

void checkBirthdateFormat(string birthdate) {
    bool valid_format = true;
    if (birthdate.size() != 10) {
        valid_format = false;
    } else if (birthdate[2] != '/' || birthdate[5] != '/') {
        valid_format = false;
    } else {
        for (int i = 0; i < 10; i++) {
            if (i == 2 || i == 5) {
                if (birthdate[i] != '/') {
                    valid_format = false;
                    break;
                }
            } else {
                if (!isdigit(birthdate[i])) {
                    valid_format = false;
                    break;
                }
            }
        }
    }
    if (!valid_format) {
        throw e1;
    }
}

class InvalidMailFormat: public exception {
public:
    virtual const char* what() const throw ()
    {return "\nThe format of your email address is invalid - please input a mail address such that it ends with @dmail.com\n";}
} e2;

bool checkMailFormat(string mail){
    if (mail.size() < 11) throw e2;
    if (mail.substr(mail.size()-10) != "@dmail.com") throw e2;
    return true;
}

class Nullpointer: public exception {
public:
    virtual const char* what() const throw() {return "Error: existence of a null pointer.";}
} e4;

class InvalidHourFormat: public exception {
public:
    virtual const char* what() const throw ()
    {return "\nThe format of your time is invalid - please input a time with the folowing format: HH:MM\n";}
} e3;
class Account{
private:
    map <string, string> personalInfo = {{"1) First Name: ", ""}, {"2) Surname: ", ""}, {"3) Birthdate (dd/mm/yyyy): ", ""}, {"4) Mail Address: ", ""}, {"5) Password: ", ""}};
public:
    string getMailAddress() const {return this->personalInfo.at("4) Mail Address: ");}
    string getPassword() const {return this->personalInfo.at("5) Password: ");}
    string getFirstName() const {return this->personalInfo.at("1) First Name: ");}
    string getSurname() const {return this->personalInfo.at("2) Surname: ");}
    string getBirthdate() const {return this->personalInfo.at("3) Birthdate (dd/mm/yyyy): ");}
    void setMailAddress(string input) {
        checkMailFormat(input);
        this->personalInfo["4) Mail Address: "] = input;
    }

    // Constructorul cu toti parametrii
    Account(string firstName, string surname, string birthdate, string mailAddress, string password) {
        this->personalInfo["1) First Name: "] = firstName;
        this->personalInfo["2) Surname: "] = surname;
        try {
            checkBirthdateFormat(birthdate);
        } catch(checkDateFormat& e1) {cout << e1.what() << endl;}
        this->personalInfo["3) Birthdate (dd/mm/yyyy): "] = birthdate;
        try {
            checkMailFormat(mailAddress);
        } catch(InvalidMailFormat& e2) {cout << e2.what() << endl;}
        this->personalInfo["4) Mail Address: "] = mailAddress;
        this->personalInfo["5) Password: "] = password;
    }

    // Constructorul fara parametri
    Account() {
        this->personalInfo["1) First Name: "] = "(Empty)";
        this->personalInfo["2) Surname: "] = "(Empty)";
        this->personalInfo["3) Birthdate (dd/mm/yyyy): "] = "dd/mm/yyyy";
        this->personalInfo["4) Mail Address: "] = "(Empty)";
        this->personalInfo["5) Password: "] = "(Empty)";
    }

    // Constructor de copiere
    Account(const Account& a){
        for(auto it = this->personalInfo.begin(); it != this->personalInfo.end(); ++it)
            for(auto it2 = a.personalInfo.begin(); it2 != a.personalInfo.end(); ++it2)
            {
                if(it->first == it2->first)
                    it->second = it2->second;
            }
    }

    // Operator =
    Account& operator=(const Account &a){
        if (this != &a)
        {
            for(auto it = this->personalInfo.begin(); it != this->personalInfo.end(); ++it)
                for(auto it2 = a.personalInfo.begin(); it2 != a.personalInfo.end(); ++it2)
                {
                    if(it->first == it2->first)
                        it->second = it2->second;
                }
        }
        return *this;
    }

    bool operator==(const Account &a){
        if (this != &a) return false;
        else {
            if(this->personalInfo["1) First Name: "] != a.getFirstName()) return false;
            if(this->personalInfo["2) Surname: "] != a.getSurname()) return false;
            if(this->personalInfo["3) Birthdate (dd/mm/yyyy): "] != a.getBirthdate()) return false;
            if(this->personalInfo["4) Mail Address: "] != a.getMailAddress()) return false;
            if(this->personalInfo["5) Password: "] != a.getPassword()) return false;
        }
        return true;
    }

    bool operator>(const Account &a) {
        if(this->personalInfo["2) Surname: "] > a.getSurname()) return true;
        else {
            if(this->personalInfo["1) First Name: "] > a.getFirstName()) return true;
        }
        return false;
    }

    istream& read(istream& in)
    {
        string birthdate, mail; bool ok = false; bool ok2 = false;
        for(auto it = personalInfo.begin(); it != personalInfo.end(); ++it)
        {
            cout << it->first;
            if(it->first == "3) Birthdate (dd/mm/yyyy): ")
            {
                while(!ok){
                    try{
                        cin >> birthdate;
                        checkBirthdateFormat(birthdate);
                        it->second = birthdate;
                        ok = true;
                    } catch(checkDateFormat& e1) {
                        cout << e1.what();
                        cout << "3) Birthdate (dd/mm/yyyy): ";
                        ok = false;
                    }
                }
            } else if (it->first == "4) Mail Address: ")
            {
                while(!ok2) {
                    try{
                        cin >> mail;
                        if (checkMailFormat(mail)) {
                            it->second = mail;
                            ok2 = true;
                        }
                    } catch(InvalidMailFormat& e2) {
                        cout << e2.what();
                        cout << "4) Mail Address: ";
                    }
                }
            } else in >> it->second;
        }
        return in;
    }



    // Supraincarcarea operatorului de citire
    friend istream& operator>> (istream& in, Account& a){return a.read(in);}

    // Afisare
    ostream& write(ostream& out) const
    {
        for(auto it = personalInfo.begin(); it != personalInfo.end(); ++it)
            out << it->first << " " << it->second << endl;
        return out;
    }

    // Supraincarcarea operatorului de afisare
    friend ostream& operator<< (ostream& out, Account& a){return a.write(out);}

    // Destructorul
    ~Account(){}
};

// CLASA TEMPLATE - am gasit pe Internet ideea de a implementa set-ul cu ajutorul unui vector - grija la set ramane SORTAREA si UNICITATEA elementelor;
template <typename T>
class Set{
private:
    vector<T> data;
public:
    Set(vector<T> data) {this->data = data;}
    Set() {}

    // METODA template #1: verificarea existentei unui element in Set
    bool find(const T& element) {
        for (auto it = data.begin(); it != data.end(); ++it)
            if((Account) element == *it) return true;
        return false;
    }
    // METODA template #2: inserarea unui element intr-un Set
    void insert(const T& element) {
        if(!find(element)) {
            size_t i = 0;
            for(; i < data.size(); i++){
                // Trebuie sa ne asiguram ca avem elemente sortate in set
                if(data[i] > element) break;
            }
            data.insert(data.begin()+i, element);
        }
    }

    // METODA template #3: stergerea unui element din Set
    void erase(const T& element) {
        for (auto it = data.begin(); it != data.end(); ++it)
            if(element == it)
                data.erase(it);
    }

    Set<T>& operator= (const Set<T>& s) {
        if(this != &s){
            this->data.clear();
            for (auto it = s.data.begin(); it != s.data.end(); it++)
                this->insert(*it);
        }
        return *this;
    }

    // Begin
  //  auto begin() {return data.begin();}

    // End
   // auto end() {return data.end();}

    // Clear
  //  void clear() {data.clear();}
};

class Mail{
protected:
    Account sender;
    Account recipient;
    string subject;
    string textBody;
    bool seen = false;
public:
    bool getSeen() const {return this->seen;}
    string getSenderMailAddress() const {return this->sender.getMailAddress();}
    string getRecipientMailAddress() const {return this->recipient.getMailAddress();}
    void setRecipientMailAddress(string m) {this->recipient.setMailAddress(m);}
    void setSenderMailAddress(string m) {this->sender.setMailAddress(m);}
    string getTextBody() const {return this->textBody;}
    string getSubject() const {return this->subject;}
    void setSeen(bool b) {this->seen = b;}

    // Constructorul cu toti parametrii
    Mail(Account sender, Account recipient, string subject, string textBody, bool seen){
        this->sender = sender;
        this->recipient = recipient;
        this->subject = subject;
        this->textBody = textBody;
        this->seen = seen;
    }

    // Constructorul fara parametri
    Mail(){
        this->sender = Account();
        this->recipient = Account();
        this->textBody = "";
        this->subject = "";
        this->seen = false;
    }

    // Constructorul de copiere
    Mail(const Mail& m){
        this->sender = m.sender;
        this->recipient = m.recipient;
        this->textBody = m.textBody;
        this->subject = m.subject;
        this->seen = m.seen;
    }

    // Operatorul =
    Mail& operator=(const Mail &m) {
        if (this != &m)
        {
            this->sender = m.sender;
            this->recipient = m.recipient;
            this->textBody = m.textBody;
            this->subject = m.subject;
            this->seen = m.seen;
        }
        return *this;
    }

    istream& read(istream& in)
    {
        cout << "\nTo: ";
        string inputRecipientAddress;
        in >> inputRecipientAddress;
        this->recipient.setMailAddress(inputRecipientAddress);
        cout << "\nSubject: ";
        in.ignore(); getline(in, this->subject, '\n');
        cout << "\nCompose a message bellow:\n";
        getline(in, this->textBody, '\n');
        cout << endl;
        return in;
    }

    // Supraincarcarea operatorului de citire
    friend istream& operator>> (istream& in, Mail& m){return m.read(in);}

    // Afisare
    ostream& write(ostream& out) const
    {
        out << "\nFrom: " << this->sender.getMailAddress();
        out << "\nTo: " << this->recipient.getMailAddress();
        out << "\nSubject: " << this->subject;
        out << endl << "-------------------------------------------" << endl;
        out << this->textBody << endl;
        return out;
    }

    // Supraincarcarea operatorului de afisare
    friend ostream& operator<< (ostream& out, const Mail& m){return m.write(out);}

    // Destructorul
    ~Mail(){}
};

class ScheduledMail: public Mail{
protected:
    tm scheduledSendTime;
public:
    void setScheduledYear(int year) {scheduledSendTime.tm_year = year - 1900;}
    void setScheduledMonth(int month) {scheduledSendTime.tm_mon = month - 1;}
    void setScheduledDay(int day) {scheduledSendTime.tm_mday = day;}
    void setScheduledHour(int hour) {scheduledSendTime.tm_hour = hour;}
    void setScheduledMinute(int minute) {scheduledSendTime.tm_min = minute;}
    tm getscheduledSendTime() {return this->scheduledSendTime;}
    // Constructorul cu toti parametrii
    ScheduledMail(Account sender, Account recipient, string subject, string textBody, bool seen, tm scheduledSendTime): Mail(sender, recipient, subject, textBody, seen){this->scheduledSendTime = scheduledSendTime;}

    // Constructorul fara parametri
    ScheduledMail(): Mail() {
        scheduledSendTime.tm_year = 123;  // 2023-1900
        scheduledSendTime.tm_mon = 0;    // Jan = 0; Feb = 1 ...
        scheduledSendTime.tm_mday = 1;  // prima zi a lunii
        scheduledSendTime.tm_hour = 0; // ora 0 (0-23)
        scheduledSendTime.tm_min = 0; // min 0
        scheduledSendTime.tm_sec = 0;// sec 0
    }

    // Constructorul de copiere
    ScheduledMail(const ScheduledMail& s): Mail(s) {
        this->scheduledSendTime = s.scheduledSendTime;
    }

    // Operatorul =
    ScheduledMail& operator= (const ScheduledMail& s){
        if (this != &s){
            Mail::operator=(s);
            this->scheduledSendTime = s.scheduledSendTime;
        }
        return *this;
    }

    istream& read(istream& in)
    {
        string dateInput, timeInput; bool ok = false, ok1 = false;
        Mail::read(in);
        cout << "\nPlease write the date (format: dd/mm/yyyy) and hour-time (format: HH:MM) for the time you prefer this dmail to be send:";
        cout << "\nDate: ";
        // Folosesc cunostintele de la Sisteme de Operare ✨ am invatat cateva functii in C si le pot integra aici
        while(!ok)
        try {
            in >> dateInput;
            if (sscanf(dateInput.c_str(), "%d/%d/%d", &this->scheduledSendTime.tm_mday, &this->scheduledSendTime.tm_mon,&this->scheduledSendTime.tm_year) != 3)
                throw e1;
            ok = true;
        }catch (checkDateFormat& e1) {cout << e1.what() << "\nDate: "; ok = false;}
        // Facem modificarile necesare pentru ca calculatorul sa poata intelege luna si anul curecte
        this->scheduledSendTime.tm_mon--;
        this->scheduledSendTime.tm_year -= 1900;
        cout << "\nTime: ";
        while(!ok1) {
            try {
                in >> timeInput;
                if (sscanf(timeInput.c_str(), "%d:%d", &this->scheduledSendTime.tm_hour,
                           &this->scheduledSendTime.tm_min) != 2)
                    throw e3;
                ok1 = true;
            } catch (InvalidHourFormat &e3) {
                cout << e3.what() << "\nTime: ";
                ok1 = false;
            }
        }
        return in;
    }
    friend istream& operator>> (istream& in, ScheduledMail& s) {return s.read(in);}

    ostream& write(ostream& out) const {
        out << "\n\tThe mail below is SCHEDULED to be send at: " << this->scheduledSendTime.tm_mday << " " << this->scheduledSendTime.tm_mon << " " << this->scheduledSendTime.tm_year << ", " << this->scheduledSendTime.tm_hour <<":" << this->scheduledSendTime.tm_min << endl;
        Mail::write(out);
        return out;
    }

    friend ostream& operator<< (ostream& out, const ScheduledMail& s) {return s.write(out);}

    virtual ~ScheduledMail() = default;
};

class SentMail: public ScheduledMail {
private:
    bool readByRecipient;
public:
    void setReadByRecipient(bool b) {this->readByRecipient = b;}
    bool getReadbyRecipient() const {return this->readByRecipient;}

    // Constructorul cu toti parametri
    SentMail(Account sender, Account recipient, string subject, string textBody, bool seen, tm scheduledSendTime, bool readByRecipient): ScheduledMail(sender, recipient, subject, textBody, seen, scheduledSendTime) {this->readByRecipient = readByRecipient;}

    // Constructorul fara parametri
    SentMail(): ScheduledMail() {this->readByRecipient = false;}

    // Constructor copiere
    SentMail(const SentMail& s): ScheduledMail(s) {this->readByRecipient = s.readByRecipient;}
    // Operatorul =
    SentMail& operator=(const SentMail& s) {
        if(this != &s) {
            ScheduledMail::operator=(s);
            this->readByRecipient = s.readByRecipient;
        }
        return *this;
    }

    istream& read(istream& in){ScheduledMail::read(in); return in;}
    friend istream& operator>> (istream&in, SentMail& s) {return s.read(in);}
    // Afisare
    ostream& write(ostream& out)const {Mail::write(out); return out;}
    friend ostream& operator<< (ostream& out, const SentMail& s) {return s.write(out);}

    ~SentMail(){}
};

class AccountMailBox {
private:
    Account account;
    Set<Account> contacts;
    list<Mail> mailbox;
    list<ScheduledMail> scheduledMailList;
    list<SentMail> sentMailList;
public:
    Account getAccount() const { return account; }
    void setAccount(const Account& newAccount) { account = newAccount; }
    void showSentMailList() {
        for(auto it = sentMailList.begin(); it != sentMailList.end(); ++it)
            cout << *it << endl;
    }
    list<ScheduledMail> getScheduledMailList() {return this->scheduledMailList;}

    // Constructorul cu toti parametri
    AccountMailBox(Account account, Set<Account> contacts, list<Mail> mailbox, list<ScheduledMail> scheduledMailList, list<SentMail> sentMailList) {
        this->account = account;
        this->contacts = contacts;
        for (auto it = mailbox.begin(); it != mailbox.end(); ++it)
                this->mailbox.push_back(*it);
        for (auto it = scheduledMailList.begin(); it != scheduledMailList.end(); ++it)
            this->scheduledMailList.push_back(*it);
        for (auto it = sentMailList.begin(); it != sentMailList.end(); ++it)
            this->sentMailList.push_back(*it);
    }
    AccountMailBox() {this->account = Account();}
    AccountMailBox(const AccountMailBox& acc) {
        this->account = acc.account;
        this->mailbox.clear();
        for (auto it = this->mailbox.begin(); it != this->mailbox.end(); ++it)
            this->mailbox.push_back(*it);
        this->scheduledMailList.clear();
        for (auto it = this->scheduledMailList.begin(); it != this->scheduledMailList.end(); ++it)
            this->scheduledMailList.push_back(*it);
        this->sentMailList.clear();
        for (auto it = this->sentMailList.begin(); it != this->sentMailList.end(); ++it)
            this->sentMailList.push_back(*it);
        this->contacts.clear();
        for (auto it = this->contacts.begin(); it != this-> contacts.end(); ++it)
            this->contacts.insert(*it);
    }

    AccountMailBox& operator=(const AccountMailBox& acc){
        if(this != &acc) {
            this->account = acc.account;
            this->mailbox.clear();
            for (auto it = this->mailbox.begin(); it != this->mailbox.end(); ++it)
                this->mailbox.push_back(*it);
            this->scheduledMailList.clear();
            for (auto it = this->scheduledMailList.begin(); it != this->scheduledMailList.end(); ++it)
                this->scheduledMailList.push_back(*it);
            this->sentMailList.clear();
            for (auto it = this->sentMailList.begin(); it != this->sentMailList.end(); ++it)
                this->sentMailList.push_back(*it);
            this->contacts.clear();
            for (auto it = this->contacts.begin(); it != this-> contacts.end(); ++it)
                this->contacts.insert(*it);
        }
        return *this;
    }

    // MailList management
    void addMailToMailBox(const Mail& m) {this->mailbox.push_back(m);}
    void removeMailFromMailBox(const Mail& m){
        for (auto it = this->mailbox.begin(); it != this->mailbox.end(); ++it)
            if(it->getRecipientMailAddress() == m.getRecipientMailAddress() && it->getSubject() == m.getSubject())
                this->mailbox.erase(it);
    }
    void addMailToSentList(const SentMail& m) {this->sentMailList.push_back(m);}
    void addContact(const Account& acc) {this->contacts.insert(acc);}
    void showContacts() {
        for (auto it = this->contacts.begin(); it != this->contacts.end(); ++it)
            cout << it->getFirstName() << " " << it->getSurname() << " " << it->getMailAddress() << endl;
    }
    void sendScheduledMail(ScheduledMail* scheduled) {
        tm schTime = scheduled->getscheduledSendTime();
            time_t scheduledTime = mktime(&schTime);
            time_t currentTime = time(nullptr);
            if (scheduledTime > currentTime) {
                chrono::system_clock::time_point scheduledTimePoint(chrono::system_clock::from_time_t(scheduledTime));
                this_thread::sleep_until(scheduledTimePoint);
            }
            this->mailbox.push_back(*scheduled);

            SentMail *sent = dynamic_cast<SentMail*>(scheduled);
            if(sent) {
                SentMail sentMail = *sent;
                this->sentMailList.push_back(sentMail);
            } else cout << "Error: Nu s-a putut realiza";
        }

    void viewUnreadmails() {
        for(auto it = this->mailbox.begin(); it != this->mailbox.end(); ++it)
                cout << *it << endl;
    }
};

class Menu{
private:
    static Menu *obj;
    string date;

    Menu() {date = "dd/mm/yyyy";}

public:
    static Menu* getInstance()
    {
        if(!obj)
            obj = new Menu();
        return obj;
    }

    string getDate() {return this->date;}
    void setDate(string dateInput) {this->date = dateInput;}

    void showLoggedInMenu();

    void showMenu()
    {
        vector<Account> listAccounts;
        vector<AccountMailBox> listAccountMailBoxes;
        int k = 1;
        // int i = 0;
        while(k == 1)
        {
            cout << "\n1) Add mail account;";
            cout << "\n2) Log into mail account;";
            cout << "\n3) Exit program;";
            cout << "\nChoice: ";

            int choice;
            cin >> choice;
            switch(choice)
            {
                case 1:
                {
                    Account acc; AccountMailBox accmailbox;
                    cin >> acc;
                    listAccounts.push_back(acc);
                    accmailbox.setAccount(acc);
                    listAccountMailBoxes.push_back(accmailbox);
                    break;
                }
                case 2: {
                    string fixedMailAddress, fixedPassword;
                    bool logged_in = 0;
                    cout << "\nYour mail address: ";
                    cin >> fixedMailAddress;
                    cout << "\nPassword: ";
                    /// Urmatorul segment de cod e preluat (si adaptat) de pe: https://stackoverflow.com/questions/41652182/how-to-display-asterisk-for-input-password-in-c-using-clion
                    char pass_input[32];
                    char a;
                    for (int i = 0;;) {
                        a = getch();
                        if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9')) {
                            pass_input[i] = a;
                            ++i;
                            cout << "*";
                        }
                        if (a == '\b' && i >= 1) {
                            cout << "\b \b";
                            --i;
                        }
                        if (a == '\r') {
                            pass_input[i] = '\0';
                            break;
                        }
                    }
                    cout << endl;
                    fixedPassword = pass_input;
                    for (auto it = listAccounts.begin(); it != listAccounts.end(); ++it) {
                        if (it->getMailAddress() == fixedMailAddress && it->getPassword() == fixedPassword) {
                            cout << "\nWelcome, " << it->getFirstName() << ", what do you need from Dmail today?\n";
                            logged_in = 1;
                            int option;
                            for (auto it2 = listAccountMailBoxes.begin(); it2 != listAccountMailBoxes.end(); ++it2)
                                if (it2->getAccount().getMailAddress() == it->getMailAddress()) {
                                    do {
                                        showLoggedInMenu();
                                        cin >> option;
                                        switch (option) {
                                            case 1: {
                                                cout<< "\nWould you like to schedule your mail for a later time, or send it right now?";
                                                cout << "\nL: Later time;";
                                                cout << "\nR: Right now;";
                                                cout << "\nPlease select an option by writing its letter: ";
                                                char letter;
                                                cin >> letter;
                                                if (toupper(letter) == 'R') {
                                                    SentMail newMail;
                                                    string recipientMailAddress;
                                                    cout << "\nFrom: ";
                                                    cout << fixedMailAddress;
                                                    cin >> newMail;
                                                    newMail.setSenderMailAddress(fixedMailAddress);
                                                    recipientMailAddress = newMail.getRecipientMailAddress();
                                                    it2->addMailToMailBox(newMail);
                                                    it2->addMailToSentList(newMail);
                                                    for (auto it3 = listAccountMailBoxes.begin(); it3 != listAccountMailBoxes.end(); it3++)
                                                        //if (it3->getAccount().getMailAddress() == recipientMailAddress)
                                                    {
                                                        it3->addMailToMailBox(newMail);
                                                    }
                                                }
                                                if (toupper(letter) == 'L') {
                                                    SentMail* newScheduledMail = new SentMail();
                                                    string recipientMailAddress;
                                                    char decision;
                                                    cout << "\nFrom: ";
                                                    cout << fixedMailAddress;
                                                    cin >> *newScheduledMail;
                                                    newScheduledMail->setSenderMailAddress(fixedMailAddress);
                                                    recipientMailAddress = newScheduledMail->getRecipientMailAddress();
                                                    it2->sendScheduledMail(newScheduledMail);
                                                    cout << "\nThe mail was sent; would you like to undo this? (Y/N): ";
                                                    cin >> decision;
                                                    if (toupper(decision) == 'N'){
                                                        for (auto it3 = listAccountMailBoxes.begin(); it3 != listAccountMailBoxes.end(); ++it3)
                                                            if (it3->getAccount().getMailAddress() == recipientMailAddress)
                                                                it3->addMailToMailBox(*newScheduledMail);
                                                    }
                                                    if (toupper(decision) == 'Y'){
                                                        // UPCASTING
                                                        ScheduledMail* scheduledMail = new ScheduledMail(*newScheduledMail);
                                                        it2->removeMailFromMailBox(*newScheduledMail);
                                                        it2->addMailToMailBox(*scheduledMail);
                                                    }


                                                }
                                                break;
                                            }
                                            case 2: {
                                                it2->viewUnreadmails();
                                                break;
                                            }
                                            case 3:
                                                it2->getScheduledMailList();
                                                break;
                                            case 4:
                                                it2->showSentMailList();
                                                break;
                                            case 5: {
                                                Account acc;
                                                string accMail;
                                                bool ok = 0;
                                                cout << "Please write correctly the dmail of the desired contact: ";
                                                cin >> accMail;
                                                for (auto it5 = listAccounts.begin();
                                                     it5 != listAccounts.end(); ++it5) {
                                                    if (it5->getMailAddress() == accMail) {
                                                        ok = 1;
                                                        cout << "\nWould you like to add " << it5->getFirstName() << " "
                                                             << it5->getSurname() << " to your Contacts list?";
                                                        char YN;
                                                        cout << "\nY/N: ";
                                                        cin >> YN;
                                                        if (toupper(YN) == 'N')
                                                            break;
                                                        else {
                                                            acc = *it5;
                                                            it2->addContact(acc);
                                                            cout << endl << it5->getFirstName() << " has been added to your Contacts list.\n";
                                                            cout << "Your Contacts list currently is:\n";
                                                            it2->showContacts();
                                                        }
                                                    }
                                                }
                                                if (ok == 0)
                                                    cout << "\nWe couldn't find the dmail you wrote!";
                                                break;
                                            }
                                            case 6:
                                                option = 0;
                                                break;
                                            default:
                                                cout << "\nPlease write a valid option (a number from 1-6).";
                                                break;
                                        }
                                    }while (option != 0);
                        }
                        }
                    }
                    if (logged_in == 0)
                        cout << "Your credentials are incorrect/ do not exist in our database. Please try again!\n";
                    break;
                }
                case 3:
                    k = 0;
                    break;
            }
        }
    }
};

Menu* Menu::obj = 0;

void Menu::showLoggedInMenu() {
    cout << "\n1) Compose a new dmail;";
    cout << "\n2) View unread dmails;";
    cout << "\n3) View scheduled dmails;";
    cout << "\n4) View sent dmails (check read status);";
    cout << "\n5) Add new dmail contact;";
    cout << "\n6) Go back (Log into another mail account/Exit app);";
    cout << "\nOption: ";
}

int main()
{

    Menu* myMenu = myMenu->getInstance();
    myMenu->setDate("03/01/2023");
    myMenu->showMenu();
    return 0;
}
