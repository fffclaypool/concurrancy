#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

/*
    - 非同期処理とは
    あるタスクを実行している際に, 他のタスクが別の処理を実行できる方式のこと
    時間のかかる処理の完了を待たずに他の処理を行うことができる
 */

// ファイル読み取り用関数
vector<char> readFile(const string &inPath)
{
    // std::ifstreamというclassから, fileというインスタンスを生成
    ifstream file(inPath, ios::binary | ios::ate);
    // ストリームバッファから現在の読み取り位置を取得
    size_t length = (size_t)file.tellg();
    // 変数bufferという可変長配列を定義する
    vector<char> buffer(length);
    // ストリームバッファに対して, 読み取り位置の移動を指示する
    file.seekg(0, std::ios::beg);
    // ストリームから指定した数の文字を入力する
    file.read(&buffer[0], length);
    return buffer;
}

// ファイル書き込み用関数
size_t writeFile(const vector<char> &buffer, const string &outPath)
{
    // std::ofstreamというclassから, fileというインスタンスを生成
    ofstream file(outPath, ios::binary);
    // 配列の最初のn文字をストリームに挿入する
    file.write(&buffer[0], buffer.size());
    // 出力ストリームにおける現在の文字の位置を返す
    return (size_t)file.tellp();
}

// ファイルコピー用関数
size_t sync_copyFile(const string &inFile, const string &outFile)
{
    return writeFile(readFile(inFile), outFile);
}

int main(int argc, char *argv[])
{
    string src = argv[1];
    string dest = argv[2];
    sync_copyFile(src, dest);
}
