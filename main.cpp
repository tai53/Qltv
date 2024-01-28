#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

void createUser(const string &userId, const string &username, const string &password, const string &role) {
    // Create a text file
    ofstream usersFile;
    usersFile.open("users.txt", ios::app);

    // Write to the file
    usersFile << "User ID: " << userId << endl;
    usersFile << "Username: " << username << endl;
    usersFile << "Password: " << password << endl;
    usersFile << "Role: " << role << endl;
    usersFile << "----------" << endl;

    // Close the file
    usersFile.close();
}

string removeUser(const string &userId) {
    // Create a text string, which is used to output the text file
    string myText;

    // Read from the text file
    ifstream usersFile("users.txt");
    if (!usersFile.is_open()) {
        return "Input file failed to open\n";
    }

    // now open temp output file
    ofstream tempFile("temp.txt");

    string user_id = "User ID: " + userId;
    int index = 0;
    bool flag = false;

    while (getline(usersFile, myText)) {
        if (index > 4) {
            flag = false;
        }
        if (myText != user_id && !flag) {
            tempFile << myText << endl;
        } else if (myText == user_id || flag) {
            index += 1;
            flag = true;
            continue;
        }
    }

    // Close the file
    tempFile.close();
    usersFile.close();

    // delete the original file
    remove("users.txt");
    // rename old to new
    rename("temp.txt", "users.txt");

    if (index == 0)
        return "User ID not exist!";
    return "User ID is deleted!";
}

string login(const string &username, const string &password) {
    // Create a text string, which is used to output the text file
    string myText;

    // Read from the text file
    ifstream usersFile("users.txt");
    if (!usersFile.is_open()) {
        return "Input file failed to open\n";
    }

    bool isUsername = false, isPassword = false;
    string role;
    string user_name = "Username: " + username;
    string pwd = "Password: " + password;

    while (getline(usersFile, myText)) {
        if (myText == user_name) {
            isUsername = true;
            continue;
        }
        if (myText == pwd && isUsername) {
            isPassword = true;
            continue;
        }
        if (isPassword) {
            role = myText;
            break;
        }
    }
    // Close the file
    usersFile.close();

    return role;
}

class Book {
public:
    string isbn;
    string title;
    string subject;
    string author;
    string publisher;
    string date;
    int pages{};
    int copies{};
};

class User {
public:
    int userid{};
    string username;
    string password;
    int userLevel{};

    virtual ~User() = default;
};

class NormalUser : public User {
public:
    static void manageLibrary(vector<Book> &library);

    static void displayLibrary(const vector<Book> &library);

    static void searchBook(const vector<Book> &library);

private:
    static void displayBook(const Book &book);
};

class AdminUser : public User {
public:
    static void manageLibrary(vector<Book> &library);

    static void searchBook(const vector<Book> &library);

private:
    static void displayLibrary(const vector<Book> &library);

    static void displayBook(const Book &book);

    static void addBook(vector<Book> &library);

    static void deleteBook(vector<Book> &library);

    static void editBook(vector<Book> &library);
};

void addBookFile(vector<Book>& library) {
    ofstream outfile;
    outfile.open("Sach.txt");
    for (const Book& book : library) {
        outfile << book.isbn << "|" << book.title << "|" << book.subject << "|" << book.author << "|"
                << book.publisher << "|" << book.date << "|" << book.pages << "|" << book.copies << endl;
    }
    outfile.close();
}

void readBookFile(vector<Book>& library) {
    library.clear();
    ifstream infile;
    infile.open("Sach.txt");
    while (infile >> ws) {
        Book book;
        getline(infile, book.isbn, '|');
        if (book.isbn.empty()) {
            break;
        }
        getline(infile, book.title, '|');
        getline(infile, book.subject, '|');
        getline(infile, book.author, '|');
        getline(infile, book.publisher, '|');
        getline(infile, book.date, '|');
        infile >> book.pages;
        infile.ignore();
        infile >> book.copies;
        infile.ignore();
        library.push_back(book);
    }
}

