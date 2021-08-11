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
        packaged_taskは, 「別スレッドでの処理完了を待ち, その処理結果を取得する」といった非同期処理を実現するための
        クラスであり, futureクラスと組み合わせて使用する. packaged_taskに登録した非同期実行する関数の戻り値をfu
        tureが読み取る. packaged_taskとfutureは内部的に同一の共有状態を参照する.
     */

    /*
        エイリアステンプレートは, テンプレートによって型の別名を定義する機能である. これによって, パラメータ化した型の別
        名付けができる. エイリアステンプレートによる型の別名付けには, usingキーワードを使用する.
     */

    using Task_Type_Read = vector<char>(const string &);
    packaged_task<Task_Type_Read> pt1(readFile);
    future<vector<char>> fut1{pt1.get_future()};
    // movie: 左辺値を右辺値にキャストする
    thread th1{move(pt1), inFile};

    using Task_Type_Write = size_t(const string &);
    packaged_task<Task_Type_Write> pt2([&fut1](const string &path)
                                       { return writeFile(fut1.get(), path); });
    future<size_t> fut2{pt2.get_future()};
    thread th2{move(pt2), outFile};

    size_t result = fut2.get();
    th1.join();
    th2.join();

    return result;
}

int main(int argc, char *argv[])
{
    string src = argv[1];
    string dest = argv[2];
    packagedtask_copyFile(src, dest);
    return 0;
}
