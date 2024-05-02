#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

bool WriteToFile(const string& str) {
    ofstream inFile("C:\\pc_info\\info_test.txt");
    if (!inFile.is_open()) {
        cerr << "ОШИБКА ЗАПИСИ ФАЙЛА!";
        return false;
    }

    inFile << str;
    inFile.close();
    return true;
}

bool AddInv(string &str) {
    string buffer;
    string answer;

    cout << "\t-----ИНВЕНТАРНЫЙ НОМЕР ПК-----";

    do {
        cout << endl << "Введите инвентарный номер: ";
        cin >> buffer;
        cout << endl << "Вы ввели: " << buffer << endl << "Верно? (Y/n, Д/н): ";
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        getline(cin, answer);
    } while (!answer.empty() and answer != "y" and answer != "Y" and answer != "д" and answer != "Д");

    str = buffer;
    return true;
}

bool AddInvMonitors(string &str) {
    string buffer;
    string answer;
    int count_monitors = 1;
    bool is_good_digit = false;

    cout << endl << endl << endl;
    cout << "\t-----ИНВЕНТАРНЫЙ НОМЕР МОНИТОРА-----";
    cout << endl << endl;

    do {
        do {
            cout << "Сколько мониторов на этом компьютере: ";
            cin >> count_monitors;

            if (!cin.good()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                is_good_digit = true;
            }

        } while (!is_good_digit or count_monitors < 0);

        cout << endl << "У этого ПК " << count_monitors << " мониторов" << endl << "Верно? (Y/n, Д/н): ";
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        getline(cin, answer);

    } while (!answer.empty() and answer != "y" and answer != "Y" and answer != "д" and answer != "Д");


    answer = 'n';


    for (int i = 0; i < count_monitors; ++i) {
        cout << endl << endl;
        do {
            cout << "Введите инвентарный номер " << i + 1 << " монитора: ";
            cin >> buffer;
            cout << endl << "Инвентарный номер " << i + 1 << " монитора: " << buffer << endl << "Верно? (Y/n, Д/н): ";
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getline(cin, answer);
        } while (!answer.empty() and answer != "y" and answer != "Y" and answer != "д" and answer != "Д");
        str += buffer + "; ";
    }
    return true;
}

bool AddCPUName(string &str) {
    string buffer, cpu_name;

    cout << "\t-----CPU-----" << endl;

    system(R"(wmic cpu get Name /format:list | FindStr "Name" > "c:\pc_info\1.txt")");
    ifstream outFile("C:\\pc_info\\1.txt");
    if (!outFile.is_open()) {
        cerr << "ВРЕМЕННЫЙ ФАЙЛ НЕ ОТКРЫЛСЯ";
        return false;
    }
    getline(outFile, buffer);
    outFile.close();

    for (int i = 5; i < buffer.size() - 1; ++i) {
        cpu_name += buffer[i];
    }

    str = cpu_name;

    return true;
}

bool AddCPUSocket(string &str) {
    string buffer, cpu_socket;

    system(R"(wmic cpu get SocketDesignation /format:list | FindStr "SocketDesignation" > "c:\pc_info\1.txt")");
    ifstream outFile("C:\\pc_info\\1.txt");
    if (!outFile.is_open()) {
        cerr << "ВРЕМЕННЫЙ ФАЙЛ НЕ ОТКРЫЛСЯ";
        return false;
    }
    getline(outFile, buffer);
    outFile.close();

    for (int i = 18; i < buffer.size() - 1; ++i) {
        cpu_socket += buffer[i];
    }

    if (cpu_socket == "U3E1") {
        cpu_socket = "LGA1700";
    }

    str = cpu_socket;

    return true;
}

bool AddDDR(string &str) {
    string buffer, ddr_type, full_info_ddr;

    cout << "\t-----RAM-----" << endl;

    system(R"(wmic MemoryChip get SMBIOSMemoryType /format:list | FindStr "SMBIOSMemoryType=" > "c:\pc_info\1.txt")");
    ifstream outFile("C:\\pc_info\\1.txt");
    if (!outFile.is_open()) {
        cerr << "ВРЕМЕННЫЙ ФАЙЛ НЕ ОТКРЫЛСЯ";
        return false;
    }
    outFile >> buffer;
    outFile.close();

    if (buffer == "SMBIOSMemoryType=21" or buffer == "SMBIOSMemoryType=22" ) {
        full_info_ddr += "DDR2 ";
    } else if (buffer == "SMBIOSMemoryType=24" or buffer == "SMBIOSMemoryType=25" or buffer == "SMBIOSMemoryType=18") {
        full_info_ddr += "DDR3 ";
    } else if (buffer == "SMBIOSMemoryType=26") {
        full_info_ddr += "DDR4 ";
    } else if (buffer == "SMBIOSMemoryType=34" or buffer == "SMBIOSMemoryType=28") {
        full_info_ddr += "DDR5 ";
    } else if (buffer == "SMBIOSMemoryType=0") {
        cout << endl << "ТИП ПАМЯТИ - Неизвестен" << endl;
        system("pause");
    } else {
        cout << endl << "ОШИБКА ТИПА ПАМЯТИ" << endl;
        system("pause");
        return false;
    };

    str = full_info_ddr;

    return true;
}