void homeMenu() {
    vector<Book> library;
    int userChoice;
    do {
        cout << "\n===== Chuong trinh quan ly thu vien =====\n";
        cout << "1. Dang nhap\n";
        cout << "0. Thoat chuong trinh\n";
        cout << "==========================================\n";
        cout << "Nhap lua chon: ";
        cin >> userChoice;
        switch (userChoice) {
            case 1: {
                string username, password;
                cout << "Nhap ten dang nhap: ";
                cin >> username;
                cout << "Nhap mat khau: ";
                cin >> password;
                string role = login(username, password);
                if (role == "Role: user") {
                    NormalUser::manageLibrary(library);
                } else if (role == "Role: admin") {
                    AdminUser::manageLibrary(library);
                } else {
                    cout << "Dang nhap k hop le";
                }
            }
                break;
            case 0:
                cout << "Thoat chuong trinh.\n";
                exit(0);
            default:
                cout << "Lựa chọn không hợp lệ. Vui lòng chọn lại.\n";
                break;
        }
    } while (userChoice != 0);
}

void NormalUser::manageLibrary(vector<Book> &library) {
    int choice;
    do {
        cout << "\n===== Quan ly thu vien =====\n";
        cout << "1. Hien thi sach\n";
        cout << "2. Tim kiem sach\n";
        cout << "0. Dang xuat\n";
        cout << "===============================\n";
        cout << "Nhap lua chon: ";
        cin >> choice;
        switch (choice) {
            case 1:
                displayLibrary(library);
                break;
            case 2:
                searchBook(library);
                break;
            case 0:
                homeMenu();
                break;
            default:
                cout << "Lua chon khong hop le. Vui long nhap lai.\n";
                break;
        }
    } while (choice != 0);
}

void NormalUser::displayLibrary(const vector<Book> &library) {
    cout << "\n===== Danh sach sach trong thu vien =====\n";
    for (const Book &book: library) {
        displayBook(book);
    }
}

void NormalUser::searchBook(const vector<Book> &library) {
    string keyword;
    cout << "Nhap tu khoa tim kiem: ";
    cin.ignore();
    getline(cin, keyword);
    cout << "\n===== Ket qua tim kiem =====\n";
    for (const Book &book: library) {
        if (book.title.find(keyword) != string::npos ||
            book.subject.find(keyword) != string::npos ||
            book.author.find(keyword) != string::npos ||
            book.publisher.find(keyword) != string::npos ||
            book.date.find(keyword) != string::npos) {
            displayBook(book);
        }
    }
}

void NormalUser::displayBook(const Book &book) {
    cout << "Ma so: " << book.isbn << endl;
    cout << "Tieu de: " << book.title << endl;
    cout << "Chu de: " << book.subject << endl;
    cout << "Tac gia: " << book.author << endl;
    cout << "Nha xuat ban: " << book.publisher << endl;
    cout << "Ngay xuat ban: " << book.date << endl;
    cout << "So trang: " << book.pages << endl;
    cout << "So ban copy trong thu vien: " << book.copies << endl;
    cout << "------------------------------" << endl;
}

void AdminUser::manageLibrary(vector<Book> &library) {
    int choice;
    do {
        cout << "\n===== Quan ly thu vien =====\n";
        cout << "1. Hien thi sach\n";
        cout << "2. Them sach moi\n";
        cout << "3. Xoa sach\n";
        cout << "4. Sua sach\n";
        cout << "5. Tim kiem sach\n";
        cout << "0. Dang xuat\n";
        cout << "===============================\n";
        cout << "Nhap lua chon: ";
        cin >> choice;
        switch (choice) {
            case 1:
                displayLibrary(library);
                break;
            case 2:
                addBook(library);
                break;
            case 3:
                deleteBook(library);
                break;
            case 4:
                editBook(library);
                break;
            case 5:
                searchBook(library);
                break;
            case 0:
                homeMenu();
                break;
            default:
                cout << "Lua chon khong hop le. Vui long nhap lai.\n";
                break;
        }
    } while (choice != 0);
}

void AdminUser::searchBook(const vector<Book> &library) {
    string keyword;
    cout << "Nhập từ khóa tìm kiếm: ";
    cin.ignore(); // Đọc kí tự '\n' từ lựa chọn trước đó
    getline(cin, keyword);

    cout << "\n===== Kết quả tìm kiếm =====\n";
    for (const Book &book: library) {
        // Tìm kiếm từ khóa trong tên sách, chủ đề, tác giả, nhà xuất bản và năm xuất bản
        if (book.title.find(keyword) != std::string::npos ||
            book.subject.find(keyword) != std::string::npos ||
            book.author.find(keyword) != std::string::npos ||
            book.publisher.find(keyword) != std::string::npos ||
            book.date.find(keyword) != std::string::npos) {
            displayBook(book);
        }
    }
}

