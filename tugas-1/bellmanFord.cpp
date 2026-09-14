#include <bits/stdc++.h>
using namespace std;

const int N = 11;                 // jumlah vertex: V1..V11
const double INF = 1e18;

vector<string> nama = {"V1","V2","V3","V4","V5","V6","V7","V8","V9","V10","V11"};

int idx(const string &v) {
    for (int i = 0; i < N; i++) if (nama[i] == v) return i;
    return -1;
}

int main() {
    // ---------- 1. Bangun graf (undirected, weighted) sesuai gambar di soal ----------
    vector<array<string,2>> pasangan = {
        {"V1","V2"}, {"V1","V3"}, {"V1","V4"},
        {"V2","V3"}, {"V2","V5"},
        {"V3","V4"}, {"V3","V5"}, {"V3","V6"}, {"V3","V7"},
        {"V4","V7"},
        {"V5","V6"}, {"V5","V8"}, {"V5","V9"},
        {"V6","V7"}, {"V6","V9"},
        {"V7","V9"}, {"V7","V10"},
        {"V8","V9"}, {"V8","V11"},
        {"V9","V10"}, {"V9","V11"},
        {"V10","V11"}
    };
    vector<int> bobot = {
        2, 8, 1,
        6, 1,
        7, 5, 1, 2,
        9,
        3, 2, 9,
        4, 6,
        3, 1,
        7, 9,
        1, 2,
        4
    };

    // Bellman-Ford bekerja pada daftar sisi. Karena graf tidak berarah,
    // setiap sisi {u,v,w} dimasukkan dua arah: u->v dan v->u.
    struct Edge { int u, v, w; };
    vector<Edge> edges;
    for (size_t e = 0; e < pasangan.size(); e++) {
        int u = idx(pasangan[e][0]);
        int v = idx(pasangan[e][1]);
        int w = bobot[e];
        edges.push_back({u, v, w});
        edges.push_back({v, u, w});
    }

    int start = idx("V1");
    int goal  = idx("V11");

    // ---------- 2. Algoritma Bellman-Ford ----------
    vector<double> dist(N, INF);       // dist(n) = jarak terpendek dari start
    vector<int>    predecessor(N, -1);
    dist[start] = 0;

    cout << "===== Proses Relaksasi Bellman-Ford =====\n";
    cout << "(hanya vertex yang mengalami update pada tiap iterasi yang ditampilkan)\n\n";

    // Relaksasi sisi sebanyak (jumlah vertex - 1) kali
    for (int iter = 1; iter <= N - 1; iter++) {
        bool adaUpdate = false;
        cout << "Iterasi ke-" << iter << ": ";

        for (auto &e : edges) {
            if (dist[e.u] < INF && dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                predecessor[e.v] = e.u;
                adaUpdate = true;
                cout << nama[e.v] << "=" << dist[e.v] << " ";
            }
        }

        if (!adaUpdate) {
            cout << "tidak ada perubahan -> berhenti lebih awal.\n";
            break;
        }
        cout << "\n";
    }

    // Pengecekan siklus berbobot negatif (di kasus ini semua bobot positif,
    // jadi seharusnya tidak pernah terdeteksi)
    bool adaSiklusNegatif = false;
    for (auto &e : edges) {
        if (dist[e.u] < INF && dist[e.u] + e.w < dist[e.v]) {
            adaSiklusNegatif = true;
            break;
        }
    }
    if (adaSiklusNegatif) {
        cout << "\nGraf mengandung siklus berbobot negatif, solusi tidak valid!\n";
        return 0;
    }

    cout << "\n===== Jarak Akhir dari V1 ke Semua Vertex =====\n";
    cout << left << setw(10) << "Vertex" << setw(12) << "dist(n)" << "\n";
    for (int i = 0; i < N; i++) {
        cout << left << setw(10) << nama[i] << setw(12) << dist[i] << "\n";
    }

    // ---------- 3. Rekonstruksi & cetak jalur ----------
    cout << "\n===== Jalur Terpendek V1 -> V11 (Bellman-Ford) =====\n";
    if (dist[goal] >= INF) {
        cout << "Tidak ada jalur dari V1 ke V11.\n";
        return 0;
    }

    vector<int> path;
    for (int cur = goal; cur != -1; cur = predecessor[cur]) path.push_back(cur);
    reverse(path.begin(), path.end());

    for (size_t i = 0; i < path.size(); i++) {
        cout << nama[path[i]];
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << "\nTotal bobot = " << dist[goal] << "\n";

    return 0;
}