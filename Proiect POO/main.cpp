/*Proiect Sistem de Autentificare*/
/* Pentru rulare pe alte calculatoare trebuie schimbate căile fișierelor
(în 5 locuri, liniile: 233, 329, 391, 420, 448); pare că funcționează
numai cu căi absolute */

#include <iostream>
#include <string>
#include <cstring>
#include <list>
#include <fstream>
#include <ctime>

const std::string car = "0123456789!@#$%^&*_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
int string_length = car.size() - 1;

std::string timp() {
    time_t now = time(0);
    std::string dt = ctime(&now);
    return dt;
}

class Lista;
class NewUser {
private:
    std::string userName;
    std::string passWord;
    std::string securityQuestions[3];
    std::string securityAnswers[3];
    NewUser *urm;
    bool verifyUserName(NewUser* head, std::string usN);
    void getNewPassword(std::string &p);
public:
    NewUser(std::string uName, std::string pWord, std::string sQ[3], std::string sA[3]);
    void setSecurityQnA(std::string secQ[], std::string secA[]);
    void createUserProfile(std::string &n);
    void citire(NewUser* &a, std::string &uN);
    virtual void afisare();
    bool verName(NewUser* head, std::string &usN);
    bool verPassword(NewUser* head, std::string &usN, std::string &usP);
    bool verSecAn(NewUser* head, std::string &usN, std::string secA[], int &i);
    void getSecQ(NewUser* head, std::string &usN, std::string secQ[], int &i);
    void rstPW(NewUser* head, std::string &usN, std::string &pW);
    friend class Lista;
};

NewUser::NewUser(std::string uName, std::string pWord, std::string sQ[3], std::string sA[3]) {
    int i;
    this->userName = uName;
    this->passWord = pWord;
    for (i = 0; i < 3; i++) {
        this->securityQuestions[i] = sQ[i];
        this->securityAnswers[i] = sA[i];
    }
    urm = NULL;
}

class Lista {
public:
    NewUser *head = NULL;
    void adaugare(NewUser* &a);
    void afisare_lista(NewUser* a);
    void citire_fisier(NewUser* &a);
    void scriere_fisier();
};

class RandomPassword {
private:
    std::string password;

    std::string genPassword(std::string &p);
public:
    RandomPassword(std::string pw);
    std::string getPassword(std::string &pw);
};

RandomPassword::RandomPassword(std::string pw) {
    this->password = pw;
}

void NewUser::setSecurityQnA(std::string secQ[], std::string secA[]) {
    int i;
    std::cout << std::endl;
    for (i = 0; i < 3; i++) {
        std::cout << "\nIntroduceti intrebarea de securitate " << i + 1 << ": ";
        getline(std::cin, secQ[i]);
        std::cout << "Introduceti raspunsul: ";
        getline(std::cin, secA[i]);
    }
}

void NewUser::createUserProfile(std::string &n) {
    std::cout << "\nNume de utilizator: ";
    if (n.empty())
        std::cin.ignore();
    getline(std::cin, n);
}

void NewUser::getNewPassword(std::string &p) {
    RandomPassword *rp = NULL;
    rp->getPassword(p);
    std::cout << "Parola generata pentru utilizatorul dat este: " << p;
}

bool NewUser::verifyUserName(NewUser* head, std::string usN) {
    if (head == NULL)
        return false;
    if (head->userName == usN)
        return true;
    return verifyUserName(head->urm, usN);
}

void NewUser::afisare() {
    int i;
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "Nume de utilizator: " << userName << std::endl;
    std::cout << "Parola: " << passWord << std::endl;
    for (i = 0; i < 3; i++) {
        std::cout << "Intrebarea de securitate " << i + 1 << ": " << securityQuestions[i] << std::endl;
        std::cout << "Raspuns: " << securityAnswers[i] << std::endl;
    }
}

void meniu(Lista &l, NewUser* &u);

std::string RandomPassword::getPassword(std::string &pw) {
    genPassword(pw);
    new RandomPassword(pw);
    return pw;
}

