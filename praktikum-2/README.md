# Praktikum 2 - Mengimplementasikan Program untuk Menyelesaikan Permasalahan "The Knight's Tour"

|    NRP     |           Nama           |
| :--------: |       :------------:       |
| 5025251055 |   Aga Nafta Filadelfiano   |
| 5025251067 |     Azka Fairus Syamsa     |
| 5025251106 |   Asher Yedijah Hoesono    |

## The Knight's Tour

Permasalahan **The Knight's Tour** menuntut sebuah bidak kuda untuk melakukan perjalanan mengunjungi semua kotak pada papan catur tepat satu kali. Pada praktikum ini, permasalahan tersebut direpresentasikan sebagai pencarian dalam graf. Program ini mendukung dua mode perjalanan:
1. **Open Tour**: Kuda mengakhiri perjalanan di sebarang kotak, dimana merupakan bentuk pencarian *Hamiltonian Path* pada graf.
2. **Closed Tour**: Kuda mengakhiri perjalanan pada *attacking square* , dimana merupakan bentuk pencarian *Hamiltonian Cycle* pada graf.

Pada implementasi kali ini, digunakan pendekatan **Depth-First Search (DFS) / Backtracking** yang dioptimasi dengan **Warnsdorff's Heuristic**. Algoritma ini memprioritaskan langkah kuda menuju kotak yang memiliki opsi jumlah langkah lanjutan (derajat) paling sedikit, sehingga komputasi berjalan jauh lebih efisien.

## Algoritma
1. Program menginisialisasi ukuran papan catur (misal: 8x8) dengan nilai `-1` yang menandakan kotak belum dikunjungi.
2. Pengguna memasukkan pilihan mode permainan (1 untuk *Open Tour*, 2 untuk *Closed Tour*).
3. Pengguna memasukkan koordinat awal kuda (baris `X` dan kolom `Y`).
4. Program menandai posisi awal kuda dengan langkah ke-`0`.
5. Program mengevaluasi seluruh 8 kemungkinan langkah kuda (berbentuk L) dari posisi saat ini.
6. Untuk setiap langkah yang valid (berada di dalam papan dan belum dikunjungi), program menghitung derajatnya (jumlah langkah valid selanjutnya).
7. Menggunakan **Warnsdorff's Heuristic**, opsi-opsi langkah tersebut diurutkan dari derajat terkecil hingga terbesar.
8. Program melakukan *Backtracking* pada opsi yang telah diurutkan.
9. Jika kuda berhasil mencapai langkah ke-64, program mengecek kondisi akhir (bebas untuk *Open Tour*, atau harus terhubung dengan start untuk *Closed Tour*).
10. Program menampilkan matriks urutan langkah pergerakan kuda dari angka `0` hingga `63`.

## Implementasi Program

```cpp
//masukkan kode disini
```

---

## Penjelasan Program


## Hasil Output Program

### Pengujian 1: Open Tour

**Input:**


**Output:**


**Hasil yang diperoleh:**


### Pengujian 2: Closed Tour

**Input:**

**Output:**

**Hasil yang diperoleh:**


### Pengujian 3: Input Tidak Valid

**Input:**

**Output:**

## Kesimpulan



