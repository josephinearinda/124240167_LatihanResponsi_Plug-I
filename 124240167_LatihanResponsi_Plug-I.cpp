#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

struct Pelaku {
    string nama;
    string posisi;
    int nominal;
    Pelaku* left;
    Pelaku* right;
};

struct Aksi {
    string tipe; 
    Pelaku* data;
    Aksi* next;
};

Pelaku* root = nullptr;    
Aksi* top = nullptr; 

Pelaku* buatNode(string nama, string posisi, int nominal) {
    Pelaku* baru = new Pelaku{nama, posisi, nominal, nullptr, nullptr};
    return baru;
}

Pelaku* insert(Pelaku* root, Pelaku* baru) {
    if (!root) return baru;
    if (baru->nama < root->nama) root->left = insert(root->left, baru);
    else root->right = insert(root->right, baru);
    return root;
}

void printHeader() {
    cout << "==============================================================\n";
    cout << "| Nama                   | Posisi           | Nominal      |\n";
    cout << "--------------------------------------------------------------\n";
}
void printFooter() {
    cout << "==============================================================\n";
}
void inorder(Pelaku* root) {
    if (root) {
        inorder(root->left);
        cout << "| " << setw(23) << left << root->nama
             << "| " << setw(17) << left << root->posisi
             << "| " << setw(13) << left << root->nominal << "|\n";
        inorder(root->right);
    }
}

void tambahData() {
    string nama, posisi;
    int nominal;
    cout << "Nama   : "; cin.ignore(); getline(cin, nama);
    cout << "Posisi : "; getline(cin, posisi);
    cout << "Nominal :  "; cin >> nominal;

    Pelaku* baru = buatNode(nama, posisi, nominal);
    root = insert(root, baru);

    top = new Aksi{"tambah", baru, top}; 
    cout << "Data berhasil ditambahkan!\n";
}

void tampilData() {
    cout << "================================\n";
    cout << "      DAFTAR TERDUGA PELAKU      \n";
    cout << "================================\n";
    if (!root) {
        cout << "Data belom ada!\n";
    } else {
        printHeader();
        inorder(root);
        printFooter();
    }
}

Pelaku* minValueNode(Pelaku* node) {
    Pelaku* current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return current;
}

Pelaku* hapusNode(Pelaku* root, string nama, Pelaku*& terhapus) {
    if (!root) return nullptr;
    if (nama < root->nama) root->left = hapusNode(root->left, nama, terhapus);
    else if (nama > root->nama) root->right = hapusNode(root->right, nama, terhapus);
    else {
        terhapus = root;
        if (!root->left) return root->right;
        else if (!root->right) return root->left;
        Pelaku* pengganti = minValueNode(root->right);
        root->nama = pengganti->nama;
        root->posisi = pengganti->posisi;
        root->nominal = pengganti->nominal;
        root->right = hapusNode(root->right, pengganti->nama, terhapus);
    }
    return root;
}

void hapusData() {
    string nama;
    cout << "Nama pelaku: "; cin.ignore(); getline(cin, nama);

    Pelaku* terhapus = nullptr;
    root = hapusNode(root, nama, terhapus);

    if (terhapus != nullptr) {
        Pelaku* simpan = buatNode(terhapus->nama, terhapus->posisi, terhapus->nominal);
        top = new Aksi{"hapus", simpan, top};
        cout << "Data berhasil dihapus!\n";
    } else {
        cout << "Data tidak ditemukan!\n";
    }
}

void undoAksi() {
    if (top == nullptr) {
        cout << "Tidak ada aksi yang bisa di undo!\n";
        return;
    }
    Aksi* aksiTerakhir = top;
    top = top->next;

    if (aksiTerakhir->tipe == "tambah") {
        Pelaku* dummy = nullptr;
        root = hapusNode(root, aksiTerakhir->data->nama, dummy);
        cout << "Undo berhasil! Data penambahan dibatalkan!\n";
    } else if (aksiTerakhir->tipe == "hapus") {
        Pelaku* kembali = buatNode(aksiTerakhir->data->nama, aksiTerakhir->data->posisi, aksiTerakhir->data->nominal);
        root = insert(root, kembali);
        cout << "Undo berhasil! Data yang dihapus sudah dikembalikan!\n";
    }
}

int main() {
    int pilihan;
    do {
        cout << "\n========================================\n";
        cout << "  Selamat Datang di Aplikasi Audit MBG  \n";
        cout << "========================================\n";
        cout << "              MENU APLIKASI              \n";
        cout << "----------------------------------------\n";
        cout << "1. Tambah Data Pelaku\n";
        cout << "2. Tampilkan Daftar Pelaku\n";
        cout << "3. Hapus Data Pelaku\n";
        cout << "4. Undo Aksi Terakhir\n";
        cout << "5. Keluar\n";
        cout << "PILIH MENU: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: tambahData(); break;
            case 2: tampilData(); break;
            case 3: hapusData(); break;
            case 4: undoAksi(); break;
            case 5: cout << "TERIMA KASIH TELAH MENGGUNAKAN APLIKASI MBG!\n"; break;
            default: cout << "PILIHAN TIDAK VALID!\n";
        }
    } while (pilihan != 5);
    return 0;
}