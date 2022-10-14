#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <utility>

using namespace std;

static void printline() {
    for (int i = 0; i < 90; i++) {
        cout << "=";
    }
    cout << endl;
}


class member {
public:
    string member_ID;
    string member_Username;
    string member_password;
    string member_type;
    member *link{};

    member(string ID, string username, string password, string type) {
        member_ID = std::move(ID);
        member_Username = std::move(username);
        member_password = std::move(password);
        member_type = std::move(type);
    }
};

class member_list {
public:
    member *head_of_member_list;
    member *tail_of_member_list;
    int member_count;
    int total_count;
    void add(const string &ID, string user, string pass, string type) {
        member *temp;
        temp = new member(ID, std::move(user), std::move(pass), std::move(type));
        if (member_count == 0) {
            head_of_member_list = temp;
            tail_of_member_list = temp;
        } else {
            tail_of_member_list->link = temp;
            tail_of_member_list = temp;
        }
        member_count++;
    }



    void readUserFile() {
        string total_member;
        ifstream user_file;
        string line;
        string user;
        string password;
        string id;
        string type;
        user_file.open("..\\user.txt", ios::in);
        if (user_file.fail()) {
            cout << "cant open file" << endl;
        } else {
            getline(user_file, total_member);{
            }
            while (getline(user_file, line)) {
                id = line.substr(0, line.find(','));
                line.erase(0, line.find(',') + 1);
                user = line.substr(0, line.find(','));
                line.erase(0, line.find(',') + 1);
                password = line.substr(0, line.find(','));
                line.erase(0, line.find(',') + 1);
                type = line.substr(0, line.find(' '));
                stringstream total(total_member);
                total>>total_count;
                add(id, std::move(user), std::move(password), std::move(type));
            }
        }
    }

    int login()  {
        string username;
        string password;
        printline();
        cout << setw(43) << left << "|" << "LOGIN" << setw(42) << right << "|" << endl;
        cout << "Username:";
        cin >> username;
        cout << "Password:";
        cin >> password;
        member *temp = head_of_member_list;
        bool found = false;
        for (int i = 0; i < member_count; i++) {
            if (username == temp->member_Username) {
                if (password == temp->member_password) {
                    found = true;
                    return i;
                }
            } else temp = temp->link;
        }
        if (!found) {
            printline();
            cout << "Not found!" << endl;
            return -1;
        }
        return 0;
    }

    void registerUser() {
        string ID;
        string user;
        string pass;
        string type;
        int dupe = false;
        printline();
        cout << "Register" << endl;
        printline();
        do {
            cout << "Enter Username:";
            cin >> user;
            cout << "Enter Password:";
            cin >> pass;
            dupe = false;
            member *temp = head_of_member_list;
            for (int i = 0; i < member_count; i++) {
                if (user == temp->member_Username) {
                    dupe = true;
                }
                temp = temp->link;
            }
            if (dupe) {
                printline();
                cout << "Your username have already use now!" << endl << "Please Enter again." << endl;
                printline();
            }
        } while (dupe);

        printline();
        cout << "Register Successfully" << endl;
        printline();
        string ID_check = to_string(total_count);
        int check = 6-ID_check.length();
        for(int i = 0 ; i<check ; i++){
            ID+="0";
        }
        ID+= to_string(total_count);
        add(ID, user, pass, "customer");
        total_count++;
        writefile();
    }

    void writefile() const {
        ofstream fileout;
        fileout.open("..\\user.txt", ios::out);
        if (fileout.fail()) {
            cout << "cant open file" << endl;
        } else {
            member *temp = head_of_member_list;
            fileout << total_count << endl;
            for (int j = 0; j < member_count; j++) {
                fileout << temp->member_ID<<"," << temp->member_Username << ","
                << temp->member_password << "," << temp->member_type << " " << endl;
                temp = temp->link;
            }
        }

    }

    string get(int index, int type) const {
        member *temp = nullptr;
        for (int i = 0; i <= index; i++) {
            if (i == 0) {
                temp = head_of_member_list;
            } else {
                temp = temp->link;
            }

        }
        if(type == 0){
            return temp->member_type;
        }
        return {};
    }
};

class order {
public:
    string order_name;
    string order_ID;
    float order_price;

    order(string name, string ID, float price) {
        order_name = std::move(name);
        order_ID = std::move(ID);
        order_price = price;
    }
};


class menu_list {
private:
    class node_read_menu {
    public:
        node_read_menu *link;
        string menu_name;
        string menu_ID;
        int menu_price;
        int menu_price_extra;

        node_read_menu() {
            link = nullptr;
            menu_name = "";
            menu_ID = "";
            menu_price = 0;
            menu_price_extra = 0;
        }


    };

public:
    int count_menu;
    node_read_menu *head_menu;
    node_read_menu *tail_menu;

    menu_list() {
        count_menu = 0;
        head_menu = nullptr;
        tail_menu = nullptr;
    }

    // function add data to node
    void add_read_menu_txt(string name, string menu_ID, int menu_price, int menu_price_extra) {
        auto *temp = new node_read_menu();

        temp->menu_name = name;
        temp->menu_ID = menu_ID;
        temp->menu_price = menu_price;
        temp->menu_price_extra = menu_price_extra;
        if (count_menu == 0) {
            temp->link = NULL;
            head_menu = temp;
            tail_menu = temp;
        } else {
            temp->link = NULL;
            tail_menu->link = temp;
            tail_menu = tail_menu->link;
        }

        count_menu++;
    }

