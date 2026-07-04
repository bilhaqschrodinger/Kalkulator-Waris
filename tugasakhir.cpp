#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
#include <fstream> 
#include <sstream>
#include <cstdlib>
using namespace std;
const string NAMA_FILE = "data_waris.txt";

struct HasilWaris {
    string nama;
    double bagianPersen;
    double nominal;
    string catatan;
};

struct DataKasus {
    int id;
    string namaAlmarhum;
    double hartaKotor, hutang, biayaMakam;
    char jenisKelaminMayit;
    int isSuamiIstriHidup;
    int isAyahHidup;
    int isIbuHidup;
    int jumlahAnakLk;
    int jumlahAnakPr;
};

vector <DataKasus> database;
int idCounter = 1;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void printCurrency(string label, double amount) {
    cout << left << setw(30) << label << ": Rp " << fixed << setprecision(2) << amount << endl;
}

double ambilInputUang(string pertanyaan) {
    string input;
    double hasil;
    while (true) {
        cout << pertanyaan;
        cin >> input;
        try {
            size_t pos;
            hasil = stod(input, &pos);
            if (pos == input.length() && hasil >= 0) return hasil;
        } catch (...) {}
        cout << "[!] Input salah. Masukkan nominal uang (angka positif)." << endl;
    }
}

int ambilInputJumlah(string pertanyaan) {
    string input;
    int hasil;
    while (true) {
        cout << pertanyaan;
        cin >> input;
        try {
            size_t pos;
            hasil = stoi(input, &pos);
            if (pos == input.length() && hasil >= 0) return hasil;
        } catch (...) {}
        cout << "[!] Input salah. Masukkan jumlah (angka bulat)." << endl;
    }
}

int ambilInputPilihan(string pertanyaan) {
    string input;
    while (true) {
        cout << pertanyaan;
        cin >> input;
        if (input == "1") return 1;
        else if (input == "0") return 0;
        else cout << "[!] Input salah. Harap masukkan 1 atau 0." << endl;
    }
}

void simpanDatabase() {
    ofstream file(NAMA_FILE); 
    if (file.is_open()) {
        for (const auto& d : database) {
            file << d.id << "|" << d.namaAlmarhum << "|" 
                 << fixed << setprecision(0) << d.hartaKotor << "|"
                 << d.hutang << "|" << d.biayaMakam << "|"
                 << d.jenisKelaminMayit << "|" << d.isSuamiIstriHidup << "|"
                 << d.isAyahHidup << "|" << d.isIbuHidup << "|"
                 << d.jumlahAnakLk << "|" << d.jumlahAnakPr << endl;
        }
        file.close();
    }
}

void muatDatabase() {
    ifstream file(NAMA_FILE);
    if (!file.is_open()) return; 
    database.clear();
    string line;
    int maxId = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string segment;
        DataKasus d;
        try {
            getline(ss, segment, '|'); d.id = stoi(segment);
            getline(ss, d.namaAlmarhum, '|');
            getline(ss, segment, '|'); d.hartaKotor = stod(segment);
            getline(ss, segment, '|'); d.hutang = stod(segment);
            getline(ss, segment, '|'); d.biayaMakam = stod(segment);
            getline(ss, segment, '|'); d.jenisKelaminMayit = segment[0];
            getline(ss, segment, '|'); d.isSuamiIstriHidup = stoi(segment);
            getline(ss, segment, '|'); d.isAyahHidup = stoi(segment);
            getline(ss, segment, '|'); d.isIbuHidup = stoi(segment);
            getline(ss, segment, '|'); d.jumlahAnakLk = stoi(segment);
            getline(ss, segment, '|'); d.jumlahAnakPr = stoi(segment);
            database.push_back(d);
            if (d.id > maxId) maxId = d.id;
        } catch (...) {}
    }
    file.close();
    idCounter = maxId + 1; 
}

