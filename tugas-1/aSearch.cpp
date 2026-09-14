#include <bits/stdc++.h>
using namespace std;

const int N = 11;                 // jumlah vertex: V1..V11
const double INF = 1e18;

vector<string> nama = {"V1","V2","V3","V4","V5","V6","V7","V8","V9","V10","V11"};

// Perkiraan koordinat (x, y) tiap vertex, dibaca dari tata letak diagram pada soal.
// Layer kolom: V1(kiri) ... V11(kanan). Baris atas/tengah/bawah -> sumbu y.
vector<pair<double,double>> koordinat = {
    {0,1},  // V1
    {1,2},  // V2
    {1,1},  // V3
    {1,0},  // V4
    {2,2},  // V5
    {2,1},  // V6
    {2,0},  // V7
    {3,2},  // V8
    {3,1},  // V9
    {3,0},  // V10
    {4,1},  // V11
};

int idx(const string &v) {
    for (int i = 0; i < N; i++) if (nama[i] == v) return i;
    return -1;
}

double heuristic(int v, int goal) {
    double dx = koordinat[v].first  - koordinat[goal].first;
    double dy = koordinat[v].second - koordinat[goal].second;
    return sqrt(dx * dx + dy * dy);
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

    vector<vector<pair<int,int>>> adj(N); // adj[u] = { (v, bobot), ... }
    for (size_t e = 0; e < pasangan.size(); e++) {
        int u = idx(pasangan[e][0]);
        int v = idx(pasangan[e][1]);
        int w = bobot[e];
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    int start = idx("V1");
    int goal  = idx("V11");

    // ---------- 2. Algoritma A* ----------
    vector<double> g(N, INF);          // g(n) = jarak riil dari start
    vector<double> f(N, INF);          // f(n) = g(n) + h(n)
    vector<int>    predecessor(N, -1);
    vector<bool>   closed(N, false);

    // priority queue berisi (f(n), vertex), diurutkan dari f terkecil
    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> open;

    g[start] = 0;
    f[start] = heuristic(start, goal);
    open.push({f[start], start});

    cout << "===== Urutan Ekspansi Vertex (A*) =====\n";
    cout << left << setw(6) << "No" << setw(10) << "Vertex"
         << setw(10) << "g(n)" << setw(10) << "h(n)" << setw(10) << "f(n)" << "\n";

    int urutan = 1;
    while (!open.empty()) {
        auto [fCur, u] = open.top();
        open.pop();

        if (closed[u]) continue;   // entry basi (sudah pernah diperbarui), lewati
        closed[u] = true;

        cout << left << setw(6) << urutan++ << setw(10) << nama[u]
             << setw(10) << g[u] << setw(10) << heuristic(u, goal)
             << setw(10) << f[u] << "\n";

        if (u == goal) break;      // tujuan ditemukan & sudah "diambil" dari open set

        for (auto &[v, w] : adj[u]) {
            if (closed[v]) continue;
            double tentativeG = g[u] + w;
            if (tentativeG < g[v]) {
                g[v] = tentativeG;
                f[v] = tentativeG + heuristic(v, goal);
                predecessor[v] = u;
                open.push({f[v], v});
            }
        }
    }

    // ---------- 3. Rekonstruksi & cetak jalur ----------
    cout << "\n===== Jalur Terpendek V1 -> V11 (A*) =====\n";
    if (g[goal] >= INF) {
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
    cout << "\nTotal bobot = " << g[goal] << "\n";

    return 0;
}