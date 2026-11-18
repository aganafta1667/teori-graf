# Praktikum 2 - Mengimplementasikan Program untuk Menyelesaikan Permasalahan "The Knight's Tour"

|    NRP     |          Nama          |
| :--------: | :--------------------: |
| 5025251055 | Aga Nafta Filadelfiano |
| 5025251067 |   Azka Fairus Syamsa   |
| 5025251106 | Asher Yedijah Hoesono  |

## The Knight's Tour

**The Knight's Tour** adalah permasalahan dalam Teori Graf yang mengharuskan bidak kuda (_knight_) pada permainan catur mengunjungi seluruh kotak papan catur tepat satu kali menggunakan gerakan berbentuk huruf L.

Pada papan berukuran 8×8 terdapat 64 kotak. Kuda harus melakukan 63 perpindahan sehingga seluruh kotak dikunjungi tanpa pengulangan.

Permasalahan ini dapat dimodelkan menggunakan graf, yaitu:

- **Vertex:** Merepresentasikan setiap kotak papan catur.
- **Edge:** Merepresentasikan hubungan antara dua kotak yang dapat dijangkau melalui satu gerakan kuda.
- **Path:** Urutan kotak yang dilalui oleh kuda.

Program mendukung dua mode perjalanan:

1. **Open Tour:** Kuda mengunjungi seluruh kotak tepat satu kali tanpa harus kembali ke titik awal. Permasalahan ini berkaitan dengan pencarian _Hamiltonian Path_.
2. **Closed Tour:** Kuda mengunjungi seluruh kotak tepat satu kali dan posisi terakhir terhubung dengan posisi awal melalui satu gerakan kuda yang valid. Permasalahan ini berkaitan dengan pencarian _Hamiltonian Cycle_.

### Metode Penyelesaian

Pada praktikum ini digunakan gabungan tiga pendekatan:

**1. Depth-First Search (DFS)**

DFS menelusuri suatu kemungkinan jalur sedalam mungkin sebelum mencoba jalur alternatif.

**2. Backtracking**

Jika suatu jalur tidak dapat menghasilkan perjalanan yang valid, algoritma membatalkan langkah terakhir dan mencoba kemungkinan lainnya.

**3. Warnsdorff's Heuristic**

Heuristik Warnsdorff mengutamakan langkah menuju kotak yang memiliki jumlah kemungkinan langkah lanjutan paling sedikit.

Tujuannya adalah mengunjungi kotak yang sulit dijangkau terlebih dahulu agar tidak tertinggal pada akhir pencarian.

Ketiga pendekatan ini dikombinasikan untuk mengurangi jumlah percabangan yang perlu dieksplorasi dibandingkan pencarian DFS tanpa pengurutan kandidat.

---

## Algoritma

Langkah-langkah penyelesaian Knight's Tour:

1. Inisialisasi papan catur berukuran 8×8 menggunakan nilai `-1` sebagai penanda kotak yang belum dikunjungi.
2. Pengguna memilih mode Open Tour atau Closed Tour.
3. Pengguna memasukkan koordinat awal berupa baris dan kolom dengan rentang 0 hingga 7.
4. Program memvalidasi mode dan koordinat.
5. Kotak awal diberi nilai `0`.
6. Program mengevaluasi delapan kemungkinan gerakan kuda.
7. Setiap gerakan diperiksa berdasarkan batas papan dan status kunjungan.
8. Untuk setiap kandidat valid, program menghitung derajatnya, yaitu jumlah kemungkinan langkah lanjutan.
9. Kandidat diurutkan menggunakan Warnsdorff's Heuristic, dari derajat paling kecil.
10. Jika terdapat derajat yang sama, digunakan jumlah derajat tetangga lanjutan sebagai _tie-breaker_.
11. Program mencoba kandidat pertama menggunakan DFS rekursif.
12. Jika jalur gagal, dilakukan backtracking dengan mengembalikan kotak menjadi `-1`.
13. Proses berulang sampai seluruh 64 kotak dikunjungi atau batas pencarian tercapai.
14. Untuk Open Tour, solusi diterima jika seluruh kotak telah dikunjungi tepat satu kali.
15. Untuk Closed Tour, solusi harus memenuhi syarat tambahan bahwa kotak terakhir terhubung dengan kotak awal.
16. Program menampilkan matriks 8×8 berisi urutan langkah dari `0` hingga `63`.

