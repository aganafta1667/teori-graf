#include <bits/stdc++.h>
using namespace std;

struct Edge {
    long from, to, weight;
};

long vertexCount, edgeCount;
vector<Edge> edges;

long getRoot(vector<long>& parent, long vertex) {
    if (parent[vertex] != vertex)
        parent[vertex] = getRoot(parent, parent[vertex]);
    return parent[vertex];
}

bool connect(vector<long>& parent, long a, long b) {
    long rootA = getRoot(parent, a);
    long rootB = getRoot(parent, b);

    if (rootA == rootB) return false;

    parent[rootA] = rootB;
    return true;
}

vector<Edge> buildMST() {
    vector<long> parent(vertexCount + 1);
    vector<Edge> mst;

    for (long i = 1; i <= vertexCount; i++)
        parent[i] = i;

    long numComponents = vertexCount;

    // Ulangi selama masih ada lebih dari satu komponen (fragmen pohon) yang terpisah
    while (numComponents > 1) {
        // cheapest[akar] = indeks sisi termurah yang keluar dari komponen tsb (-1 = belum ada)
        vector<long> cheapest(vertexCount + 1, -1);

        // Fase 1: tiap komponen mencari sisi termurah yang menghubungkannya ke komponen lain
        for (long i = 0; i < (long)edges.size(); i++) {
            long rootFrom = getRoot(parent, edges[i].from);
            long rootTo   = getRoot(parent, edges[i].to);

            if (rootFrom == rootTo) continue; // sisi di dalam komponen yang sama, lewati

            if (cheapest[rootFrom] == -1 || edges[cheapest[rootFrom]].weight > edges[i].weight)
                cheapest[rootFrom] = i;

            if (cheapest[rootTo] == -1 || edges[cheapest[rootTo]].weight > edges[i].weight)
                cheapest[rootTo] = i;
        }

        // Fase 2: gabungkan tiap komponen dengan sisi termurah miliknya
        bool adaPenggabungan = false;
        for (long node = 1; node <= vertexCount; node++) {
            long e = cheapest[node];
            if (e == -1) continue; // bukan akar komponen, atau komponen sudah tidak punya sisi keluar

            if (connect(parent, edges[e].from, edges[e].to)) {
                mst.push_back(edges[e]);
                numComponents--;
                adaPenggabungan = true;
            }
        }

        if (!adaPenggabungan) break; // graf tidak terhubung, MST tidak bisa dilengkapi
    }
    return mst;
}

int main() {
    cin >> vertexCount >> edgeCount;

    for (long i = 0; i < edgeCount; i++) {
        Edge input;

        cin >> input.from >> input.to >> input.weight;
        edges.push_back(input);
    }

    vector<Edge> mst = buildMST();

    long totalWeight = 0;

    cout << "\nHASIL ALGORITMA BORUVKA\n";

    for (Edge current : mst) {
        cout << "v" << current.from << " - v" << current.to << " : " << current.weight << endl;
        totalWeight += current.weight;
    }

    cout << "bobot spanning tree terkecil: " << totalWeight << endl;

    return 0;
}