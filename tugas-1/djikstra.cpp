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

    // ---------- 2. Algoritma Dijkstra ----------
    vector<double> dist(N, INF);       // dist(n) = jarak terpendek sementara dari start
    vector<int>    predecessor(N, -1);
    vector<bool>   visited(N, false);  // vertex yang sudah "difinalisasi"

    // priority queue berisi (dist(n), vertex), diurutkan dari dist terkecil
    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;

    dist[start] = 0;
    pq.push({0, start});

    cout << "===== Urutan Vertex Difinalisasi (Dijkstra) =====\n";
    cout << left << setw(6) << "No" << setw(10) << "Vertex" << setw(12) << "dist(n)" << "\n";

    int urutan = 1;
    while (!pq.empty()) {
        auto [dCur, u] = pq.top();
        pq.pop();

        if (visited[u]) continue;   // entry basi (sudah pernah diperbarui), lewati
        visited[u] = true;

        cout << left << setw(6) << urutan++ << setw(10) << nama[u] << setw(12) << dist[u] << "\n";

        if (u == goal) break;       // tujuan sudah difinalisasi, boleh berhenti

        for (auto &[v, w] : adj[u]) {
            if (visited[v]) continue;
            double baru = dist[u] + w;
            if (baru < dist[v]) {
                dist[v] = baru;
                predecessor[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // ---------- 3. Rekonstruksi & cetak jalur ----------
    cout << "\n===== Jalur Terpendek V1 -> V11 (Dijkstra) =====\n";
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