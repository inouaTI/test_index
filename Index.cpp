/***********************************
Index.cpp
2015/01/23 12:20 井上太一

【機能】文字ファイルから単語と行を抽出し、ファイル
サンプル読込ファイル名："data/sample.txt"
書込ファイル名："data/index.txt"
***********************************/

#include <fstream>
#include <iostream>
#include <map>
#include <regex>

void index();
void out_file(std::wofstream &woutfile, std::map< std::wstring, std::vector< int > > &mp);
void print(std::map < std::wstring, std::vector< int > > &mp);
std::vector< std::wstring > reg(std::wstring &str);
void locale_jp();

int main()
{
	locale_jp();
	index();
}

#define TEST 10
// ========== 開始 ==========
void index()
{
#if TEST
	std::wifstream winfile(L"data/sample.txt");				//サンプル読込ファイル名
#else
	std::wifstream winfile(L"data/C++11_Syntax_and_Feature.txt");//読込ファイル名
#endif
	if (winfile.fail()) {
		std::wcout << L"読込失敗" << std::endl;
		return;
	}
	std::wofstream woutfile(L"data/index.txt");				//書込ファイル名
	if (woutfile.fail()) {
		std::wcout << L"書込ファイルを開けない" << std::endl;
		return;
	}

	std::wstring word;
	std::map< std::wstring, std::vector< int > > mp;		//抽出データ + 行+
	int l_no = 0;												//行カウンター

	while (getline(winfile, word)) {							//空行を含む1行読込
		l_no++;
		std::vector< int > v = { l_no };						//行格納
		if (mp.find(word) == mp.end()) {						//新
			std::vector< std::wstring > reg_wd = reg(word);		//正規表現で分割
			for (auto &rwd : reg_wd) {
				if (mp.find(rwd) == mp.end()) {
					mp.insert(make_pair(rwd, v));				//分割を格納
				}
				else { mp.find(rwd)->second.push_back(l_no); }	// + 行
			}
		}
		else { mp.find(word)->second.push_back(l_no); }			//新単語
	}
	out_file(woutfile, mp);		//ファイル出力

#if TEST
	print(mp);					//コンソールに試験出力
#endif
}

//部分一致をすべて検索
//text:検索文字列
//return:検索文字列に分けて格納
std::vector< std::wstring > reg(std::wstring &text)
{
	std::wregex re(LR"([!-~]+|[ァ-ヶー]+|[あ-ん亜-黑]+)");
	std::wsregex_iterator iter(text.cbegin(), text.cend(), re);
	std::wsregex_iterator end;
	std::vector< std::wstring > result;

	for (; iter != end; ++iter) {
		result.push_back(iter->str());
	}
	return result;
}

const int CLM = 25;		//行番号を表示する位置
//ファイル出力
//woutfile:書出しファイル名
//mp:<単語、行>
void out_file(std::wofstream &woutfile, std::map< std::wstring, std::vector< int > > &mp)
{
	for (auto out : mp) {
		woutfile << out.first;

		int str_l = wcslen(out.first.c_str());
		if (out.first.at(0) < L'~') {
			str_l /= 2;
		}
		str_l = str_l < CLM ? CLM - str_l : 1;
		for (int j = 0; j < str_l; j++) {
			woutfile << L'・';
		}

		char c[10];
		for (size_t i = 0; i < out.second.size(); i++) {
			sprintf_s(c, "%d", out.second.at(i));
			woutfile << ',' << c;
			//woutfile << ',' << out.second.at(i);	//ロケールで数字に区切り','が入るため不可
		}
		woutfile << std::endl;
	}
}

//コンソールに試験出力
//mp:<単語、行>
void print(std::map < std::wstring, std::vector< int > > &mp)
{
	for (auto &wrd : mp) {
		std::wcout << wrd.first.c_str();

		int str_l = wcslen(wrd.first.c_str());
		if (wrd.first.at(0) < L'~') {
			str_l /= 2;
		}
		str_l = str_l < CLM ? CLM - str_l : 1;
		for (int j = 0; j < str_l; j++) {
			std::wcout << L'・';
		}

		char c[10];
		for (size_t i = 0; i < wrd.second.size(); i++) {
			sprintf_s(c, "%d", wrd.second.at(i));
			std::wcout << L',' << c;
		}
		std::cout << std::endl;
	}
}

//ロケール
void locale_jp()
{
	std::locale loc("japanese");
	std::locale::global(loc);
	std::wcout.imbue(loc);
}

//不可 std::wcout.imbue(locale::locale("japanese"));
//不可	   wcout.imbue(locale("japanese"));