---

## Implementasi Program

Bahasa pemrograman yang digunakan adalah **C++17**.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
using namespace std;

class KnightsTour {
private:
    static constexpr int N = 8;

    vector<vector<int>> board;

    // Delapan kemungkinan gerakan kuda
    const int dx[8] = {2, 1, -1, -2, -2, -1, 1, 2};
    const int dy[8] = {1, 2, 2, 1, -1, -2, -2, -1};

    int startX, startY;
    int endX, endY;
    bool closed;

    long long calls;
    long long limit;

    struct Candidate {
        int x, y;
        int degree;
        int secondDegree;
    };

    // Periksa apakah koordinat berada di dalam papan
    bool inside(int x, int y) const {
        return x >= 0 && x < N &&
               y >= 0 && y < N;
    }

    // Periksa apakah dua kotak terhubung
    // oleh satu gerakan kuda yang valid
    bool knightMove(int x1, int y1,
                    int x2, int y2) const {
        int a = abs(x1 - x2);
        int b = abs(y1 - y2);

        return (a == 2 && b == 1) ||
               (a == 1 && b == 2);
    }

    // Hitung jumlah langkah lanjutan yang valid
    int getDegree(int x, int y) const {
        int degree = 0;

        for (int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (!inside(nx, ny))
                continue;

            if (board[nx][ny] != -1)
                continue;

            // Sisakan kotak terakhir untuk Closed Tour
            if (closed &&
                nx == endX && ny == endY)
                continue;

            degree++;
        }

        return degree;
    }

    // Tie-breaker ketika degree sama
    int getSecondDegree(int x, int y) const {
        int total = 0;

        for (int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (!inside(nx, ny))
                continue;

            if (board[nx][ny] != -1)
                continue;

            if (closed &&
                nx == endX && ny == endY)
                continue;

            total += getDegree(nx, ny);
        }

        return total;
    }

    // DFS dan Backtracking
    bool dfs(int x, int y, int step) {
        calls++;

        // Batasi jumlah pemanggilan rekursif
        if (calls > limit)
            return false;

        // Semua 64 kotak sudah dikunjungi
        if (step == N * N) {
            if (!closed)
                return true;

            // Closed Tour harus dapat kembali
            // ke kotak awal dengan satu langkah kuda
            return knightMove(
                x, y, startX, startY
            );
        }

        vector<Candidate> candidates;

        // Bangkitkan semua kandidat langkah
        for (int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (!inside(nx, ny))
                continue;

            if (board[nx][ny] != -1)
                continue;

            if (closed) {
                // Jangan kunjungi kotak akhir
                // sebelum langkah terakhir
                if (step < N * N - 1 &&
                    nx == endX && ny == endY)
                    continue;

                // Langkah terakhir harus menuju
                // kotak akhir yang telah ditentukan
                if (step == N * N - 1 &&
                    (nx != endX || ny != endY))
                    continue;
            }

            candidates.push_back({
                nx,
                ny,
                getDegree(nx, ny),
                getSecondDegree(nx, ny)
            });
        }

        // Warnsdorff's Heuristic
        sort(
            candidates.begin(),
            candidates.end(),
            [](const Candidate& a,
               const Candidate& b) {

                if (a.degree != b.degree)
                    return a.degree < b.degree;

                if (a.secondDegree != b.secondDegree)
                    return a.secondDegree < b.secondDegree;

                if (a.x != b.x)
                    return a.x < b.x;

                return a.y < b.y;
            }
        );

        // Coba setiap kandidat secara berurutan
        for (const Candidate& next : candidates) {
            // Tandai kotak sebagai dikunjungi
            board[next.x][next.y] = step;

            // Pencarian rekursif
            if (dfs(next.x, next.y, step + 1))
                return true;

            // Backtracking
            board[next.x][next.y] = -1;
        }

        return false;
    }