std::string RandomPassword::genPassword(std::string &p) {
    int i, n;
    srand(time(0));
    n = 12 + (rand() % 5);
    for(i = 0; i < n; i++) {
        p += car[rand() % string_length];
    }
    return p;
}

class ResetPassword {
private:
    std::string userName;
    std::string passWord;
    std::string savedSecurityQuestions[3];
    std::string savedSecurityAnswers[3];
    bool identityConfirmed;

    bool verifyUserName(NewUser* &a, std::string &usN);

    void getSavedSecurityQnA(NewUser* &a, std::string &usN, std::string ssQ[], int &i);

    bool getUserSecurityAnswers(NewUser* &a, std::string &usN, std::string ssA[], int &i);

    void getNewPassword(NewUser* &a, std::string &usN, std::string &pW);

    void updatePassword(Lista &l, NewUser* &a);
public:
    ResetPassword(std::string usN, std::string pW, std::string ssQ[3], std::string ssA[3], bool id);
    void upPw(Lista &l, NewUser* &a);
};

ResetPassword::ResetPassword(std::string usN, std::string pW, std::string ssQ[3], std::string ssA[3], bool id) {
    int i;
    this->userName = usN;
    this->passWord = pW;
    for (i = 0; i < 3; i++) {
        this->savedSecurityQuestions[i] = ssQ[i];
        this->savedSecurityAnswers[i] = ssA[i];
    }
    this->identityConfirmed = id;
}

bool ResetPassword::verifyUserName(NewUser* &a, std::string &usN) {
    a->verName(a, usN);
}

void NewUser::getSecQ(NewUser* head, std::string &usN, std::string secQ[], int &i) {
    if (head == NULL)
        return;
    if (head->userName == usN) {
        secQ[i] = head->securityQuestions[i];
        std::cout << secQ[i] << std::endl;
    }
    getSecQ(head->urm, usN, secQ, i);
}

void ResetPassword::getSavedSecurityQnA(NewUser* &a, std::string &usN, std::string ssQ[], int &i) {
    a->getSecQ(a, usN, ssQ, i);
}

bool NewUser::verSecAn(NewUser* head, std::string &usN, std::string secA[], int &i) {
    if (head == NULL)
        return false;
    if (head->userName == usN && head->securityAnswers[i] == *secA)
        return true;
    return verSecAn(head->urm, usN, secA, i);
}

bool ResetPassword::getUserSecurityAnswers(NewUser* &a, std::string &usN, std::string ssA[], int &i) {
    if (a->verSecAn(a, usN, ssA, i))
        return true;
    else
        return false;
}

void NewUser::rstPW(NewUser* head, std::string &usN, std::string &pW) {
    RandomPassword *rp = NULL;
    if (head == NULL)
        return;
    if (head->userName == usN) {
        head->passWord = "";
        head->passWord = rp->getPassword(head->passWord);
        pW = head->passWord;
        std::cout << "Parola dvs. actualizata este: " << pW << std::endl;
    }
    rstPW(head->urm, usN, pW);
}

void ResetPassword::getNewPassword(NewUser* &a, std::string &usN, std::string &pW) {
    a->rstPW(a, usN, pW);
}

void ResetPassword::updatePassword(Lista &l, NewUser* &a) {
    ResetPassword *r = NULL;
    std::string usN;
    std::string pW;
    std::string ssQ[3];
    std::string ssA[3];
    bool id;
    std::string dt;
    int i;
    std::ofstream log("/home/danielp/Programare-Orientată-pe-Obiecte/Proiect POO/evidenta.log", std::ofstream::app);
    std::cout << "\nNume utilizator: ";
    std::cin.ignore();
    getline(std::cin, usN);
    if (!verifyUserName(a, usN)) {
        std::cout << "Numele de utilizator " << usN << " nu apartine unui profil existent!" << std::endl;
        std::cout << "Introduceti altul!" << std::endl;
        meniu(l, a);
    }
    std::cout << "Raspundeti la intrebarile de securitate asociate profilului: " << std::endl;
    for (i = 0; i < 3; i++) {
        getSavedSecurityQnA(a, usN, ssQ, i);
        std::cout << "Raspuns: ";
        getline(std::cin, ssA[i]);
        if (!getUserSecurityAnswers(a, usN, &ssA[i], i)) {
            std::cout << "Raspunsul dat este gresit, incercati din nou!" << std::endl;
            meniu(l, a);
        }
    }
    getNewPassword(a, usN, pW);
    r = new ResetPassword(usN, pW, &ssQ[3], &ssA[3], id);
    l.scriere_fisier();
    if (log.is_open()) {
        dt = timp();
        log << dt << " " << "Parola utilizatorului " << usN << " a fost modificata si salvata cu succes!\n"
            << std::endl;
        log.close();
    }
}

