#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void findAndPrintLMIS(const vector<int>& sequence) {
    int n = sequence.size();

    vector<int> dp(n, 1);
    vector<int> parent(n, -1);

    int maxLength = 1;
    int endIndex = 0;

    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (sequence[i] > sequence[j] && dp[i] < dp[j] + 1) {
                dp[i] = dp[j] + 1;
                parent[i] = j;
            }
        }

        if (dp[i] > maxLength) {
            maxLength = dp[i];
            endIndex = i;
        }
    }

    vector<int> lmis_sequence;
    int curr = endIndex;

    while (curr != -1) {
        lmis_sequence.push_back(sequence[curr]);
        curr = parent[curr];
    }

    reverse(lmis_sequence.begin(), lmis_sequence.end());

    cout << "\n================ HASIL ================\n";

    cout << "Urutan Awal : [ ";
    for (int num : sequence) {
        cout << num << " ";
    }
    cout << "]\n";
    cout << "Panjang LMIS: " << maxLength << "\n";

    cout << "Subsequence : [ ";
    for (int num : lmis_sequence) {
        cout << num << " ";
    }
    cout << "]\n";
    cout << "=======================================\n";
}

int main() {
    int pilihan;

    do {
        int n;

        cout << "\n=== PROGRAM LARGEST MONOTONICALLY INCREASING SUBSEQUENCE ===\n";
        cout << "Masukkan jumlah bilangan (ketik 0 untuk keluar): ";
        cin >> n;

        if (n == 0) {
            cout << "Program selesai, Terima Kasih." << endl;
            return 0;
        }

        if (n < 0) {
            cout << "Jumlah bilangan harus lebih dari 0" << endl;
            continue;
        }

        vector<int> sequence(n);

        cout << "Masukkan " << n << " bilangan:\n";

        for (int i = 0; i < n; ++i) {
            cin >> sequence[i];
        }

        findAndPrintLMIS(sequence);

        cout << "\nMasukkan 1 untuk mencoba lagi atau 0 untuk keluar: ";
        cin >> pilihan;

    } while (pilihan != 0);

    cout << "Program selesai. Terima Kasih." << endl;

    return 0;
}