    bool solveFrom(int x, int y,
                   bool closedMode) {

        board.assign(
            N, vector<int>(N, -1)
        );

        closed = closedMode;
        startX = x;
        startY = y;

        endX = -1;
        endY = -1;

        calls = 0;

        // Batas pencarian praktis
        limit = closed ? 30000 : 2000000;

        // Tandai posisi awal
        board[x][y] = 0;

        if (!closed)
            return dfs(x, y, 1);

        // Pada Closed Tour, kotak terakhir
        // harus bertetangga dengan kotak awal
        for (int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (!inside(nx, ny))
                continue;

            endX = nx;
            endY = ny;

            if (dfs(x, y, 1))
                return true;

            if (calls > limit)
                return false;
        }

        return false;
    }

public:
    bool solve(int mode, int x, int y) {
        if (mode == 1) {
            return solveFrom(x, y, false);
        }

        // Coba Closed Tour dari posisi pengguna
        if (solveFrom(x, y, true))
            return true;

        // Solusi cadangan:
        // bangun satu cycle dari (0,0)
        if (!solveFrom(0, 0, true))
            return false;

        // Hamiltonian Cycle dapat dimulai
        // dari vertex mana pun pada cycle.
        // Geser nomor langkah agar titik
        // pengguna menjadi langkah ke-0.
        int shift = board[x][y];

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                board[i][j] =
                    (board[i][j] - shift + N * N)
                    % (N * N);
            }
        }

        return true;
    }

    void printBoard() const {
        cout << "\nKnight's Tour Result:\n\n";

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << setw(3) << board[i][j];
            }

            cout << '\n';
        }

        cout << "\nNumbers 0 to 63 represent "
             << "the order of visited squares.\n";
    }
};

int main() {
    cout << "================================\n";
    cout << "         KNIGHT'S TOUR\n";
    cout << "================================\n";

    cout << "1. Open Tour\n";
    cout << "2. Closed Tour\n";

    int mode;

    cout << "Choose mode (1/2): ";

    if (!(cin >> mode) ||
        (mode != 1 && mode != 2)) {
        cout << "Error: Invalid mode!\n";
        return 1;
    }

    int x, y;

    cout << "Enter start row (0-7): ";

    if (!(cin >> x)) {
        cout << "Error: Invalid row input!\n";
        return 1;
    }

    cout << "Enter start column (0-7): ";

    if (!(cin >> y)) {
        cout << "Error: Invalid column input!\n";
        return 1;
    }

    if (x < 0 || x >= 8 ||
        y < 0 || y >= 8) {
        cout << "Error: Coordinates must be "
             << "between 0 and 7!\n";
        return 1;
    }

    KnightsTour solver;

    if (solver.solve(mode, x, y)) {
        cout << "\nTour found successfully!\n";
        solver.printBoard();
    } else {
        cout << "\nNo tour found within "
             << "the search limit.\n";
    }

    return 0;
}
```

### Cara Menjalankan Program

Kompilasi program menggunakan compiler C++ yang mendukung standar C++17.

**Kompilasi:**

```bash
g++ -std=c++17 -O2 knights_tour.cpp -o knights_tour
```

**Menjalankan pada Linux:**

```bash
./knights_tour
```

**Menjalankan pada Windows:**

```powershell
.\knights_tour.exe
```

---

## Penjelasan Program

### 1. Representasi Papan Catur

Papan catur direpresentasikan menggunakan matriks dua dimensi:

```cpp
vector<vector<int>> board;
```

Setiap elemen matriks menyimpan nomor urutan kunjungan kuda.

- `-1`: Kotak belum dikunjungi.
- `0`: Kotak awal.
- `1` sampai `63`: Urutan langkah berikutnya.

Dengan demikian, papan sekaligus berfungsi sebagai representasi keadaan pencarian dan penyimpanan hasil solusi.

### 2. Representasi Gerakan Kuda

Gerakan kuda pada papan catur berbentuk huruf L, yaitu:

- Dua kotak vertikal dan satu kotak horizontal.
- Satu kotak vertikal dan dua kotak horizontal.

Delapan kemungkinan gerakan disimpan dalam array berikut:

```cpp
const int dx[8] = {2, 1, -1, -2, -2, -1, 1, 2};
const int dy[8] = {1, 2, 2, 1, -1, -2, -2, -1};
```

Contohnya, jika kuda berada di koordinat `(3,3)`, salah satu langkah yang mungkin adalah `(5,4)`.

Koordinat tersebut diperoleh dari:

```text
x_baru = 3 + 2 = 5
y_baru = 3 + 1 = 4
```

### 3. Validasi Pergerakan

Fungsi `inside()` digunakan untuk memastikan koordinat tujuan berada di dalam papan.

```cpp
bool inside(int x, int y) const {
    return x >= 0 && x < N &&
           y >= 0 && y < N;
}
```

Pemeriksaan tambahan dilakukan untuk memastikan kotak tujuan belum pernah dikunjungi.

```cpp
if (board[nx][ny] != -1)
    continue;