bool AddDDRCapacity(string &str) {
    string buffer, full_info_ddr;
    double capacity = 0;

    system(R"(wmic MemoryChip get Capacity /format:list | FindStr "Capacity" > "c:\pc_info\1.txt")");
    ifstream outFile2("C:\\pc_info\\1.txt");
    if (!outFile2.is_open()) {
        cerr << "ВРЕМЕННЫЙ ФАЙЛ НЕ ОТКРЫЛСЯ";
        return false;
    }

    while (outFile2 >> buffer) {

        string capacity_bits;
        for (int i = 9; i < buffer.size(); ++i) {
            capacity_bits += buffer[i];
        }

        string gb;
        gb = capacity_bits;
        capacity += stod(gb);
    }

    outFile2.close();

    int capacity_int = static_cast<int>(capacity / 1024 / 1024 / 1024);
    full_info_ddr += to_string(capacity_int) + "Gb";

    str = full_info_ddr;

    return true;

}

bool AddGPU(string &str) {
    string buffer, gpu_name;

    cout << "\t-----GPU-----" << endl;

    system(R"(WMIC path Win32_VideoController get Name /format:list | FindStr "Name" > "c:\pc_info\1.txt")");
    ifstream outFile("C:\\pc_info\\1.txt");
    if (!outFile.is_open()) {
        cerr << "ВРЕМЕННЫЙ ФАЙЛ НЕ ОТКРЫЛСЯ";
        return false;
    }
    getline(outFile, buffer);
    outFile.close();

    for (int i = 5; i < buffer.size() - 1; ++i) {
        gpu_name += buffer[i];
    }

    str = gpu_name;

    return true;
}

bool AddMotherboard(string &str) {
    string buffer, mother_manufacturer, mother_model, full_info_mother;

    cout << "\t-----Motherboard-----" << endl;

    system(R"(WMIC baseboard get Manufacturer /format:list | FindStr "Manufacturer" > "c:\pc_info\1.txt")");
    ifstream outFile("C:\\pc_info\\1.txt");
    if (!outFile.is_open()) {
        cerr << "ВРЕМЕННЫЙ ФАЙЛ НЕ ОТКРЫЛСЯ";
        return false;
    }
    getline(outFile, buffer);
    outFile.close();

    for (int i = 13; i < buffer.size() - 1; ++i) {
        mother_manufacturer += buffer[i];
    }

    system(R"(wmic baseboard get Product /format:list | FindStr "Product" > "c:\pc_info\1.txt")");
    ifstream outFile2("C:\\pc_info\\1.txt");
    if (!outFile2.is_open()) {
        cerr << "ВРЕМЕННЫЙ ФАЙЛ НЕ ОТКРЫЛСЯ";
        return false;
    }
    getline(outFile2, buffer);
    outFile2.close();

    for (int i = 8; i < buffer.size() - 1; ++i) {
        mother_model += buffer[i];
    }

    full_info_mother = mother_manufacturer + ' ' + mother_model;

    str = full_info_mother;

    return true;
}