void hitungDanTampilkan(DataKasus data) {
    clearScreen(); 
    cout << "\n==========================================" << endl;
    cout << "  DETAIL PERHITUNGAN: " << data.namaAlmarhum << endl;
    cout << "==========================================" << endl;

    double hartaBersih = data.hartaKotor - (data.hutang + data.biayaMakam);
    printCurrency("Harta Kotor", data.hartaKotor);
    printCurrency("Hutang + Biaya Makam", data.hutang + data.biayaMakam);
    cout << "------------------------------------------" << endl;
    printCurrency("HARTA BERSIH", hartaBersih);
    cout << "------------------------------------------" << endl;

    if (hartaBersih <= 0) {
        cout << "[!] Peringatan: Harta habis. Tidak ada warisan dibagikan." << endl;
        return;
    }

    vector<HasilWaris> hasil;
    double sisaHarta = hartaBersih;
    bool adaKeturunan = (data.jumlahAnakLk > 0 || data.jumlahAnakPr > 0);

    if (data.isSuamiIstriHidup) {
        double bagian = 0;
        string status = "";
        if (data.jenisKelaminMayit == 'P' || data.jenisKelaminMayit == 'p') {
            bagian = adaKeturunan ? 0.25 : 0.5;
            status = adaKeturunan ? "Suami (1/4)" : "Suami (1/2)";
        } else {
            bagian = adaKeturunan ? 0.125 : 0.25;
            status = adaKeturunan ? "Istri (1/8)" : "Istri (1/4)";
        }
        double nominal = hartaBersih * bagian;
        hasil.push_back({status, bagian, nominal, ""});
        sisaHarta -= nominal;
    }

    if (data.isIbuHidup) {
        double bagian = adaKeturunan ? (1.0/6.0) : (1.0/3.0);
        string status = adaKeturunan ? "Ibu (1/6)" : "Ibu (1/3)";
        double nominal = hartaBersih * bagian;
        hasil.push_back({status, bagian, nominal, ""});
        sisaHarta -= nominal;
    }

    if (data.isAyahHidup && adaKeturunan) {
        double bagian = 1.0/6.0;
        double nominal = hartaBersih * bagian;
        hasil.push_back({"Ayah (1/6 Pasti)", bagian, nominal, ""});
        sisaHarta -= nominal;
    }

    if (data.jumlahAnakLk == 0 && data.jumlahAnakPr > 0) {
        double bagian = (data.jumlahAnakPr == 1) ? 0.5 : (2.0/3.0);
        string status = (data.jumlahAnakPr == 1) ? "Anak Pr Tunggal (1/2)" : to_string(data.jumlahAnakPr) + " Anak Pr (2/3)";
        double nominal = hartaBersih * bagian;
        hasil.push_back({status, bagian, nominal, ""});
        sisaHarta -= nominal;
    }

    if (data.jumlahAnakLk > 0) {
        if(sisaHarta > 0) {
            double bagianSatuUnit = sisaHarta / (data.jumlahAnakLk * 2 + data.jumlahAnakPr);
            hasil.push_back({"Anak Laki-laki (Asabah)", 0, bagianSatuUnit * 2 * data.jumlahAnakLk, "Total semua Anak LK"});
            if (data.jumlahAnakPr > 0) {
                hasil.push_back({"Anak Perempuan (Asabah)", 0, bagianSatuUnit * data.jumlahAnakPr, "Total semua Anak PR"});
            }
            sisaHarta = 0;
        }
    } else if (data.isAyahHidup && !adaKeturunan) {
        hasil.push_back({"Ayah (Sisa/Asabah)", 0, sisaHarta, "Mengambil sisa"});
        sisaHarta = 0;
    } else if (data.isAyahHidup && data.jumlahAnakLk == 0 && data.jumlahAnakPr > 0) {
        if (sisaHarta > 0) {
            hasil.push_back({"Ayah (Sisa Tambahan)", 0, sisaHarta, "Sisa setelah Anak Pr"});
            sisaHarta = 0;
        }
    }

    for (const auto& h : hasil) printCurrency(h.nama, h.nominal);
    cout << "------------------------------------------" << endl;
    if (sisaHarta > 1.0) printCurrency("SISA TAK TERBAGI", sisaHarta);
}

void tambahKasus() {
    clearScreen();
    DataKasus d;
    d.id = idCounter++;
    
    cout << "\n--- TAMBAH KASUS BARU ---" << endl;
    cout << "Nama Almarhum/ah         : "; getline(cin, d.namaAlmarhum);
    
    d.hartaKotor = ambilInputUang("Total Harta (Rp)         : ");
    d.hutang     = ambilInputUang("Total Hutang (Rp)        : ");
    d.biayaMakam = ambilInputUang("Biaya Makam (Rp)         : ");
    
    while(true) {
        cout << "Jenis Kelamin (L/P)      : "; cin >> d.jenisKelaminMayit;
        if(d.jenisKelaminMayit == 'L' || d.jenisKelaminMayit == 'l' || 
           d.jenisKelaminMayit == 'P' || d.jenisKelaminMayit == 'p') break;
        cout << "[!] Masukkan 'L' atau 'P'" << endl;
    }
    
    string pasangan = (d.jenisKelaminMayit == 'L' || d.jenisKelaminMayit == 'l') ? "Istri" : "Suami";
    d.isSuamiIstriHidup = ambilInputPilihan("Apakah " + pasangan + " hidup? (1=Ya, 0=Tdk): ");
    d.isAyahHidup       = ambilInputPilihan("Apakah Ayah hidup? (1=Ya, 0=Tdk): ");
    d.isIbuHidup        = ambilInputPilihan("Apakah Ibu hidup? (1=Ya, 0=Tdk): ");
    d.jumlahAnakLk      = ambilInputJumlah("Jml Anak Laki-laki       : ");
    d.jumlahAnakPr      = ambilInputJumlah("Jml Anak Perempuan       : ");
    
    cin.ignore(); 
    database.push_back(d);
    simpanDatabase();
    cout << "\n[OK] Data disimpan!" << endl;
}