```

Pemeriksaan ini mencegah kuda mengunjungi kotak yang sama lebih dari satu kali.

### 4. Depth-First Search

DFS merupakan algoritma pencarian yang menelusuri satu jalur sampai kedalaman maksimum sebelum mencoba jalur lain.

Dalam implementasi Knight's Tour, satu pemanggilan DFS merepresentasikan satu posisi kuda pada suatu langkah.

Parameter fungsi:

```cpp
bool dfs(int x, int y, int step)
```

memiliki arti:

- `x`: Baris posisi kuda.
- `y`: Kolom posisi kuda.
- `step`: Nomor langkah yang akan diberikan kepada kotak berikutnya.

Contohnya:

```cpp
dfs(0, 0, 1);
```

menandakan pencarian dimulai pada `(0,0)` dan langkah berikutnya akan diberi nomor `1`.

Setiap kandidat yang dipilih menghasilkan pemanggilan rekursif:

```cpp
dfs(next.x, next.y, step + 1)
```

Proses ini dilanjutkan sampai seluruh kotak berhasil dikunjungi atau jalur menemui jalan buntu.

### 5. Backtracking

Backtracking merupakan mekanisme untuk membatalkan keputusan ketika suatu jalur tidak menghasilkan solusi.

Contohnya, kuda berhasil mengunjungi 60 kotak, tetapi empat kotak yang tersisa tidak dapat dicapai menggunakan langkah legal tanpa mengunjungi ulang kotak sebelumnya.

Dalam keadaan tersebut, jalur dinyatakan gagal.

Program kemudian mengembalikan kotak terakhir menjadi belum dikunjungi:

```cpp
board[next.x][next.y] = -1;
```

Selanjutnya, program mencoba kandidat langkah lain yang sebelumnya belum dieksplorasi.

Dengan backtracking, pencarian tidak berhenti hanya karena satu pilihan menghasilkan jalan buntu.

### 6. Warnsdorff's Heuristic

Warnsdorff's Heuristic digunakan untuk menentukan prioritas langkah berdasarkan banyaknya kemungkinan gerakan lanjutan.

Jumlah gerakan lanjutan disebut **degree**.

Sebagai contoh:

| Kandidat | Degree |
| :------: | :----: |
|    A     |   5    |
|    B     |   2    |
|    C     |   4    |

Algoritma akan memilih kandidat B terlebih dahulu karena memiliki degree terkecil.

Alasan penggunaan heuristik ini adalah kotak dengan sedikit kemungkinan gerakan berisiko menjadi tidak dapat dijangkau apabila dilewati terlalu lama.

Perhitungan derajat dilakukan oleh:

```cpp
int getDegree(int x, int y) const
```

Setelah seluruh kandidat dihitung, kandidat diurutkan menggunakan:

```cpp
sort(candidates.begin(), candidates.end(), ...);
```

Jika beberapa kandidat memiliki degree sama, program memakai `secondDegree` sebagai kriteria tambahan.

Nilai tersebut dihitung dari total derajat petak-petak lanjutan yang masih mungkin dikunjungi.

Pengurutan ini membantu mengurangi eksplorasi cabang yang kurang menjanjikan. Namun, Warnsdorff tidak menjamin bahwa langkah pertama yang dipilih selalu menghasilkan solusi sehingga backtracking tetap diperlukan.

### 7. Implementasi Open Tour

Open Tour berkaitan dengan pencarian Hamiltonian Path pada graf.

Syarat keberhasilannya adalah:

1. Semua vertex dikunjungi.
2. Setiap vertex dikunjungi tepat satu kali.
3. Perpindahan antarvertex mengikuti edge yang valid.

Ketika `step == 64`, seluruh petak sudah diberi nomor 0 sampai 63.

Untuk Open Tour, kondisi tersebut cukup untuk menyatakan solusi berhasil:

```cpp
if (step == N * N) {
    if (!closed)
        return true;
}
```

Tidak ada persyaratan agar posisi terakhir dapat kembali ke posisi awal.

### 8. Implementasi Closed Tour

Closed Tour berkaitan dengan pencarian Hamiltonian Cycle.

Selain memenuhi syarat Open Tour, posisi terakhir harus terhubung dengan posisi awal melalui gerakan kuda yang sah.

Pemeriksaan dilakukan menggunakan:

```cpp
return knightMove(x, y, startX, startY);
```

Fungsi `knightMove()` memeriksa selisih koordinat kedua posisi.

Perjalanan termasuk Closed Tour apabila selisih tersebut adalah `(2,1)` atau `(1,2)` dalam nilai absolut.

Program mengoptimalkan pencarian Closed Tour dengan menentukan salah satu tetangga posisi awal sebagai kandidat petak terakhir.

Petak tersebut disisihkan selama pencarian dan baru boleh dikunjungi pada langkah ke-63.

Apabila pencarian langsung dari koordinat pengguna melewati batas yang ditentukan, program mencoba membangun Hamiltonian Cycle dari `(0,0)`.

Setelah cycle ditemukan, penomoran langkah dapat diputar sehingga kotak yang dipilih pengguna menjadi langkah ke-0.

Proses tersebut valid karena sebuah siklus tidak memiliki titik awal yang tetap.

### 9. Kondisi Berhenti

Program memiliki dua jenis kondisi berhenti.

**Berhasil:**

```cpp
if (step == N * N)
```

menandakan seluruh kotak telah dikunjungi.

Untuk Closed Tour, masih dilakukan pemeriksaan koneksi menuju titik awal.

**Batas eksplorasi:**

```cpp
if (calls > limit)
    return false;
