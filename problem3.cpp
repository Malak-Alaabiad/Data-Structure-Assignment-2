#include <iostream>
using namespace std;
int main()
{
    cout << "Hello!" << endl;
    cout << "input format :\n1 - size array\n"
         << "2 - k\n"
         << "3 - array\n"
         << "example :\n3 2\n1 0 0\n";
    string s;
    cout << "enter the name of the file : ";
    cin >> s;
    freopen(s.c_str(), "r", stdin);
    cout << "output in file (output.txt)";
    freopen("output.txt", "w", stdout);
    int n, k;
    cin >> n >> k;
    int arr[n];
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }
    int answer = 0, sum = 0, prefix[n + 1];
    for (int i = 0; i < n + 1; i++)
    {
        prefix[i] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        sum -= prefix[i];
        if ((arr[i] + sum) % 2)
            continue;
        if (i + k - 1 < n)
        {
            answer++;
            sum++;
            prefix[i + k]++;
        }
        else
        {
            cout << -1 << '\n';
            return 0;
        }
    }
    cout << answer << '\n';
    return 0;
}