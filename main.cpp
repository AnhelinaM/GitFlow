#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

int n, m, l, k; // k - число потоков
vector<vector<int>> C;
vector<vector<int>> A;
vector<vector<int>> B;

void threadFunction(int id) {
  n = A.size();
  m = A[0].size();
  l = B[0].size();
  int left = n / k * id;
  int right = n / k * (id + 1);
  if ((n - 1) < right) {
    right = n - 1;
  }
  for (int i = left; i <= right; ++i) {
    for (int j = 0; j < l; ++j) {
      C[i][j] = 0;
      for (int h = 0; h < m; ++h)
        C[i][j] += A[i][h] * B[h][j];
    }
  }
}

void threadFunction2(int id) {
  n = A.size();
  m = A[0].size();
  l = B[0].size();
  int left = m / k * id;
  int right = m / k * (id + 1);
  if (m - 1 < right) {
    right = m - 1;
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < l; ++j) {
      C[i][j] = 0;
      for (int h = left; h <= right; ++h)
        C[i][j] += A[i][h] * B[h][j];
    }
  }
}

void threadFunction3(int id) {
  n = A.size();
  m = A[0].size();
  l = B[0].size();
  int left1 = n / k * id;
  int right1 = n / k * (id + 1);
  if ((n - 1) < right1) {
    right1 = n - 1;
  }
  int left2 = m / k * id;
  int right2 = m / k * (id + 1);
  if ((m - 1) < right2) {
    right2 = m - 1;
  }
  for (int i = left1; i <= right1; ++i) {
    for (int j = 0; j < l; ++j) {
      C[i][j] = 0;
      for (int h = left2; h <= right2; ++h)
        C[i][j] += A[i][h] * B[h][j];
    }
  }
}

int main() {
  ifstream fin("input.txt");
  fin >> n >> m >> l >> k;
  int elem;
  for (int i = 0; i < n; ++i) {
    A.push_back(vector<int>());
    for (int j = 0; j < m; ++j) {
      fin >> elem;
      A[i].push_back(elem);
    }
  }
  for (int i = 0; i < m; i++) {
    B.push_back(vector<int>());
    for (int j = 0; j < l; j++) {
      fin >> elem;
      B[i].push_back(elem);
    }
  }
  fin.close();

  cout << "Обычное перемножение:\n";
  unsigned int start_time = clock();
  for (int i = 0; i < n; ++i) {
    C.push_back(vector<int>());
    for (int j = 0; j < l; ++j) {
      C[i].push_back(0);
      for (int h = 0; h < m; ++h)
        C[i][j] += A[i][h] * B[h][j];
    }
  }
  unsigned int end_time = clock();
  cout << "Время:" << end_time - start_time << '\n';

  cout << "\nПеремножение через многопоточность 1 способом:\n";
  vector<thread> ths;
  unsigned int start_time2 = clock();
  for (int i = 0; i < k; ++i) {
    ths.push_back(thread(threadFunction, i));
  }
  for (auto &th : ths) {
    th.join();
  }
  unsigned int end_time2 = clock();
  cout << "Время:" << end_time2 - start_time2 << '\n';

  cout << "\nПеремножение через многопоточность 2 способом:\n";
  vector<thread> ths2;
  unsigned int start_time3 = clock();
  for (int i = 0; i < k; ++i) {
    ths2.push_back(thread(threadFunction2, i));
  }
  for (auto &th : ths2) {
    th.join();
  }
  unsigned int end_time3 = clock();
  cout << "Время:" << end_time3 - start_time3 << '\n';

  cout << "\nПеремножение через многопоточность 3 способом:\n";
  vector<thread> ths3;
  unsigned int start_time4 = clock();
  for (int i = 0; i < k; ++i) {
    ths3.push_back(thread(threadFunction3, i));
  }
  for (auto &th : ths3) {
    th.join();
  }
  unsigned int end_time4 = clock();
  cout << "Время:" << end_time4 - start_time4 << '\n';
  return 0;
}
