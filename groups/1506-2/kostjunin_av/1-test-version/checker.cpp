// checker.cpp : Defines the entry point for the console application.
//



#include <cstdio> 
#include <cmath> 
#include <string> 
#include <iostream>

using namespace std;

// ������������ ��� �������������� � ����������� �������� 
//////////////////////////////////////////////////////////////////////////////////////////// 
/*
// Checker ����� ������������� ��� ��� ��� ��������:
AC = Accepted = ������� ����� ���������� ��������� �� ������ �����
WA = Wrong Answer = ������� ����� ������������ ��������� �� ������ �����
PE = Presentation Error = ������ ������� �������� ������

// ��������� �������� checker �� ����� �������������
NO = No verdict = ������� �����������
CE = Compilation Error = ������ ����������
ML = Memory Limit Exceeded = ��������� ����������� �� ������
TL = Time Limit Exceeded = ��������� ����������� �� ������� ������
RE = Runtime Error = ������ ������� ���������� ���������
IL = Idle Limit Exceeded = ��������� ����� ������� (�����������) ���������
DE = Deadly Error = ������ ����������� �������
*/
enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };

class result
{
private:
	FILE * bur;
public:
	enum ext_cls { NO = 1, VERDICT, MESSAGE, TIME, MEMORY };
	result(bool read = false)
	{
		if (read) bur = fopen("result.txt", "r"); else bur = fopen("result.txt", "w");
	}
	result(const char* test, bool read = false)
	{
		std::string name(test);
		name = "result" + name + ".txt";
		auto res_name = name.c_str();
		if (read) bur = fopen(res_name, "r"); else bur = fopen(res_name, "w");
	}
	~result() { fclose(bur); }
	void write_type(ext_cls t) { fwrite(&t, sizeof(t), 1, bur); }

	// �������� ����������� �������, ��� ������� �������� ���� �� ��������� verdict  
	void write_verdict(verdict v)
	{
		write_type(ext_cls::VERDICT); fwrite(&v, sizeof(v), 1, bur);
	}

	// �������� ��������� �� checker'a ������������.  
	//   ��������, ��� ������� ������, ��� ��������.  
	//   ������������ ������ ��������� ����� � ����� ���������� 
	void write_message(string str)
	{
		write_type(ext_cls::MESSAGE); int l = str.size(); fwrite(&l, sizeof(l), 1, bur);
		fwrite(&str[0], sizeof(str[0]), l, bur);
	}

	// �������� ����������� ������� ����� ������ ��������� ���������,  
	//   ����������� � ������� before_code 
	// x ����� ����������� 100 �� = 10 ^ (-7) ��� 
	void write_time(long long x)
	{
		write_type(ext_cls::TIME); fwrite(&x, sizeof(x), 1, bur);
	}

	// �������� ����������� �������, ������ ������������� ���������� ��������� 
	void write_memory(unsigned long long x)
	{
		write_type(ext_cls::MEMORY); fwrite(&x, sizeof(x), 1, bur);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////// 

void wa(result& checker_result)//��������� ������ ������� �������� ������
{
	checker_result.write_message("PE. Output format is not correct.");
	checker_result.write_verdict(verdict::PE);
}



int main(int argc, char * argv[])
{

	if (argc == 1)
	{
		std::cerr << "too little args";
		return argc;
	}
	static result checker_result(argv[1]);

	std::string test_name = argv[1];//��� �����

	auto in = freopen(test_name.c_str(), "rb", stdin);					//������� ������
	FILE * out = fopen((test_name + ".ans").c_str(), "rb");		//�������� ������("����� ���������") - ��������� ������ ���������� ������� ����������
	FILE * answer = fopen((test_name + ".txt").c_str(), "rb");	//���������� �����("����� ����") - ��������� ������ ��������� ������� ����������

	if (in == 0 || out == 0 || answer == 0)
		cerr << "file not found";

	unsigned long long N;
	// ��������� ������ �������
	fread(&N, sizeof(N), 1, stdin);
	fclose(stdin);

	// �������� ������ ��� ������ ���� � ������ ��������� (��� �������)
	long double * ans = new long double[N], *res = new long double[N];
	double ans_time, res_time;

	// ��������� ����� ������ ��������� ��������� � �������������� ������ ��������� 
	if (fread(&res_time, sizeof(res_time), 1, out) != 1) wa(checker_result);	//����� ������ 
	if (fread(res, sizeof(*res), N, out) != N) wa(checker_result);			//��������� ������
	// ��������� ����� ������ ��������� � �������������� ������ ���� 
	if (fread(&ans_time, sizeof(ans_time), 1, answer) != 1)	wa(checker_result);//����� ������ 
	if (fread(ans, sizeof(*ans), N, answer) != N) wa(checker_result);			//��������� ������

	//�������� - ���������� �������
	if (equal(ans, ans + N, res, res + N))
	{
		checker_result.write_message("AC. Numbers are equal.");
		checker_result.write_verdict(verdict::AC);
		std::cout << "AC. Numbers are equal." << std::endl;
	}
	else
	{
		checker_result.write_message("WA. Output is not correct.");
		checker_result.write_verdict(verdict::WA);
		std::cout << "WA. Output is not correct." << std::endl;
	}

	// ���������� ����� � ���������� ����������� (��������� �� 100 �� = 10 ^ (-7) ���). 
	checker_result.write_time(res_time * 1e7);

	fclose(out);
	fclose(answer);

	std::cout << "time = " << res_time << std::endl;
	std::cout << "speed up = " << ans_time / res_time<< std::endl;

	return 0;
}