//Hàm hiển thị thông tin sách
void AdminUser::displayBook(const Book &book) {
    cout << "Ma so: " << book.isbn << endl;
    cout << "Tieu de: " << book.title << endl;
    cout << "Chu de: " << book.subject << endl;
    cout << "Tac gia: " << book.author << endl;
    cout << "Nha xuat ban: " << book.publisher << endl;
    cout << "Ngay xuat ban: " << book.date << endl;
    cout << "So trang: " << book.pages << endl;
    cout << "So ban copy trong thu vien: " << book.copies << endl;
    cout << "------------------------------" << endl;
}

//Hàm thêm sách
void AdminUser::addBook(vector<Book> &library) {
    Book newBook;
    //Xóa bộ đệm đầu vào
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Nhap thong tin sach moi:\n";
    cout << "Ma sach: ";
    getline(cin, newBook.isbn);
    cout << "Ten sach: ";
    getline(cin, newBook.title);
    cout << "Chu de: ";
    getline(cin, newBook.subject);
    cout << "Tac gia: ";
    getline(cin, newBook.author);
    cout << "Nha xuat ban: ";
    getline(cin, newBook.publisher);
    cout << "Ngay xuat ban: ";
    getline(cin, newBook.date);
    cout << "So trang: ";
    cin >> newBook.pages;
    //Kiểm tra đầu vào có hợp lê không
    while (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Khong hop le. Vui long nhap. So trang: ";
        cin >> newBook.pages;
    }
    cout << "So ban copy trong thu vien: ";
    cin >> newBook.copies;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Khong hop le. Vui long nhap lai. So ban copy trong thu vien: ";
        cin >> newBook.copies;
    }
    library.push_back(newBook);
    cout << "Sach da duoc them vao vien." << endl;
}

//Hàm xóa sách
void AdminUser::deleteBook(vector<Book> &library) {
    string isbnToDelete;
    cout << "Nhap ma sach can xoa: ";
    cin >> isbnToDelete;
    auto it = std::remove_if(library.begin(), library.end(), [&isbnToDelete](const Book &book) {
        return book.isbn == isbnToDelete;
    });
    if (it != library.end()) {
        library.erase(it, library.end());
        cout << "Sach da duoc xoa khoi thu vien. " << endl;
    } else {
        cout << "Khong tim thay sach co ma so: " << isbnToDelete << endl;
    }
}

//Hàm sửa thông tin sách
void AdminUser::editBook(vector<Book> &library) {
    string isbnToEdit;
    cout << "Nhap ma sach can sua: ";
    cin >> isbnToEdit;
    auto it = find_if(library.begin(), library.end(), [&isbnToEdit](const Book &book) {
        return book.isbn == isbnToEdit;
    });
    if (it != library.end()) {
        // Nhập thông tin mới từ người dùng
        Book editedBook = *it;  // Copy sách cần sửa đổi để không làm thay đổi sách trong thư viện khi người dùng nhập liệu
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Nhap thong tin moi cho sach (Nhan Enter de giu nguyen gia tri):" << endl;

        cout << "Ten sach: ";
        getline(cin, editedBook.title);

        cout << "Chu de: ";
        getline(cin, editedBook.subject);

        cout << "Tac gia: ";
        getline(cin, editedBook.author);

        cout << "Nha xuat ban: ";
        getline(cin, editedBook.publisher);

        cout << "Ngay xuat ban: ";
        getline(cin, editedBook.date);

        cout << "So trang: ";
        cin >> editedBook.pages;
        while (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Khong hop le. Vui long nhap. So trang: ";
            cin >> editedBook.pages;
        }
        cout << "So ban copy trong thu vien: ";
        cin >> editedBook.copies;
        while (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Khong hop le. Vui long nhap lai. So ban copy trong thu vien: ";
            cin >> editedBook.copies;
        }
        // Cập nhật sách trong thư viện
        *it = editedBook;

        cout << "Thong tin da duoc cap nhat." << endl;
    } else {
        cout << "Khong tim thay ma so: " << isbnToEdit << endl;
    }
}

void AdminUser::displayLibrary(const vector<Book> &library) {
    cout << "\n===== Danh sach sach trong thu vien =====\n";
    for (const Book &book: library) {
        displayBook(book);
    }
}

int main() {
    homeMenu();
    return 0;
}