bool AddDrives(string &str) {
    string buffer, full_info_drives;
    vector<string> drives_name, drives_size;

    cout << "\t-----Drives-----" << endl;

    system(R"(wmic diskdrive get caption /format:list | FindStr "Caption" > "c:\pc_info\1.txt")");
    ifstream outFile("C:\\pc_info\\1.txt");
    if (!outFile.is_open()) {
        cerr << "ВРЕМЕННЫЙ ФАЙЛ НЕ ОТКРЫЛСЯ";
        return false;
    }

    while (getline(outFile, buffer)) {

        string drive_name;
        for (int i = 8; i < buffer.size() - 1; ++i) {
            drive_name += buffer[i];
        }
        drives_name.push_back(drive_name);

    }

    outFile.close();
    buffer = "";


    system(R"(wmic diskdrive get Size /format:list | FindStr "Size" > "c:\pc_info\1.txt")");
    ifstream outFile2("C:\\pc_info\\1.txt");
    if (!outFile2.is_open()) {
        cerr << "ВРЕМЕННЫЙ ФАЙЛ НЕ ОТКРЫЛСЯ";
        return false;
    }

    while (getline(outFile2, buffer)) {

        string drive_size;
        for (int i = 5; i < buffer.size() - 1; ++i) {
            drive_size += buffer[i];
        }
        drives_size.push_back(drive_size);

    }

    outFile2.close();

    for (int i = 0; i < drives_size.size();) {
        if (drives_name[i].find("USB") != string::npos or drives_name[i].find("usb") != string::npos){
            drives_name.erase(drives_name.begin() + i);
            drives_size.erase(drives_size.begin() + i);
        } else if (!drives_size.empty()){
            double disk_size_gb = stod(drives_size[i]) / 1000 / 1000 / 1000;
            int disk_size_int = static_cast<int>(disk_size_gb);
            drives_size[i] = to_string(disk_size_int);
            i++;
        }
    }

    for (int i = 0; i < drives_name.size(); ++i) {
        full_info_drives += "Drive " + to_string(i + 1) + " - " + drives_name[i] + " - " + drives_size[i] + "Gb; ";
    }

    str = full_info_drives;

    return true;
}

bool ReadFromFile(vector<string> &texts) {
    string buffer;

    ifstream outFile("C:\\pc_info\\info.txt");
    if (!outFile.is_open()) {
        cerr << "ВРЕМЕННЫЙ ФАЙЛ НЕ ОТКРЫЛСЯ";
        return false;
    }

    int iterator = 0;
    while (getline(outFile, buffer)) {

        texts.push_back(buffer);

        iterator++;
    }

    outFile.close();

    return true;
}

string AddDevicesInfo() {
    string buffer, str;

    str += "Devices: ";
    cout << endl << endl;


    str += "CPU - ";
    if (AddCPUName(buffer)) {
        cout << endl << "Название CPU добавлено" << endl;
        str += buffer;
        str += "; ";
    } else {
        cout << endl << "Ошибка добавления названия CPU";
        cout << endl << "Введите данную информацию позднее вручную";
    }
    buffer = "";

    str += "Socket - ";
    if (AddCPUSocket(buffer)) {
        cout << endl << "Информация с CPU сокете добавлена" << endl;
        str += buffer;
        str += "; ";
    } else {
        cout << endl << "Ошибка добавления информации о CPU сокете";
        cout << endl << "Введите данную информацию позднее вручную";
    }
    buffer = "";


    cout << endl;
    str += "RAM - ";
    if (AddDDR(buffer)) {
        cout << endl << "Информация о RAM добавлена" << endl;
        str += buffer;
    } else {
        cout << endl << "Ошибка добавления информации о RAM";
        cout << endl << "Введите данную информацию позднее вручную";
    }
    buffer = "";

    if (AddDDRCapacity(buffer)) {
        cout << endl << "Информация об объеме RAM добавлена" << endl;
        str += buffer;
        str += "; ";
    } else {
        cout << endl << "Ошибка добавления информации об объеме RAM";
        cout << endl << "Введите данную информацию позднее вручную";
    }
    buffer = "";


    cout << endl;
    str += "GPU - ";
    if (AddGPU(buffer)) {
        cout << endl << "Информация о GPU добавлена" << endl;
        str += buffer;
        str += "; ";
    } else {
        cout << endl << "Ошибка добавления информации о GPU";
        cout << endl << "Введите данную информацию позднее вручную";
    }
    buffer = "";


    cout << endl;
    str += "Motherboard - ";
    if (AddMotherboard(buffer)) {
        cout << endl << "Информация о материнской плате добавлена" << endl;
        str += buffer;
        str += "; ";
    } else {
        cout << endl << "Ошибка добавления информации о материнской плате";
        cout << endl << "Введите данную информацию позднее вручную";
    }
    buffer = "";


    cout << endl;
    if (AddDrives(buffer)) {
        cout << endl << "Информация о жестких дисках добавлена" << endl;
        str += buffer;
    } else {
        cout << endl << "Ошибка добавления информации о жестких дисках";
        cout << endl << "Введите данную информацию позднее вручную";
    }
    buffer = "";

    return str;
}