void ResetPassword::upPw(Lista &l, NewUser* &a) {
    updatePassword(l, a);
}

void aplicatie(Lista &l, NewUser* &a, std::string &usN) {
    int opt;
    std::string dt;
    std::cout << "\nProfil: " << usN << std::endl;
    do {
        //system("CLS");
        std::cout << "\n1.Afisati timpul si data\n";
        std::cout << "0.Delogare\n";
        std::cout << "Introduceti optiunea dvs.: ";
        std::cin >> opt;
        switch (opt) {
            case 1:
                dt = timp();
                std::cout << std::endl << dt << std::endl;
                break;
            case 0: meniu(l, a);
                break;
            default: std::cout << "Optiune gresita!";
        }
    } while (opt != 0);
}

class Identity {
private:
    std::string userName;
    std::string userPassword;
    bool identityConfirmed;

    bool confirmIdentity(NewUser* &a, std::string &usN, std::string &usP, bool idC);
public:
    Identity(std::string uN, std::string uP, bool id);
    bool getConfirmedIdentity(Lista &l, NewUser* &a);
};

Identity::Identity(std::string uN, std::string uP, bool id) {
    this->userName = uN;
    this->userPassword = uP;
    this->identityConfirmed = id;
}

bool NewUser::verName(NewUser* head, std::string &usN) {
    if (verifyUserName(head, usN))
        return true;
    else
        return false;
}

bool NewUser::verPassword(NewUser* head, std::string &usN, std::string &usP) {
    if (head == NULL)
        return false;
    if (head->userName == usN && head->passWord == usP)
        return true;
    return verPassword(head->urm, usN, usP);
}

bool Identity::getConfirmedIdentity(Lista &l, NewUser* &a) {
    Identity *idt;
    std::string usN;
    std::string usP;
    bool idC;
    std::string dt;
    idC = true;
    std::ofstream log("/home/danielp/Programare-Orientată-pe-Obiecte/Proiect POO/evidenta.log", std::ofstream::app);
    std::cout << "\nNume de utilizator: ";
    std::cin.ignore();
    getline(std::cin, usN);
    if (!a->verName(a, usN)) {
        std::cout << "Utilizatorul " << usN << " nu exista!" << std::endl;
        meniu(l, a);
    }
    std::cout << "Parola: ";
    std::cin >> usP;
    if (!confirmIdentity(a, usN, usP, idC)) {
        std::cout << "\nParola este gresita!\nAutentificare esuata!" << std::endl;
        if (log.is_open()) {
            dt = timp();
            log << dt << " " << "Autentificare esuata pentru utilizatorul " << usN << "!\n" << std::endl;
            log.close();
        }
        meniu(l, a);
    } else {
        idt = new Identity(usN, usP, idC);
        aplicatie(l, a, usN);
    }
}

bool Identity::confirmIdentity(NewUser* &a, std::string &usN, std::string &usP, bool idC) {
    if (!a->verPassword(a, usN, usP)) {
        return !idC;
    } else
        return idC;
}

void Lista::adaugare(NewUser* &a) {
    NewUser *p;
    p = head;
    if (p) {
        if (a->userName > p->userName) {
            a->urm = head;
            head = a;
        }
        else {
            while (p->urm && (p->urm->userName > a->userName))
                p = p->urm;
            a->urm = p->urm;
            p->urm = a;
        }
    }
    else
        head = a;
}

