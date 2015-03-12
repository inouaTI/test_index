/***********************************
Index.cpp
2015/01/23 12:20 ��㑾��

�y�@�\�z�����t�@�C������P��ƍs�𒊏o���A�t�@�C��
�Ǎ��t�@�C�����F"data/sample.txt"
�����t�@�C�����F"data/index.txt"
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
// ========== �J�n ==========
void index()
{
#if TEST
	std::wifstream winfile(L"data/sample.txt");				//�T���v���Ǎ��t�@�C����
#else
	std::wifstream winfile(L"data/C++11_Syntax_and_Feature.txt");//�Ǎ��t�@�C����
#endif
	if (winfile.fail()) {
		std::wcout << L"�Ǎ����s" << std::endl;
		return;
	}
	std::wofstream woutfile(L"data/index.txt");				//�����t�@�C����
	if (woutfile.fail()) {
		std::wcout << L"�����t�@�C�����J���Ȃ�" << std::endl;
		return;
	}

	std::wstring word;
	std::map< std::wstring, std::vector< int > > mp;		//���o�f�[�^ + �s+
	int l_no = 0;												//�s�J�E���^�[

	while (getline(winfile, word)) {							//��s���܂�1�s�Ǎ�
		l_no++;
		std::vector< int > v = { l_no };						//�s�i�[
		if (mp.find(word) == mp.end()) {						//�V
			std::vector< std::wstring > reg_wd = reg(word);		//���K�\���ŕ���
			for (auto &rwd : reg_wd) {
				if (mp.find(rwd) == mp.end()) {
					mp.insert(make_pair(rwd, v));				//�������i�[
				}
				else { mp.find(rwd)->second.push_back(l_no); }	// + �s
			}
		}
		else { mp.find(word)->second.push_back(l_no); }			//�V�P��
	}
	out_file(woutfile, mp);		//�t�@�C���o��

#if TEST
	print(mp);					//�R���\�[���Ɏ����o��
#endif
}

//������v�����ׂČ���
//text:����������
//return:����������ɕ����Ċi�[
std::vector< std::wstring > reg(std::wstring &text)
{
	std::wregex re(LR"([!-~]+|[�@-���[]+|[��-��-�K]+)");
	std::wsregex_iterator iter(text.cbegin(), text.cend(), re);
	std::wsregex_iterator end;
	std::vector< std::wstring > result;

	for (; iter != end; ++iter) {
		result.push_back(iter->str());
	}
	return result;
}

const int CLM = 25;		//�s�ԍ���\������ʒu
//�t�@�C���o��
//woutfile:���o���t�@�C����
//mp:<�P��A�s>
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
			woutfile << L'�E';
		}

		char c[10];
		for (size_t i = 0; i < out.second.size(); i++) {
			sprintf_s(c, "%d", out.second.at(i));
			woutfile << ',' << c;
			//woutfile << ',' << out.second.at(i);	//���P�[���Ő����ɋ�؂�','�����邽�ߕs��
		}
		woutfile << std::endl;
	}
}

//�R���\�[���Ɏ����o��
//mp:<�P��A�s>
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
			std::wcout << L'�E';
		}

		char c[10];
		for (size_t i = 0; i < wrd.second.size(); i++) {
			sprintf_s(c, "%d", wrd.second.at(i));
			std::wcout << L',' << c;
		}
		std::cout << std::endl;
	}
}

//���P�[��
void locale_jp()
{
	std::locale loc("japanese");
	std::locale::global(loc);
	std::wcout.imbue(loc);
}

//�s�� std::wcout.imbue(locale::locale("japanese"));
//�s��	   wcout.imbue(locale("japanese"));