```

digunakan untuk mencegah pencarian berlangsung terlalu lama.

Perlu diperhatikan bahwa pencarian yang gagal karena batas eksplorasi tidak membuktikan bahwa solusi tidak ada. Hal tersebut hanya menunjukkan bahwa pencarian tidak menemukan solusi dalam batas yang ditentukan.

---

## Hasil Output Program

Bagian berikut menampilkan contoh hasil yang telah diperiksa melalui simulasi logika algoritma yang ekuivalen dengan implementasi.

### Pengujian 1: Open Tour

**Tujuan:** Mengetahui apakah kuda mampu mengunjungi seluruh 64 kotak tepat satu kali tanpa harus kembali ke posisi awal.

**Input:**

```text
Choose mode (1/2): 1
Enter start row (0-7): 0
Enter start column (0-7): 0
```

**Contoh Output:**

```text
Tour found successfully!

Knight's Tour Result:

  0  3 38 19 22  5 24 27
 37 18  1  4 39 26 21  6
  2 53 36 49 20 23 28 25
 17 42 51 54 35 40  7 60
 52 13 48 41 50 59 34 29
 43 16 55 58 47 32 61  8
 12 57 14 45 10 63 30 33
 15 44 11 56 31 46  9 62

Numbers 0 to 63 represent
the order of visited squares.
```

**Hasil yang diperoleh:**

Program menghasilkan matriks 8×8 yang berisi seluruh nomor langkah dari 0 hingga 63.

Setiap nomor muncul tepat satu kali, sehingga tidak terdapat kotak yang dikunjungi ulang.

Seluruh pasangan langkah berurutan juga memenuhi aturan gerakan kuda.

Karena seluruh vertex berhasil dikunjungi, solusi memenuhi karakteristik Hamiltonian Path.

Posisi terakhir tidak diwajibkan terhubung kembali dengan posisi awal.

Dengan demikian, pengujian Open Tour dinyatakan berhasil.

### Pengujian 2: Closed Tour

**Tujuan:** Memastikan seluruh kotak dikunjungi tepat satu kali dan posisi terakhir dapat terhubung kembali dengan posisi awal.

**Input:**

```text
Choose mode (1/2): 2
Enter start row (0-7): 0
Enter start column (0-7): 0
```

**Contoh Output:**

```text
Tour found successfully!

