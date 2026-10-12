#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

const char nodeNames[] = {'u', 'v', 'w', 'x', 'y', 'z'};

const int dist[6][6] = {
    {  0, 51, 56, 60,  2, 35},
    { 51,  0, 78, 13, 51, 68},
    { 56, 78,  0, 70, 57, 21},
    { 60, 13, 70,  0, 61, 68},
    {  2, 51, 57, 61,  0, 36},
    { 35, 68, 21, 68, 36,  0} 
};

int hitungTotalBobot(const vector<int>& rute) {
    int total = 0;
    for (size_t i = 0; i < rute.size() - 1; i++) {
        total += dist[rute[i]][rute[i+1]];
    }
    return total;
}

void cetakRute(const vector<int>& rute) {
    for (size_t i = 0; i < rute.size(); i++) {
        cout << nodeNames[rute[i]];
        if (i < rute.size() - 1) cout << " -> ";
    }
    cout << "\n";
}

int main() {
    int startNode = 0;
    vector<int> rute;
    vector<bool> dikunjungi(6, false);
    
    int current = startNode;
    rute.push_back(current);
    dikunjungi[current] = true;

    for (int step = 0; step < 5; step++) {
        int nextNode = -1;
        int minDist = INT_MAX;
        
        for (int j = 0; j < 6; j++) {
            if (!dikunjungi[j] && dist[current][j] < minDist && dist[current][j] > 0) {
                minDist = dist[current][j];
                nextNode = j;
            }
        }
        
        rute.push_back(nextNode);
        dikunjungi[nextNode] = true;
        current = nextNode;
    }
    rute.push_back(startNode);
    
    cout << "Hasil Nearest Neighbor:\n";
    cout << "Rute  : ";
    cetakRute(rute);
    cout << "Bobot : " << hitungTotalBobot(rute) << "\n\n";

    bool adaPerbaikan = true;
    while (adaPerbaikan) {
        adaPerbaikan = false;
        for (size_t i = 1; i < rute.size() - 2; i++) {
            for (size_t j = i + 1; j < rute.size() - 1; j++) {
                int bobotLama = dist[rute[i-1]][rute[i]] + dist[rute[j]][rute[j+1]];
                int bobotBaru = dist[rute[i-1]][rute[j]] + dist[rute[i]][rute[j+1]];
                
                if (bobotBaru < bobotLama) {
                    reverse(rute.begin() + i, rute.begin() + j + 1);
                    adaPerbaikan = true;
                }
            }
        }
    }

    cout << "Hasil Setelah Optimasi 2-opt:\n";
    cout << "Rute  : ";
    cetakRute(rute);
    cout << "Bobot : " << hitungTotalBobot(rute) << "\n";

    return 0;
}