#ifndef __MY_UTILS__
#define __MY_UTILS__

#include "stdafx.h"
#include <string>
class CMyUtils{

public:
	static std::string & std_string_format(std::string & _str, const char * _Format, ...){
		va_list marker = NULL;
		va_start(marker, _Format);

		size_t num_of_chars = _vscprintf(_Format, marker);

		if (num_of_chars > _str.capacity()){
			_str.resize(num_of_chars + 1);
		}

		vsprintf((char *)_str.c_str(), _Format, marker);

		va_end(marker);
		return _str;
	}

	static std::string getFilePath(){
		OPENFILENAME opfn;
		WCHAR file_name[MAX_PATH];//file name 
		ZeroMemory(&opfn, sizeof(OPENFILENAME));
		opfn.lStructSize = sizeof(OPENFILENAME);//ָ������ṹ�Ĵ�С
		opfn.lpstrFilter = _T("�����ļ�\0*.*\0");//ָ��һ���Կ��ַ������Ĺ����ַ�����һ�����塣
		//�����е����һ���ַ�������������  NULL�ַ�������
		opfn.nFilterIndex = 1;	//ָ�����ļ����Ϳؼ��е�ǰѡ��Ĺ�����������
		opfn.lpstrFile = file_name; //ָ�������ʼ���ļ����༭�ؼ�ʹ�õ��ļ����Ļ���
		opfn.lpstrFile[0] = '\0'; 	//�������ĵ�һ���ַ�������NULL
		opfn.nMaxFile = sizeof(file_name);
		//ָ��lpstrFile����Ĵ�С����TCHARsΪ��λ��
		//����ANSI�汾�����ֽڵĸ���������Unicode�汾�����ַ��ĸ�����
		opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;  //OFN_FILEMUSTEXIST ָ���û����������ļ�����¼�ֶ���
		//�����Ѵ��ڵ��ļ������֡�
		//��������Ǳ�ָ���Ĳ����û�������һ����Ч�����֣��Ի��������ʾһ���ȴ���Ϣ��
		//��������Ǳ�ָ����OFN_PATHMUSTEXIST���Ҳ��ʹ�á�
		if (GetOpenFileName(&opfn)){
			return w2a(opfn.lpstrFile);
		}
		return "";
	}
};

#endif
