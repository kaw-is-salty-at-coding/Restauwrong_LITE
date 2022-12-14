#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <utility>

using namespace std;
int slot = -1;
static void print_menu_line() {
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
            getline(user_file, total_member);
            {
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
                total >> total_count;
                add(id, std::move(user), std::move(password), std::move(type));
            }
        }
    }

    int login() {
        string username;
        string password;
        print_menu_line();
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
            print_menu_line();
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
        print_menu_line();
        cout << "Register" << endl;
        print_menu_line();
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
                print_menu_line();
                cout << "Your username have already use now!" << endl << "Please Enter again." << endl;
                print_menu_line();
            }
        } while (dupe);

        print_menu_line();
        cout << "Register Successfully" << endl;
        print_menu_line();
        string ID_check = to_string(total_count);
        int check = 6 - ID_check.length();
        for (int i = 0; i < check; i++) {
            ID += "0";
        }
        ID += to_string(total_count);
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
                fileout << temp->member_ID << "," << temp->member_Username << ","
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
        if (type == 0) {
            return temp->member_type;
        }else if (type == 1){
            return temp->member_Username;
        }
        return {};
    }
};

class nodeMenu {
public:
    nodeMenu(string m, int q, int p,string u,int s) {
        menu = std::move(m);
        quantity = q;
        price = p;
        username = u;
        status = s;
    }

    string menu;
    int quantity;
    int price;
    string username;
    int status;
    nodeMenu *link{};
};

class nodeOrder {
public:
    nodeMenu *head{};
    nodeMenu *tail{};
    int count = 0;
    int total = 0;
    nodeOrder *link{};

    explicit nodeOrder() {
    }

    void add(string m, int q, int p,string u,int s) {
        auto *temp = new nodeMenu(std::move(m), q, p,std::move(u),s);

        if (count == 0) {
            head = temp;
            tail = temp;
            count++;
        } else {
            tail->link = temp;
            tail = temp;
            count++;
        }
        total += temp->price * temp->quantity;
    }

    void showFull() const {
        nodeMenu *temp = head;
        print_menu_line();
        cout << "|Table:" << setw(2)  << setw(40) << right << "|" << endl
             << "|" << setw(4) << "No.|" << setw(26) << left << "Menu" << "|" << setw(5) << "Qty" << "|" << setw(5)
             << "Price" << "|" << "Net |" << endl;
        print_menu_line();
        for (int i = 0; i < count; i++) {
            cout << "|" << setw(4) << i + 1 << setw(26) << temp->menu << "|" << setw(5) << temp->quantity << "|"
                 << setw(5)
                 << temp->price
                 << "|" << setw(4) << temp->quantity * temp->price << "|" << endl;
            temp = temp->link;
        }
        print_menu_line();
        cout << "Total:" << right << total << endl;
        print_menu_line();
    }

    void showMini() const {
        nodeMenu *temp = head;
        print_menu_line();
        cout << "|Table:" << setw(2)  << setw(29) << right << "|" << endl
             << "|" << setw(4) << "No.|" << setw(26) << left << "Menu" << "|" << setw(5) << "Qty" << "|"
             << endl;
        print_menu_line();
        for (int i = 0; i < count; i++) {
            cout << "|" << setw(4) << i + 1 << setw(26) << temp->menu << "|" << setw(5) << temp->quantity << "|"
                 << endl;
            temp = temp->link;
        }
        print_menu_line();
    }
    void view_my_order(string user) {
        print_menu_line();
        cout << setw(41) << left << "|" << "My order" << setw(41) << right << "|" << endl;
        print_menu_line();
        cout << left << setw(6) << "|"  << left << setw(48) << "Food Name" << left
             << setw(10) << "Quantity" << setw(10)
             << "" << left << setw(10) << "Price" << right << setw(6) << "|" << endl;
        print_menu_line();
        nodeMenu *temp = head;
        total = 0;
        for(int i = 0 ; i< count ; i++){
            if((temp->username == user ||user =="-" )  && temp->status == 1 ){
                total += temp->price * temp->quantity;
                cout << left << setw(6) << "|"  << left << setw(48) << temp->menu << left
                     << setw(10) << temp->quantity << setw(10)
                     << "" << left << setw(10) << temp->price << right << setw(6) << "|" << endl;
            }

            temp = temp->link;
        }
        print_menu_line();
        cout <<"|" <<  setw(70) << right << "Total: " <<setw(5)<< total << right<<setw(14) <<right<< "|" << endl;
    }
    void read_file_order(){
        string username, menuname, menupricestr,menuquantitystr,menustatusstr;
        int menuprice,menuquantity,menustatus;
        ifstream file;
        string line;
        string d = ",";
        file.open("..//order.txt",ios::in);
        if(file.fail()){
            cout << " can't open file order.txt" << endl;
        }else {
            while( getline( file, line) ){
                username = line.substr(0, line.find(d));
                line.erase(0, line.find(d) + d.length());
                menuname = line.substr(0, line.find(d));
                line.erase(0, line.find(d) + d.length());
                menupricestr = line.substr(0, line.find(d));
                line.erase(0, line.find(d) + d.length());
                menuquantitystr = line.substr(0, line.find(d));
                line.erase(0, line.find(d) + d.length());
                menustatusstr = line.substr(0, line.find(d));
                                  line.erase(0, line.find(d) + d.length());
                menustatus = stoi(menustatusstr);
                menuprice = stoi(menupricestr);
                menuquantity = stoi(menuquantitystr);
                add(menuname,menuquantity,menuprice,username,menustatus);
            }
            file.close();
        }
    }