Knight's Tour Result:

  0  3 38 19 22  5 24 27
 37 18  1  4 39 26 21  6
  2 63 36 57 20 23 28 25
 17 60 47 40 35 58  7 42
 48 13 62 59 56 41 34 29
 61 16 55 46 51 32 43  8
 12 49 14 53 10 45 30 33
 15 54 11 50 31 52  9 44

Numbers 0 to 63 represent
the order of visited squares.
```

**Hasil yang diperoleh:**

Program berhasil menemukan rute yang mengunjungi seluruh 64 kotak tepat satu kali.

Posisi awal berada di `(0,0)` dengan nomor langkah `0`.

Posisi terakhir berada di `(2,1)` dengan nomor langkah `63`.

Selisih koordinatnya adalah:

```text
|2 - 0| = 2
|1 - 0| = 1
```

Selisih tersebut memenuhi aturan gerakan kuda.

Dengan demikian, kuda dapat kembali ke posisi awal menggunakan satu gerakan tambahan, sehingga urutan perjalanan membentuk Hamiltonian Cycle.

Pengujian Closed Tour dinyatakan berhasil.

### Pengujian 3: Input Tidak Valid

**Tujuan:** Memastikan program menolak koordinat di luar papan.

**Input:**

```text
Choose mode (1/2): 1
Enter start row (0-7): 8
Enter start column (0-7): 0
```

**Output:**

```text
Error: Coordinates must be between 0 and 7!
```

**Hasil yang diperoleh:**

Program menolak input karena koordinat baris `8` berada di luar rentang yang diperbolehkan.

Indeks yang valid adalah 0 sampai 7 untuk baris maupun kolom.

Validasi ini mencegah akses terhadap elemen array di luar batasnya.

### Pengujian 4: Mode Tidak Valid

**Input:**

```text
Choose mode (1/2): 3
```

**Output:**

```text
Error: Invalid mode!
```

**Hasil yang diperoleh:**

Program menolak pilihan mode selain 1 dan 2 sehingga pengguna tidak dapat menjalankan mode yang tidak tersedia.

---

## Analisis Kompleksitas

Misalkan ukuran papan adalah N×N, sehingga terdapat N² vertex.

### Kompleksitas Waktu

**1. Inisialisasi papan**

Seluruh N² elemen papan diinisialisasi menggunakan nilai `-1`.

Kompleksitas:

```text
O(N²)
```

**2. Pemeriksaan gerakan kuda**

Setiap petak memiliki paling banyak delapan kemungkinan gerakan kuda.

Karena jumlah ini konstan, pengecekan satu posisi memiliki kompleksitas:

```text
O(1)
```

**3. Perhitungan degree**

Fungsi `getDegree()` memeriksa paling banyak delapan tetangga.

Kompleksitas:

```text
O(1)
```

**4. DFS dan Backtracking**

Pada kondisi terburuk, DFS dapat mengeksplorasi sangat banyak kombinasi urutan kunjungan.

Setiap posisi memiliki paling banyak delapan kandidat gerakan dan kedalaman pencarian maksimum mencapai N².

Batas atas kasar kompleksitas waktu adalah:

```text
O(8^(N²))
```

Kompleksitas ini menunjukkan pertumbuhan eksponensial terhadap jumlah petak.

Walaupun demikian, batas tersebut sangat longgar karena gerakan tidak boleh mengunjungi kembali petak yang sudah dikunjungi.

Warnsdorff's Heuristic mengurangi pencarian yang diperlukan pada banyak kasus, tetapi tidak menghilangkan kemungkinan kompleksitas eksponensial.

### Kompleksitas Ruang

Komponen utama penggunaan memori:

1. Matriks papan berukuran N×N.
2. Stack rekursi DFS dengan kedalaman maksimum N².
3. Daftar kandidat gerakan dengan jumlah maksimum delapan per pemanggilan.

Dengan demikian, kompleksitas ruang keseluruhan adalah:

```text
O(N²)
```

Untuk papan 8×8, penggunaan memori relatif kecil karena hanya terdapat 64 kotak.

---

## Kesimpulan

Pada Praktikum 2, permasalahan The Knight's Tour berhasil direpresentasikan menggunakan konsep graf, dengan setiap kotak papan catur sebagai vertex dan hubungan gerakan kuda sebagai edge.

Pendekatan **Depth-First Search (DFS)** digunakan untuk menelusuri kemungkinan jalur, sedangkan **Backtracking** memungkinkan algoritma kembali ke percabangan sebelumnya apabila jalur yang dipilih tidak dapat menghasilkan solusi.

Penggunaan **Warnsdorff's Heuristic** membantu mengoptimalkan proses pencarian dengan memprioritaskan kotak yang memiliki kemungkinan langkah lanjutan paling sedikit.

Program mendukung dua mode perjalanan:

1. **Open Tour**, yang menghasilkan Hamiltonian Path melalui seluruh kotak papan catur.
2. **Closed Tour**, yang menghasilkan Hamiltonian Cycle dengan memastikan posisi terakhir dapat terhubung kembali ke posisi awal.

Berdasarkan simulasi algoritma, kedua mode dapat menghasilkan urutan kunjungan 64 kotak yang valid. Program juga dilengkapi validasi input untuk mencegah penggunaan koordinat maupun mode yang tidak sesuai.

Dari praktikum ini dapat disimpulkan bahwa kombinasi DFS, Backtracking, dan heuristik merupakan pendekatan yang efektif secara praktis untuk menyelesaikan permasalahan pencarian lintasan dalam graf, khususnya Knight's Tour pada papan catur 8×8.

---

**Catatan:** Matriks pengujian telah diperiksa menggunakan simulasi algoritma yang ekuivalen. Hasil tersebut belum berasal dari eksekusi langsung berkas C++ melalui compiler, sehingga pengujian kompilasi dan dokumentasi terminal tetap perlu dilakukan sebelum pengumpulan praktikum.