    //function read data from menu.txt and then use function add_read to put data in node
    void read_file_menu_txt() {
        string read_name, read_ID;
        int read_price, read_price_extra;
        string read_pricestr, read_priceextra_str;
        string line;
        ifstream file;
        string d = ",";
        file.open("..//menu.txt", ios::in);
        if (file.fail()) {
            cout << "Can't open menu file" << endl;
        } else {
            while (getline(file, line)) {

                read_ID = line.substr(0, line.find(d));
                line.erase(0, line.find(d) + d.length());
                read_name = line.substr(0, line.find(d));
                line.erase(0, line.find(d) + d.length());
                read_pricestr = line.substr(0, line.find(d));
                line.erase(0, line.find(d) + d.length());
                read_priceextra_str = line.substr(0, line.find(d));
                line.erase(0, line.find(d) + d.length());

                read_price = stoi(read_pricestr);
                read_price_extra = stoi(read_priceextra_str);
                add_read_menu_txt(read_name, read_ID, read_price, read_price_extra);
            }
            file.close();
        }
    }

    //function updatefile menu.txt
    void update_file_menu() {
        fstream data;
        data.open("..//menu.txt", ios::out);
        if (data.is_open()) {
            auto *temp = new node_read_menu();
            temp = head_menu;
            while (temp != NULL) {
                data << temp->menu_ID << "," << temp->menu_name << "," << temp->menu_price << ","
                     << temp->menu_price_extra << endl;
                temp = temp->link;
            }
            data.close();
        } else {
            cout << "Sorry, Can't update file, Please check file again! " << endl;
        }
    }

    // function add menu and then updatefile menu.txt automatics
    void add_menu(string menu_id, string menu_name, int menu_pirce, int menu_price_extra) {
        add_read_menu_txt(menu_name, menu_id, menu_pirce, menu_price_extra);
        update_file_menu();
    }

    // function delete menu ( delete node ) and then update file menu.txt automatics
    void delete_menu(string menu_ID) {
        auto *main = new node_read_menu();
        main = head_menu;
        auto *temp = main;
        node_read_menu *prev = nullptr;
        if (temp != nullptr && temp->menu_ID == menu_ID) {
            main = temp->link;
            delete temp;
            return;
        } else {
            while (temp != nullptr && temp->menu_ID != menu_ID) {
                prev = temp;
                temp = temp->link;
            }
            if (temp == nullptr)
                return;

            prev->link = temp->link;
            delete temp;
        }
        update_file_menu();
    }

    // test print data
    void test_print() {
        auto *temp = new node_read_menu();
        temp = head_menu;
        while (temp != NULL) {
            cout << temp->menu_ID << " " << temp->menu_name << " " << temp->menu_price << endl;
            temp = temp->link;
        }
    }

};

static void print_menu_line() {
    for (int i = 1; i <= 90; i++) {
        cout << "=";
    }
    cout << endl;
}

int home_menu() {
    int home_menu_choice;
    print_menu_line();
    cout << setw(43) << left << "|" << "HOME" << setw(43) << right << "|" << endl;
    print_menu_line();
    cout << setw(40) << left << "|" << setw(20) << left << "[1] Login" << setw(30) << right << "|" << endl;
    cout << setw(40) << left << "|" << setw(20) << left << "[2] Register" << setw(30) << right << "|" << endl;
    cout << setw(40) << left << "|" << setw(20) << left << "[3] Exit" << setw(30) << right << "|" << endl;
    print_menu_line();
    cout << "Select option[1-3] :";
    cin >> home_menu_choice;
    return home_menu_choice;
};

void main_menu(const string &role) {
    int main_menu_choice;
    Main_Menu:
    if (role == "customer") {
        print_menu_line();
        cout << setw(40) << left << "|" << "MAIN MENU" << setw(41) << right << "|" << endl;
        print_menu_line();
        cout << setw(35) << left << "|" << setw(40) << left << "[1] Order" << setw(15) << right << "|" << endl;
        cout << setw(35) << left << "|" << setw(40) << left << "[2] Cancel order" << setw(15) << right << "|" << endl;
        cout << setw(35) << left << "|" << setw(40) << left << "[3] View menu" << setw(15) << right
             << "|" << endl;
        cout << setw(35) << left << "|" << setw(40) << left << "[4] Logout" << setw(15) << right << "|" << endl;
        print_menu_line();
        cout << "Select option[1-4] :";
        cin >> main_menu_choice;
        if (main_menu_choice == 1) {

        }else if(main_menu_choice == 2){

        }else if (main_menu_choice == 3){
            
        }
        if (main_menu_choice != 4){
            goto Main_Menu;
        }
    }
    else if (role == "staff") {
        print_menu_line();
        cout << setw(40) << left << "|" << "MAIN MENU" << setw(41) << right << "|" << endl;
        print_menu_line();
        cout << setw(35) << left << "|" << setw(40) << left << "[1] Manage Order" << setw(15) << right << "|" << endl;
        cout << setw(35) << left << "|" << setw(40) << left << "[2] View menu" << setw(15) << right << "|" << endl;
        cout << setw(35) << left << "|" << setw(40) << left << "[3] Logout" << setw(15) << right << "|" << endl;
        print_menu_line();
        cout << "Select option[1-4] :";
        cin >> main_menu_choice;
        if (main_menu_choice == 1) {

        }else if(main_menu_choice == 2){

        }
        if (main_menu_choice != 3){
            goto Main_Menu;
        }
    }
}

int main() {
    member_list memberList{};
    memberList.readUserFile();
    menu_list menu;
    menu.read_file_menu_txt();

    Home:
    int home = home_menu();
    int slot  = -1;
    if (home == 1) {
        slot = memberList.login();
        if(slot!= -1){
            main_menu(memberList.get(slot,0));
        }
    } else if (home == 2) {
        memberList.registerUser();
        goto Home;
    }
    if (home != 3) {
        goto Home;
    }
    return 0;
}