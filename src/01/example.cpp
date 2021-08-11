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

size_t future_copyFile(const string &inFile, const string &outFile)
{
    /*
        promiseは, 「別スレッドでの処理完了を待ち, その処理結果を取得する」といった非同期処理を実現するためのクラス
        であり, futureクラスと組み合わせて使用する. promiseが別スレッドでの処理結果を書き込み, futureがその結
        果を読み取る. promiseとfutureは内部的に同一の共有状態を参照する. これによってスレッド間での値の受け渡し
        やスレッド間同期を実現する.
     */

    // promiseオブジェクトを生成する
    std::promise<vector<char> > prom1;
    // 結果取得のためのfutureオブジェクトを取得する
    std::future<vector<char> > fut1 = prom1.get_future();
    // クラスthreadは, 新しいスレッドの作成/待機/その他操作を行う機構を提供する
    std::thread th1([&prom1, inFile]()
                    {
                        // 結果の値をpromiseオブジェクトに格納する
                        prom1.set_value(readFile(inFile));
                    });

    std::promise<int> prom2;
    std::future<int> fut2 = prom2.get_future();
    std::thread th2([&fut1, &prom2, outFile]()
                    { prom2.set_value(writeFile(fut1.get(), outFile)); });

    // 共有状態が準備完了状態(future_status::ready)になるまでwait()で待機し, 共有状態に格納
    // されている値を取得する
    size_t result = fut2.get();

    // スレッドが終了するまで待機する
    th1.join();
    th2.join();

    return result;
}

int main(int argc, char *argv[])
{
    string src = argv[1];
    string dest = argv[2];
    future_copyFile(src, dest);
    return 0;
}
