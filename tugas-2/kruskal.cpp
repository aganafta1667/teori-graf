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

    for (long i=1; i<=vertexCount; i++)
        parent[i] = i;

    sort(edges.begin(), edges.end(), [](Edge a, Edge b) {
        return a.weight < b.weight;
    });

    for (Edge current : edges) {
        if (connect(parent, current.from, current.to)) {
            mst.push_back(current);

            if ((long)mst.size() == vertexCount - 1) break;
        }
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

    cout << "\nHASIL ALGORITMA KRUSKAL\n";

    for (Edge current : mst) {
        cout << "v" << current.from << " - v" << current.to << " : " << current.weight << endl;
        totalWeight += current.weight;
    }

    cout << "bobot spanning tree terkecil: " << totalWeight << endl;

    return 0;
}