void Lista::afisare_lista(NewUser* a) {
    if (!a)
        std::cout << "\nBaza de date nu exista!\n";
    else
        while (a) {
            a->afisare();
            a = a->urm;
        }
}

void Lista::scriere_fisier() {
    int i;
    std::ofstream f("/home/danielp/Programare-Orientată-pe-Obiecte/Proiect POO/BazaDate.txt");
    NewUser* a;
    a = head;
    if (!a)
        std::cout << "Baza de date este vida!";
    else {
        if (f.is_open()) {
            while (a) {
                f << a->userName << std::endl;
                f << a->passWord << std::endl;
                for (i = 0; i < 3; i++) {
                    f << a->securityQuestions[i] << std::endl;
                    f << a->securityAnswers[i] << std::endl;
                }
                f << std::endl;
                a = a->urm;
            }
            f.close();
        }
    }
}

void Lista::citire_fisier(NewUser* &a) {
    NewUser* b = NULL;
    std::string uN;
    std::string pW;
    std::string sQ[3];
    std::string sA[3];
    int i;
    std::ifstream f("/home/danielp/Programare-Orientată-pe-Obiecte/Proiect POO/BazaDate.txt");
    if (f) {
        while (!(f >> std::ws).eof()) {
            getline(f, uN);
            getline(f, pW);
            for (i = 0; i < 3; i++) {
                getline(f, sQ[i]);
                getline(f, sA[i]);
            }
            if (!a) {
                a = new NewUser(uN, pW, sQ, sA);
                adaugare(a);
            } else {
                b = new NewUser(uN, pW, sQ, sA);
                adaugare(b);
            }
        }
        f.close();
    }
}

void NewUser::citire(NewUser* &a, std::string &uN) {
    Lista l;
    std::string pW;
    std::string sQ[3];
    std::string sA[3];
    std::string dt;
    int i;
    std::ofstream log("/home/danielp/Programare-Orientată-pe-Obiecte/Proiect POO/evidenta.log", std::ofstream::app);
    l.citire_fisier(a);
    createUserProfile(uN);
    if (verifyUserName(a, uN)) {
        std::cout << "Numele de utilizator " << uN << " apartine unui profil existent!" << std::endl;
        std::cout << "Introduceti unul nou!" << std::endl;
        citire(a, uN);
    }
    else {
        pW = "";
        getNewPassword(pW);
        if (log.is_open()) {
            dt = timp();
            log << dt << " " << "S-a generat o parola pentru utilizatorul " << uN << "!\n" << std::endl;
            log.close();
        }
        a->setSecurityQnA(sQ, sA);
        a = new NewUser(uN, pW, sQ, sA);
        l.adaugare(a);
        l.scriere_fisier();
    }
}

std::istream& operator >> (std::istream &in, NewUser* &a) {
    std::string uN;
    a->citire(a, uN);
    return in;
}

void inregistrare(NewUser* &a) {
    std::cin >> a;
}

void meniu(Lista &l, NewUser* &u) {
    ResetPassword *r = NULL;
    int opt;
    Identity* id;
    do {
        //system("CLS");
        std::cout << "\n1.Creare profil nou de utilizator\n";
        std::cout << "2.Autentificare cu un cont existent\n";
        std::cout << "3.Resetare parola\n";
        std::cout << "4.Afisati baza de date\n";
        std::cout << "0.Iesire\n";
        std::cout << "Introduceti optiunea dvs.: ";
        std::cin >> opt;
        switch (opt) {
            case 1: inregistrare(u);
                break;
            case 2: id->getConfirmedIdentity(l, u);
                break;
            case 3: r->upPw(l, u);
                break;
            case 4: l.afisare_lista(u);
                break;
            case 0: exit(0);
            default: std::cout << "Optiune gresita!\n";
        }
    } while (opt != 0);
}

int main() {
    NewUser *u = NULL;
    Lista l;
    l.head = NULL;
    l.citire_fisier(u);
    meniu(l, u);
    return 0;
}