void lihatDaftar() {
    clearScreen();
    cout << "\n--- DAFTAR KASUS WARIS ---" << endl;
    if (database.empty()) {
        cout << "Belum ada data." << endl;
        return;
    }
    cout << left << setw(5) << "ID" << setw(20) << "Nama" << setw(15) << "Harta Bersih" << endl;
    cout << "--------------------------------------------" << endl;
    for (const auto& d : database) {
        double bersih = d.hartaKotor - (d.hutang + d.biayaMakam);
        cout << left << setw(5) << d.id << setw(20) << d.namaAlmarhum << setw(15) << (long long)bersih << endl;
    }
}

void detailKasus() {
    lihatDaftar(); 
    if (database.empty()) return;
    
    int targetId = ambilInputJumlah("\nMasukkan ID untuk dihitung: ");
    cin.ignore(); 

    bool found = false;
    for (const auto& d : database) {
        if (d.id == targetId) { 
            hitungDanTampilkan(d); 
            found = true;
            break; 
        }
    }
    if (!found) cout << "[!] ID tidak ditemukan." << endl;
}

void editKasus() {
    lihatDaftar();
    if (database.empty()) return;

    int targetId = ambilInputJumlah("\nMasukkan ID Edit: ");
    cin.ignore(); 

    for (auto& d : database) {
        if (d.id == targetId) {
            clearScreen();
            cout << "\n--- EDIT DATA (ID: " << d.id << ") ---" << endl;
            cout << "Nama Baru (" << d.namaAlmarhum << "): "; getline(cin, d.namaAlmarhum);
            d.hartaKotor = ambilInputUang("Harta Baru: ");
            d.hutang     = ambilInputUang("Hutang Baru: ");
            cin.ignore(); 
            simpanDatabase();
            cout << "[OK] Data diperbarui." << endl;
            return;
        }
    }
    cout << "[!] ID tidak ditemukan." << endl;
}

void hapusKasus() {
    lihatDaftar();
    if (database.empty()) return;
    int targetId = ambilInputJumlah("\nMasukkan ID Hapus: ");
    cin.ignore(); 

    auto awal = database.size();
    database.erase(remove_if(database.begin(), database.end(), 
        [targetId](const DataKasus& d) { return d.id == targetId; }), database.end());

    if (database.size() < awal) {
        simpanDatabase(); 
        cout << "[OK] Data dihapus." << endl;
    } else {
        cout << "[!] ID tidak ditemukan." << endl;
    }
}

int main() {
    muatDatabase(); 
    string inputPilihan;
    
    do {
        clearScreen();
        cout << "\n==============================================" << endl;
        cout << "  WARISCALC 'PROGRAM PENGHITUNG HARTA WARIS'  " << endl;
        cout << "==============================================" << endl;
        cout << "1. Tambah Kasus" << endl;
        cout << "2. Lihat Daftar" << endl;
        cout << "3. Hitung Detail" << endl;
        cout << "4. Edit Data" << endl;
        cout << "5. Hapus Kasus" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilihan: "; cin >> inputPilihan;
        cin.ignore();

        if (inputPilihan == "1") tambahKasus();
        else if (inputPilihan == "2") lihatDaftar();
        else if (inputPilihan == "3") detailKasus();
        else if (inputPilihan == "4") editKasus();
        else if (inputPilihan == "5") hapusKasus();
        else if (inputPilihan == "0") cout << "Keluar dari program..." << endl;
        else cout << "Salah input." << endl;

        if (inputPilihan != "0") {
            cout << "\nTekan Enter untuk kembali ke menu...";
            cin.get(); 
        }

    } while (inputPilihan != "0");

    return 0;
}