    void write_file_order(){
        fstream fileout;
        fileout.open("..//order.txt", ios::out);
        if (fileout.fail()) {
            cout << "cant open file" << endl;
        } else {
            nodeMenu *temp = head;
            while( temp != NULL )
            {
                fileout << temp->username << "," << temp->menu << "," <<temp->price << "," << temp->quantity << "," << temp->status << endl;
                temp = temp->link;
            }
            fileout.close();
        }
    }

    void resetnode(){
        count = 0;
        read_file_order();
    }

    void cancelorder( string username,int num ){
        nodeMenu * main = head;
        nodeMenu * temp = main;
        int c[100];
        int i = 0,k = 0;
            while (temp != NULL) {
                if ((temp->username ==username || username == "-")&& temp->status == 1) {
                    c[k] = i;
                    k++;
                }
                i++;
                temp = temp->link;
            }
        for( i = 0; i <= c[num-1]; i++ )
        {
            if( i == c[num-1] ){
                main->status = 0;
            }
            main = main->link;
        }
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

    void view_menu() {
        print_menu_line();
        cout << setw(43) << left << "|" << "View Menu" << setw(38) << right << "|" << endl;
        print_menu_line();
        auto *temp = new node_read_menu();
        temp = head_menu;
        cout << left << setw(6) << "|" << left << setw(10) << "No." << left << setw(35) << "Food Name" << left
             << setw(10) << "Normal Price" << setw(10)
             << "" << left << setw(10) << "Extra Price" << right << setw(6) << "|" << endl;
        print_menu_line();
        string normalprice, extraprice;
        while (temp != NULL) {
            normalprice = to_string(temp->menu_price);
            extraprice = to_string(temp->menu_price + temp->menu_price_extra);
            cout << left << setw(6) << "|" << left << setw(10) << "[" + temp->menu_ID + "]" << left << setw(30)
                 << temp->menu_name << setw(5) << "|" << left << setw(10) << "     " + normalprice << setw(7)
                 << "" << setw(3) << "|" << left << setw(10) << "      " + extraprice << right << setw(9) << "|"
                 << endl;
            temp = temp->link;
        }
        print_menu_line();
    }

    string get_menu_name( int no ){
        auto * temp = new node_read_menu();
        temp = head_menu;
        int menuID; bool check = false;
        string result;
        while( temp != NULL )
        {
            menuID = stoi(temp->menu_ID);
            if( menuID == no ){
                check = true;
                result = temp->menu_name;
                break;
            }
            temp = temp->link;
        }
        if( !check ){
            return "Not Found!!";
        }else {
            return result;
        }
    }

    int get_menu_price(int no ){
        auto * temp = new node_read_menu();
        temp = head_menu;
        int menuID; bool check = false;
        int result;
        while( temp != NULL )
        {
            menuID = stoi(temp->menu_ID);
            if( menuID == no ){
                check = true;
                result = temp->menu_price;
                break;
            }
            temp = temp->link;
        }
        if( !check ){
            return 0;
        }else {
            return result;
        }
    }
    int get_menu_price_extra(int no ){
        auto * temp = new node_read_menu();
        temp = head_menu;
        int menuID; bool check = false;
        int result;
        while( temp != NULL )
        {
            menuID = stoi(temp->menu_ID);
            if( menuID == no ){
                check = true;
                result = temp->menu_price + temp->menu_price_extra;
                break;
            }
            temp = temp->link;
        }
        if( !check ){
            return 0;
        }else {
            return result;
        }
    }

    int getLastestMenuID(){
        auto * temp = new node_read_menu();
        temp = head_menu; string id; int result;
        for( int i = 1; i <= count_menu; i++ ){
            id = temp->menu_ID;
            temp = temp->link;
        }
        result = stoi(id);
        return result;
    }

};
nodeOrder ordermode;
menu_list menu;
member_list memberList{};
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
    int no,quan,normal_or_extra;
    string wait;
    int main_menu_choice;
    string menuname,menuIDstr;
    int   menuprice,menuID;
    menu.read_file_menu_txt();
    Main_Menu:
    if (role == "customer") {
        print_menu_line();
        cout << setw(40) << left << "|" << "MAIN MENU" << setw(41) << right << "|" << endl;
        print_menu_line();
        cout << setw(35) << left << "|" << setw(40) << left << "[1] Order" << setw(15) << right << "|" << endl;
        cout << setw(35) << left << "|" << setw(40) << left << "[2] Cancel order" << setw(15) << right << "|" << endl;
        cout << setw(35) << left << "|" << setw(40) << left << "[3] View menu" << setw(15) << right
             << "|" << endl;
        cout << setw(35) << left << "|" << setw(40) << left << "[4] View my order" << setw(15) << right
             << "|" << endl;
        cout << setw(35) << left << "|" << setw(40) << left << "[5] Logout" << setw(15) << right << "|" << endl;
        print_menu_line();
        cout << "Select option[1-5] :";
        cin >> main_menu_choice;
        if (main_menu_choice == 1) {
            menu.view_menu();
            cout << "Order[0 to cancel] : ";
            cin >> no;
            cout << "Normal Or Extra ( 1 for normal & 2 for extra ) : ";
            cin  >> normal_or_extra;
            cout << "How many?[0 to cancel] : ";
            cin >> quan;
            cout << endl;
            if (no != 0 && quan != 0 && normal_or_extra == 1 ) {
                ordermode.add(menu.get_menu_name(no), quan,menu.get_menu_price(no),memberList.get(slot,1),1);
                ordermode.write_file_order();
            }else if ( no != 0 && quan != 0 && normal_or_extra == 2 ){
                ordermode.add(menu.get_menu_name(no), quan,menu.get_menu_price(no) + 10,memberList.get(slot,1),1);
                ordermode.write_file_order();
            }
        } else if (main_menu_choice == 2) {
                ordermode.view_my_order(memberList.get(slot,1));
                cout << " Enter Number To Cancel :";
                int num;
                cin  >> num;
                ordermode.cancelorder(memberList.get(slot,1),num);
                ordermode.write_file_order();
                ordermode.resetnode();
                cout << " Back : ";
                cin  >> wait;
                goto Main_Menu;
        } else if (main_menu_choice == 3) {
            menu.view_menu();
            cout << " Back : ";
            cin  >> wait;
            goto Main_Menu;
        } else if (main_menu_choice == 4) {
           ordermode.view_my_order(memberList.get(slot,1));
           print_menu_line();
            cout << " Back : ";
            cin  >> wait;
            goto Main_Menu;
        }
        if (main_menu_choice != 5) {
            goto Main_Menu;
        }
    }
    else if (role == "staff") {
        print_menu_line();
        cout << setw(40) << left << "|" << "MAIN MENU" << setw(41) << right << "|" << endl;
        print_menu_line();
        cout << setw(35) << left << "|" << setw(40) << left << "[1] Manage Order" << setw(15) << right << "|" << endl;
        cout << setw(35) << left << "|" << setw(40) << left << "[2] View menu" << setw(15) << right << "|" << endl;
        cout << setw(35) << left << "|" << setw(40) << left << "[3] Add Menu" << setw(15) << right << "|" << endl;
        cout << setw(35) << left << "|" << setw(40) << left << "[4] Logout" << setw(15) << right << "|" << endl;
        print_menu_line();
        cout << "Select option[1-4] :";
        cin >> main_menu_choice;
        if (main_menu_choice == 1) {
            ordermode.view_my_order("-");
            cout << " Enter Number To Cancel :";
            int num;
            cin  >> num;
            ordermode.cancelorder("-",num);
            ordermode.write_file_order();
            ordermode.resetnode();
            cout << " Back : ";
            cin  >> wait;
            goto Main_Menu;
        } else if (main_menu_choice == 2) {
        menu.view_menu();
            cout << " Back : ";
            cin  >> wait;
            goto Main_Menu;
        } else if( main_menu_choice == 3 ){
            print_menu_line();
            cout << setw(35) << left << "|" << setw(40) << left << "Add Menu" << setw(15) << right << "|" << endl;
            print_menu_line();
            cout << "|   " << " Enter Menu Name : ";
            getline(cin >> ws,menuname);
            cout << "|   " << "Enter Price : ";
            cin  >> menuprice;
            menuID = menu.getLastestMenuID();
            menuID++;
            menuIDstr = to_string(menuID);
            menu.add_menu(menuIDstr,menuname,menuprice,10);
            print_menu_line();
            cout << " Back : ";
            cin  >> wait;
            goto Main_Menu;
        }
        if (main_menu_choice != 4) {

            goto Main_Menu;
        }
    }
}

int main() {
    memberList.readUserFile();
    ordermode.read_file_order();
    Home:
    int home = home_menu();

    if (home == 1) {
        slot = memberList.login();
        if (slot != -1) {
            main_menu(memberList.get(slot, 0));
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