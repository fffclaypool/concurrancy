#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <future>
using namespace std;

vector<char> readFile(const string &inPath)
{
    ifstream file(inPath, ios::binary | ios::ate);
    size_t length = (size_t)file.tellg();
    vector<char> buffer(length);
    file.seekg(0, std::ios::beg);
    file.read(&buffer[0], length);
    return buffer;
}

size_t writeFile(const vector<char> &buffer, const string &outPath)
{
    ofstream file(outPath, ios::binary);
    file.write(&buffer[0], buffer.size());
    return (size_t)file.tellp();
}

size_t packagedtask_copyFile(const string &inFile, const string &outFile)
{
    /*
        - auto
        変数宣言時に具体的な型名のかわりにautoキーワードを指定することで, 変数の型を初期化子から推論できる
        - async
        この関数は, 指定された関数を非同期実行して結果値を取得するためのfutureオブジェクトを返す.
        返されたfutureオブジェクトのget()もしくはwait()を呼び出すことで, 非同期実行の完了を待つ
     */

    auto fut1 = async(readFile, inFile);
    auto fut2 = async([&fut1](const string &path)
                      { return writeFile(fut1.get(), path); },
                      outFile);
    return fut2.get();
}

int main(int argc, char *argv[])
{
    string src = argv[1];
    string dest = argv[2];
    packagedtask_copyFile(src, dest);
    return 0;
}
