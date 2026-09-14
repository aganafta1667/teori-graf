#include <bits/stdc++.h>
using namespace std;

long jumlahVertex, jumlahEdge;
vector<vector<pair<long, long>>> graph;

void hubungkan(long asal, long tujuan, long bobot){
    graph[asal].push_back({tujuan, bobot});
    graph[tujuan].push_back({asal, bobot});
}

using DataPQ = tuple<long, long, long>;

void cariPrim(long awal, vector<DataPQ> &mst, long &bobotTotal){
    vector<bool> sudahDikunjungi(jumlahVertex + 1, false);
    priority_queue<DataPQ, vector<DataPQ>, greater<DataPQ>> antrian;

    antrian.push({0, awal, -1});
    bobotTotal = 0;

    while(!antrian.empty()){
        auto data = antrian.top();
        antrian.pop();

        long bobot = get<0>(data);
        long vertex = get<1>(data);
        long induk = get<2>(data);

        if(sudahDikunjungi[vertex])
            continue;

        sudahDikunjungi[vertex] = true;
        bobotTotal += bobot;

        if(induk != -1){
            mst.push_back({induk, vertex, bobot});
        }

        for(size_t i = 0; i < graph[vertex].size(); i++){
            long tetangga = graph[vertex][i].first;
            long nilaiBobot = graph[vertex][i].second;

            if(!sudahDikunjungi[tetangga]){
                antrian.push({nilaiBobot, tetangga, vertex});
            }
        }
    }
}

int main(){
    cin >> jumlahVertex >> jumlahEdge;

    graph.resize(jumlahVertex + 1);

    for(long i = 0; i < jumlahEdge; i++){
        long asal, tujuan, bobot;
        cin >> asal >> tujuan >> bobot;

        hubungkan(asal, tujuan, bobot);
    }

    long vertexAwal;
    cout << "Vertex pertama: ";
    cin >> vertexAwal;

    vector<DataPQ> mst;
    long bobotTotal;

    cariPrim(vertexAwal, mst, bobotTotal);

    cout << "\nHASIL ALGORITMA PRIM\n";

    for(size_t i = 0; i < mst.size(); i++){
        long asal = get<0>(mst[i]);
        long tujuan = get<1>(mst[i]);
        long bobot = get<2>(mst[i]);

        cout << "v" << asal << " - v" << tujuan << " : " << bobot << "\n";
    }

    cout << "bobot spanning tree terkecil: " << bobotTotal << "\n";

    return 0;
}