void Check(string &str, string &whois) {
    string chek;

    do {
        if (whois == "dalet"){
            cout << endl << endl << "Этот ПК - Dalet? (Y/n, Д/н): ";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (whois == "monoblock"){
            cout << endl << endl << "Этот ПК - моноблок? (Y/n, Д/н): ";
        }

        getline(cin, chek);
        if (chek.empty() or chek == "y" or chek == "Y" or chek == "д" or chek == "Д"){
            if (whois == "dalet") {
                str += "Dalet; ";
            }else if (whois == "monoblock"){
                str += "Monoblock; ";
            }
            break;
        }else if (chek == "n" or chek == "N" or chek == "н" or chek == "Н"){
            break;
        }
    } while (!chek.empty() and chek != "y" and chek != "Y" and chek != "n" and chek != "N"
             and chek != "д" and chek != "Д" and chek != "н" and chek != "Н");

}

void AddFullInfo(string &info_to_file) {

    string buffer;
    AddInv(buffer);
    cout << endl << "Инвентарный номер ПК добавлен" << endl;
    info_to_file += "Inv: ";
    info_to_file += buffer;
    info_to_file += "; \n";
    info_to_file += "Inv monitors: ";
    buffer = "";

    AddInvMonitors(buffer);
    cout << endl << "Инвентарный номер монитора добавлен" << endl;
    info_to_file += buffer;
    info_to_file += "\n";

    info_to_file += AddDevicesInfo();

    info_to_file += "\n";
    info_to_file += "Other: ";

    cout << endl << "\t-----Другие данные-----" << endl;


    int office_number;
    cout << endl << "Введите номер кабинета: ";
    while (!(cin >> office_number)) {
        cout << endl << "Введите номер кабинета: ";
        cin >> office_number;
        cin.clear();
        fflush(stdin);
    }

    cout << endl << "Номер кабинета " << office_number;
    info_to_file += "Office - " + to_string(office_number) + "; ";

    buffer = "";

    string whois = "dalet";

    Check(buffer, whois);
    info_to_file += buffer;

    buffer = "";
    whois = "monoblock";

    Check(buffer, whois);
    info_to_file += buffer;

    buffer = "";

}


void UpdateInvPC(string &str) {
    vector<string> message;
    string buffer, full_info;

    ReadFromFile(message);
    AddInv(buffer);

    message[0] = "Inv: " + buffer + "; ";

    for (auto & i : message) {
        full_info += i + "\n";
    }

    str = full_info;
}

void UpdateInvMonitors(string &str) {
    vector<string> message;
    string buffer, full_info;

    ReadFromFile(message);
    AddInvMonitors(buffer);

    message[1] = "Inv monitors: " + buffer;

    for (auto & i : message) {
        full_info += i + "\n";
    }

    str = full_info;
}

void UpdateDevicesInfo(string &str) {
    vector<string> message;
    string buffer, full_info;

    ReadFromFile(message);
    buffer = AddDevicesInfo();

    message[2] = buffer;

    for (auto & i : message) {
        full_info += i + "\n";
    }

    str = full_info;
}

int main() {
    setlocale(LC_ALL, "Russian.u8");
    system("chcp 65001");
    system("title Inventory");
    string buffer, info_to_file;
    char command;

    system(R"(IF NOT EXIST "C:\pc_info" mkdir C:\pc_info)");

    cout << "\tПРОГРАММА ИНВЕНТАРИЗАЦИИ" << endl << endl;
    cout << "\tВведите номер задачи" << endl;
    cout << "\t1 - Добавить информацию о новом компьютере" << endl;
    cout << "\t2 - Обновить инвентарный номер компьютера" << endl;
    cout << "\t3 - Обновить инвентарный номер монитора" << endl;
    cout << "\t4 - Обновить информацию о железе" << endl;
    cout << "\t5 - Открыть файл с информацией на этом ПК" << endl;
    cout << "\t0 - Выход" << endl << endl;

    while (true) {

        do {
            cout << "Команда: ";
            cin >> command;

        } while (command < '0' or command > '5');

        if (command == '1') {
            AddFullInfo(info_to_file);
            break;
        } else if (command == '2') {
            UpdateInvPC(info_to_file);
            break;
        } else if (command == '3') {
            UpdateInvMonitors(info_to_file);
            break;
        } else if (command == '4') {
            UpdateDevicesInfo(info_to_file);
            break;
        } else if (command == '5') {
            system("C:\\pc_info\\info.txt");
            continue;
        } else if (command == '0') {
            cout << endl << "\tПрограмма завершена" << endl;
            system("pause");
            return 0;
        }
    }


    WriteToFile(info_to_file);
    cout << endl << endl;
    cout << endl << "\tПроверьте информацию в документе, сохраните и закройте его" << endl;
    system("C:\\pc_info\\info_test.txt");
    system(R"(COPY "C:\pc_info\info_test.txt" "C:\pc_info\info.txt")");
    system(R"(del "C:\pc_info\1.txt")");
    system(R"(del "C:\pc_info\info_test.txt")");


    cout << endl << "\tПрограмма завершена" << endl;
    system("pause");
    return 0;
}