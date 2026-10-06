#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>

using namespace std;

const int INF = 1e9;

int dfsCount(int simpul, vector<bool>& dikunjungi, vector<vector<int>>& edge_count, int n) {
    dikunjungi[simpul] = true;
    int jumlah = 1;

    for (int i = 0; i < n; i++) {
        if (edge_count[simpul][i] > 0 && !dikunjungi[i]) {
            jumlah += dfsCount(i, dikunjungi, edge_count, n);
        }
    }

    return jumlah;
}

bool isValidNextEdge(int u, int v, vector<vector<int>>& edge_count, int n) {
    int jumlah = 0;

    for (int i = 0; i < n; i++) {
        if (edge_count[u][i] > 0) jumlah++;
    }
    if (jumlah == 1) return true;

    vector<bool> dikunjungi1(n, false);
    int jumlah1 = dfsCount(u, dikunjungi1, edge_count, n);

    edge_count[u][v]--;
    edge_count[v][u]--;

    vector<bool> dikunjungi2(n, false);
    int jumlah2 = dfsCount(u, dikunjungi2, edge_count, n);

    edge_count[u][v]++;
    edge_count[v][u]++;

    return jumlah1 <= jumlah2;
}

void runFleury(int u, vector<vector<int>>& edge_count, int n, vector<char>& simpul, vector<char>& jalur) {
    for (int v = 0; v < n; v++) {
        if (edge_count[u][v] > 0 && isValidNextEdge(u, v, edge_count, n)) {
            jalur.push_back(simpul[v]);
            edge_count[u][v]--;
            edge_count[v][u]--;
            runFleury(v, edge_count, n, simpul, jalur);
            return;
        }
    }
}

int main() {
    int n = 6;

    vector<char> simpul = {'u', 'v', 'w', 'x', 'y', 'z'};
    vector<vector<int>> weight(n, vector<int>(n, INF));
    vector<vector<int>> edge_count(n, vector<int>(n, 0));

    auto addEdge = [&](int u, int v, int bobot) {
        weight[u][v] = bobot;
        weight[v][u] = bobot;
        edge_count[u][v] = 1;
        edge_count[v][u] = 1;
    };

    addEdge(0, 3, 1);
    addEdge(0, 4, 4);
    addEdge(0, 2, 5);
    addEdge(3, 4, 2);
    addEdge(4, 2, 1);
    addEdge(3, 5, 3);
    addEdge(4, 5, 2);
    addEdge(5, 2, 2);
    addEdge(3, 1, 6);
    addEdge(5, 1, 3);
    addEdge(2, 1, 2);

    vector<int> simpul_ganjil;

    for (int i = 0; i < n; i++) {
        int derajat = 0;

        for (int j = 0; j < n; j++) {
            derajat += edge_count[i][j];
        }

        if (derajat % 2 != 0) {
            simpul_ganjil.push_back(i);
        }
    }

    cout << "Simpul berderajat ganjil: ";

    for (int simpul_sekarang : simpul_ganjil) {
        cout << simpul[simpul_sekarang] << " ";
    }

    cout << "\n\n";

    if (simpul_ganjil.size() == 2) {
        int mulai = simpul_ganjil[0];
        int tujuan = simpul_ganjil[1];

        vector<int> dist(n, INF);
        vector<int> parent(n, -1);

        priority_queue<pair<int, int>,vector<pair<int, int>>,greater<pair<int, int>>> pq;

        dist[mulai] = 0;
        pq.push({0, mulai});

        while (!pq.empty()) {
            int jarak = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (jarak > dist[u]) {
                continue;
            }

            for (int v = 0; v < n; v++) {
                if (weight[u][v] != INF) {
                    if (dist[u] + weight[u][v] < dist[v]) {
                        dist[v] = dist[u] + weight[u][v];
                        parent[v] = u;
                        pq.push({dist[v], v});
                    }
                }
            }
        }

        cout << "Bobot jalur terpendek ("
             << simpul[mulai] << " ke " << simpul[tujuan]
             << "): " << dist[tujuan] << "\n";

        int sekarang = tujuan;
        vector<int> shortest_path;

        while (sekarang != -1) {
            shortest_path.push_back(sekarang);
            sekarang = parent[sekarang];
        }

        reverse(shortest_path.begin(), shortest_path.end());

        cout << "Jalur yang digandakan: ";

        for (size_t i = 0; i < shortest_path.size() - 1; i++) {
            int u = shortest_path[i];
            int v = shortest_path[i + 1];

            edge_count[u][v]++;
            edge_count[v][u]++;

            cout << simpul[u] << "-" << simpul[v] << " ";
        }

        cout << "\n\n";
    }

    cout << "Hasil Minimum-Weight Tour (Sirkuit Euler):\n";

    vector<char> jalur;
    int simpul_mulai = 0;

    jalur.push_back(simpul[simpul_mulai]);

    runFleury(simpul_mulai, edge_count, n, simpul, jalur);

    int total_bobot = 0;

    for (size_t i = 0; i < jalur.size(); i++) {
        cout << jalur[i];

        if (i != jalur.size() - 1) {
            cout << " -> ";

            int u = -1;
            int v = -1;

            for (int j = 0; j < n; j++) {
                if (simpul[j] == jalur[i]) u = j;
                if (simpul[j] == jalur[i + 1]) v = j;
            }
            total_bobot += weight[u][v];
        }
    }
    cout << endl;
    cout << "Total bobot: " << total_bobot << endl;

    return 0;
}