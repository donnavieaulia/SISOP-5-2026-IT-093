# Praktikum Sistem Operasi - Modul 5

## Soal 2 - Season

Nama:Donnavie Aulia
NRP:5027251093
Kelas:B

---

## Deskripsi Soal

Pada soal ini, diminta untuk membangun sebuah mini operating system sederhana berbasis 16-bit yang berjalan di atas emulator **Bochs x86-64**. Sistem ini terdiri dari bootloader (`bootloader.asm`), kernel assembly (`kernel.asm`), dan kernel C (`kernel.c`). Kita hanya perlu mengisi fungsi `_getChar` pada `kernel.asm` dan mengimplementasikan seluruh fitur perintah pada `kernel.c`.

Sistem yang dibangun berfungsi layaknya sebuah shell sederhana yang menerima input dari keyboard, memproses perintah, dan menampilkan output pada layar VGA text mode.

---

## Struktur Repository

```
soal_2/
├── Makefile
├── README.md
├── bochsrc.txt
├── bootloader.asm
├── build.sh
├── kernel.asm
└── kernel.c
```

---

## Cara Build dan Menjalankan

### Linux

```bash
make build
make run
```

### MacOS

```bash
chmod +x build.sh
./build.sh
```

---

## Penjelasan Pengerjaan

### 1. `kernel.asm` — Fungsi `_getChar`

Fungsi `_getChar` bertugas membaca satu karakter input dari keyboard menggunakan BIOS interrupt. Fungsi ini dipanggil dari `kernel.c` untuk mendapatkan karakter yang diinput oleh pengguna.

```asm
_getChar:
    mov ah, 0
    int 0x16
    xor ah, ah
    ret
```

Penjelasan cara kerjanya:

- `mov ah, 0` → menyiapkan fungsi BIOS interrupt `0x16` dengan kode `AH=0`, yang berarti "tunggu sampai ada tombol ditekan dan kembalikan karakternya".
- `int 0x16` → memanggil BIOS keyboard interrupt. Setelah tombol ditekan, karakter ASCII tersimpan di register `AL`, dan scan code di `AH`.
- `xor ah, ah` → mengosongkan register `AH` sehingga nilai return yang diteruskan ke C hanya berisi kode ASCII dari `AL`.
- `ret` → kembali ke pemanggil.

---

### 2. `kernel.c` — Implementasi Kernel Shell

#### Fungsi Dasar

Beberapa fungsi helper yang sudah disediakan oleh template dan digunakan di seluruh fitur:

**`printChar(char c)`**

Menulis satu karakter ke video memory VGA pada posisi cursor saat ini. Segmen `0xB800` adalah alamat video memory untuk text mode.

```c
void printChar(char c) {
    putInMemory(0xB800, cursor * 2, c);
    putInMemory(0xB800, cursor * 2 + 1, color);
    cursor++;
}
```

**`printString(char* str)`**

Mencetak string karakter demi karakter hingga null terminator ditemukan.

```c
void printString(char* str) {
    int i = 0;
    while (str[i] != 0) {
        printChar(str[i]);
        i++;
    }
}
```

**`clearScreen()`**

Membersihkan layar dengan menimpa seluruh karakter di video memory dengan spasi, lalu mereset cursor ke posisi awal.

```c
void clearScreen() {
    int i;
    cursor = 0;
    for (i = 0; i < 80 * 25; i++) {
        putInMemory(0xB800, i * 2, ' ');
        putInMemory(0xB800, i * 2 + 1, color);
    }
}
```

**`readString(char* str)`**

Membaca input dari keyboard karakter demi karakter menggunakan `getChar()`. Berhenti saat Enter (`\r` / karakter 13) ditekan. Mendukung backspace (karakter 8) untuk menghapus karakter terakhir.

```c
void readString(char* str) {
    int i = 0;
    char c;
    while (1) {
        c = getChar();
        if (c == 13) {
            str[i] = 0;
            break;
        } else if (c == 8) {
            if (i > 0) {
                i--;
                cursor--;
                printChar(' ');
                cursor--;
            }
        } else {
            str[i] = c;
            printChar(c);
            i++;
        }
    }
}
```

---

#### Fitur `check`

Fitur ini berfungsi sebagai verifikasi bahwa sistem berjalan dengan baik. Ketika pengguna mengetik `check`, sistem akan mencetak `ok`.

```c
if (strcmp(cmd, "check")) {
    printString("ok");
}
```

Output:
```
> check
ok
```

---

#### Fitur `add`

Fitur `add` menerima dua buah angka sebagai argumen, lalu menjumlahkan keduanya dan mencetak hasilnya. String argumen dikonversi ke integer menggunakan fungsi `atoi`, lalu hasilnya dikonversi kembali ke string dengan `intToString`.

```c
else if (startsWith(cmd, "add ")) {
    // parsing argumen num1 dan num2 dari string cmd
    a = atoi(num1);
    b = atoi(num2);
    intToString(a + b, hasil);
    printString(hasil);
}
```

Output:
```
> add 5 3
8
```

---

