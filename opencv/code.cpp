#include<iostream>
#include <string>
#include <vector>
using namespace std;

int main()
{
	vector<char> s;
	char c;
	int nCount = 1;
	while (cin >> c)
		s.push_back(c);

	for (int i = 0; i < s.size() - 1; i++)
	{
		if ((int)s[i] ^ (int)s[i + 1] == 1)
		{
			nCount++;
		}
	}
	cout << nCount;
	return 0;

}