#pragma once
#include <cmath>
#include <cstring>
#include <sstream>
#include <fstream>
using namespace std;
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;

string SplitFilename(const string& dir, const string& fName)
{
	string path = "";
	int found = dir.find_last_of("/\\");
	path += dir.substr(0, found);
	path += "\\";
	path += fName;
	return path;
}

template <typename T>
string handleFile(T dataKey, T dataValue)
{
	fstream file;
	string fName = "file";
	string dummy = "";
	string filePair = "";
	string DATA = "";
	static int line_number = 0;
	static int file_num = 1;
	if (line_number == 100)
	{
		file_num++;
		line_number = 0;
	}

	string path = SplitFilename(__FILE__, fName + to_string(file_num) + ".txt");
	file.open(fName + to_string(file_num) + ".txt", ios::out | ios::app);
	DATA += dataKey;
	DATA += ",";
	DATA += dataValue;
	while (!file.eof())
	{
		line_number++;
		file << DATA + "," + to_string(line_number) << endl;
		getline(file, dummy);
	}

	filePair = "Value: " + dataValue + ", File: ";
	filePair += path;
	filePair += ", Line: ";
	filePair += to_string(line_number);
	return filePair;
}

int256_t stoint(string num)
{
	stringstream degree(num);
	int256_t x = 0;
	degree >> x;
	return x;
}

int256_t power(long long int x, long long int y)
{
	if (y == 0)
		return 1;
	else if (y % 2 == 0)
		return power(x, y / 2) * power(x, y / 2);
	else
		return x * power(x, y / 2) * power(x, y / 2);
}

void sortArr(string*& arr, int256_t size)
{
	int pass = 1;
	bool exchanges;
	do
	{
		exchanges = false;
		for (int64_t i = 0; i < size - pass; i++)
		{
			if (stoint(*(arr + i)) > stoint(arr[i + 1]))
			{
				string tmp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = tmp;
				exchanges = true;
			}
		}
		pass++;
	} while (exchanges);
}

int256_t mod(string num, int bits)
{
	int256_t x = power(2, bits);
	int256_t res = 0;
	for (int64_t i = 0; i < int64_t(num.length()); i++)
		res = (res * 2 + (int)num[i] - '0') % x;
	return res;
}