#### Fitur `sub`

Fitur `sub` bekerja sama seperti `add`, namun melakukan operasi pengurangan antara dua argumen.

```c
else if (startsWith(cmd, "sub ")) {
    // parsing argumen num1 dan num2
    a = atoi(num1);
    b = atoi(num2);
    intToString(a - b, hasil);
    printString(hasil);
}
```

Output:
```
> sub 10 2
8
```

---

#### Fitur `fac`

Fitur `fac` menghitung faktorial dari angka yang diberikan. Karena sistem berjalan dalam mode **16-bit**, ada batasan nilai integer yang dapat direpresentasikan. Jika hasil faktorial melebihi batas (overflow), sistem akan mencetak pesan `know your limit little bro.`

Fungsi faktorial yang digunakan:

```c
int factorial(int n) {
    int hasil = 1;
    int i;
    for (i = 1; i <= n; i++) {
        hasil = hasil * i;
    }
    return hasil;
}
```

Penanganan overflow pada pemanggilan `fac`:

```c
else if (startsWith(cmd, "fac ")) {
    n = atoi(angka);
    if (n > 7) {
        printString("know your limit little bro.");
    } else {
        intToString(factorial(n), hasil);
        printString(hasil);
    }
}
```

Output:
```
> fac 6
720
> fac 120
know your limit little bro.
```

---

#### Fitur `season`

Fitur `season` mengubah warna teks pada terminal sesuai dengan musim yang dipilih. Terdapat 5 pilihan musim, masing-masing dengan warna berbeda yang diatur melalui variabel global `color` (VGA color attribute byte).

| Perintah | Warna | Color Code |
|----------|-------|------------|
| `season winter` | Cyan | `0x03` |
| `season spring` | Green | `0x02` |
| `season summer` | Yellow | `0x0E` |
| `season fall` | Magenta | `0x05` |
| `season radiant` | Bright White | `0x0F` |

```c
else if (startsWith(cmd, "season ")) {
    if (strcmp(cmd, "season winter")) {
        color = 0x03;
        printString("winter mode");
    } else if (strcmp(cmd, "season spring")) {
        color = 0x02;
        printString("spring mode");
    } else if (strcmp(cmd, "season summer")) {
        color = 0x0E;
        printString("summer mode");
    } else if (strcmp(cmd, "season fall")) {
        color = 0x05;
        printString("fall mode");
    } else if (strcmp(cmd, "season radiant")) {
        color = 0x0F;
        printString("radiant mode");
    } else {
        printString("season not found");
    }
}
```

Output:
```
> season winter
winter mode        ← teks berwarna cyan
> season spring
spring mode        ← teks berwarna hijau
```

---

#### Fitur `triangle`

Fitur `triangle` mencetak segitiga bintang (*) dengan tinggi sesuai angka yang diberikan. Baris pertama memiliki 1 bintang, baris kedua 2 bintang, dan seterusnya.

```c
else if (startsWith(cmd, "triangle ")) {
    n = atoi(angka);
    for (i = 1; i <= n; i++) {
        for (j = 0; j < i; j++) {
            printChar('x');
        }
        newline();
    }
}
```

Output:
```
> triangle 5
x
xx
xxx
xxxx
xxxxx
```

---

#### Fitur `clear`

Fitur `clear` membersihkan seluruh layar terminal dengan memanggil fungsi `clearScreen()`, lalu mereset posisi cursor ke baris pertama.

```c
else if (strcmp(cmd, "clear")) {
    clearScreen();
}
```

---

#### Fitur `help`

Fitur `help` mencetak daftar semua perintah yang tersedia pada sistem, memudahkan pengguna untuk mengetahui command apa saja yang bisa digunakan.

```c
else if (strcmp(cmd, "help")) {
    printString("check add sub fac season triangle clear about help");
}
```

Output:
```
> help
check add sub fac season triangle clear about help
```

---

#### Loop Utama `main()`

Seluruh fitur di atas dijalankan di dalam loop utama pada fungsi `main()`. Sistem terus menunggu input dari pengguna, memproses perintah yang diberikan, lalu menampilkan hasilnya.

```c
void main() {
    char cmd[64];
    clearScreen();
    printString("Welcome to Assistant's Last Gift");
    newline();
    printString("type 'help'");
    newline();
    newline();

    while (1) {
        printString("> ");
        readString(cmd);
        newline();
        // ... penanganan setiap perintah
        newline();
    }
}
```

---

## Kendala dan Cara Penyelesaian

| Kendala | Solusi |
|---------|--------|
| Fungsi `_getChar` belum diimplementasikan sehingga input tidak terbaca | Menggunakan BIOS interrupt `int 0x16` dengan `AH=0` untuk membaca keystroke |
| Sistem 16-bit memiliki keterbatasan integer sehingga faktorial besar overflow | Menambahkan pengecekan batas nilai `n` sebelum menghitung faktorial |
| Warna teks tidak berubah setelah perintah `season` | Memastikan variabel global `color` diubah sebelum `printString` dipanggil agar karakter berikutnya menggunakan warna baru |



