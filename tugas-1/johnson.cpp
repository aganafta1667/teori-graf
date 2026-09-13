#include <bits/stdc++.h>
using namespace std;

const long long INF = LLONG_MAX / 4;

struct Edge {
    int vertex;
    int nextVertex;
    long long weight;
};

bool bellmanFord(int vertexCount, const vector<Edge>& edges, vector<long long>& h) {
    h.assign(vertexCount + 1, 0);
    for (int i = 0; i < vertexCount; i++) {
        bool changed = false;

        for (const Edge& edge : edges) {
            if (h[edge.vertex] + edge.weight < h[edge.nextVertex]) {
                h[edge.nextVertex] =
                    h[edge.vertex] + edge.weight;
                changed = true;
            }
        }

        if (!changed) {
            break;
        }
    }

    for (const Edge& edge : edges) {
        if (h[edge.vertex] + edge.weight < h[edge.nextVertex]) {
            return false;
        }
    }

    return true;
}

void dijkstra(int startVertex, int targetVertex, int vertexCount, const vector<vector<pair<int, long long>>>& graph, const vector<long long>& h) {
    priority_queue< pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>> > pq;

    vector<long long> distance(vertexCount + 1, INF);
    vector<int> previous(vertexCount + 1, -1);

    distance[startVertex] = 0;
    pq.push({0, startVertex});

    while (!pq.empty()) {
        long long currentDistance = pq.top().first;
        int currentVertex = pq.top().second;
        pq.pop();

        if (currentDistance != distance[currentVertex]) {
            continue;
        }

        for (const auto& neighbor : graph[currentVertex]) {
            int nextVertex = neighbor.first;
            long long weight = neighbor.second;

            if (distance[currentVertex] + weight < distance[nextVertex]) {
                distance[nextVertex] = distance[currentVertex] + weight;
                previous[nextVertex] = currentVertex;

                pq.push({
                    distance[nextVertex],
                    nextVertex
                });
            }
        }
    }

    cout << endl;
    cout << "Hasil Algoritma Johnson" << endl;

    if (distance[targetVertex] == INF) {
        cout << "Tidak ada rute dari v" << startVertex << " ke v" << targetVertex << endl;
        return;
    }

    long long originalDistance = distance[targetVertex] - h[startVertex] + h[targetVertex];

    cout << "Jarak terpendek v" << startVertex << " ke v" << targetVertex << " = " << originalDistance << endl;

    vector<int> path;

    int currentVertex = targetVertex;

    while (currentVertex != -1) {
        path.push_back(currentVertex);
        currentVertex = previous[currentVertex];
    }

    reverse(path.begin(), path.end());

    cout << "Rute yang ditempuh: ";

    for (int i = 0; i < path.size(); i++) {
        cout << "v" << path[i];

        if (i != path.size() - 1) {
            cout << " -> ";
        }
    }

    cout << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int vertexCount, edgeCount;

    cout << "Jumlah vertex dan edge: " << endl;
    cin >> vertexCount >> edgeCount;

    vector<Edge> originalEdges;
    vector<Edge> bellmanEdges;

    for (int vertex = 1; vertex <= vertexCount; vertex++) {
        bellmanEdges.push_back({0, vertex, 0});
    }

    cout << "Masukkan setiap edge (vertex1 vertex2 weight):" << endl;

    for (int i = 0; i < edgeCount; i++) {
        int vertex1, vertex2;
        long long weight;

        cin >> vertex1 >> vertex2 >> weight;

        originalEdges.push_back({
            vertex1,
            vertex2,
            weight
        });

        originalEdges.push_back({
            vertex2,
            vertex1,
            weight
        });

        bellmanEdges.push_back({
            vertex1,
            vertex2,
            weight
        });

        bellmanEdges.push_back({
            vertex2,
            vertex1,
            weight
        });
    }

    int startVertex, targetVertex;

    cout << "Vertex awal dan vertex tujuan: " << endl;
    cin >> startVertex >> targetVertex;

    vector<long long> h;

    if (!bellmanFord(vertexCount, bellmanEdges, h)) {
        cout << endl;
        cout << "Hasil Algoritma Johnson" << endl;
        cout << "Graf mengandung negative-weight cycle!" << endl;
        return 0;
    }

    vector<vector<pair<int, long long>>> graph(vertexCount + 1);

    for (const Edge& edge : originalEdges) {
        long long newWeight = edge.weight + h[edge.vertex] - h[edge.nextVertex];

        graph[edge.vertex].push_back({
            edge.nextVertex,
            newWeight
        });
    }

    dijkstra( startVertex, targetVertex, vertexCount, graph, h);

    return 0;
}