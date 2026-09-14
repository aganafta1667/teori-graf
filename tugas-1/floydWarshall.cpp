#include <bits/stdc++.h>
using namespace std;

const int N = 11;                 // jumlah vertex: V1..V11
const long long INF = 1e15;

// Nama vertex, index 0 = V1, index 10 = V11
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

    vector<vector<long long>> dist(N, vector<long long>(N, INF));
    vector<vector<int>>        next_(N, vector<int>(N, -1));

    for (int i = 0; i < N; i++) dist[i][i] = 0;

    for (size_t e = 0; e < pasangan.size(); e++) {
        int u = idx(pasangan[e][0]);
        int v = idx(pasangan[e][1]);
        int w = bobot[e];
        dist[u][v] = w;  dist[v][u] = w;
        next_[u][v] = v; next_[v][u] = u;
    }

    // ---------- 2. Algoritma Floyd-Warshall ----------
    // D[i][j] = min( D[i][j], D[i][k] + D[k][j] )  untuk setiap k
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            if (dist[i][k] == INF) continue;
            for (int j = 0; j < N; j++) {
                if (dist[k][j] == INF) continue;
                long long lewatK = dist[i][k] + dist[k][j];
                if (lewatK < dist[i][j]) {
                    dist[i][j] = lewatK;
                    next_[i][j] = next_[i][k];
                }
            }
        }
    }

    // ---------- 3. Cetak matriks jarak akhir (all-pairs) ----------
    cout << "===== Matriks Jarak Terpendek Antar Semua Pasangan Vertex =====\n\n";
    cout << setw(6) << " ";
    for (auto &v : nama) cout << setw(6) << v;
    cout << "\n";
    for (int i = 0; i < N; i++) {
        cout << setw(6) << nama[i];
        for (int j = 0; j < N; j++) {
            if (dist[i][j] >= INF) cout << setw(6) << "INF";
            else cout << setw(6) << dist[i][j];
        }
        cout << "\n";
    }

    // ---------- 4. Rekonstruksi & cetak jalur khusus V1 -> V11 ----------
    int src = idx("V1"), dst = idx("V11");
    cout << "\n===== Jalur Terpendek V1 -> V11 =====\n";

    if (dist[src][dst] >= INF || next_[src][dst] == -1) {
        cout << "Tidak ada jalur dari V1 ke V11.\n";
        return 0;
    }

    vector<int> path = {src};
    int cur = src;
    while (cur != dst) {
        cur = next_[cur][dst];
        path.push_back(cur);
    }

    for (size_t i = 0; i < path.size(); i++) {
        cout << nama[path[i]];
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << "\nTotal bobot = " << dist[src][dst] << "\n";

    return 0;
}