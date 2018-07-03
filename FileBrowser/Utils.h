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
		opfn.lStructSize = sizeof(OPENFILENAME);//指定这个结构的大小
		opfn.lpstrFilter = _T("所有文件\0*.*\0");//指向一对以空字符结束的过滤字符串的一个缓冲。
		//缓冲中的最后一个字符串必须以两个  NULL字符结束。
		opfn.nFilterIndex = 1;	//指定在文件类型控件中当前选择的过滤器的索引
		opfn.lpstrFile = file_name; //指向包含初始化文件名编辑控件使用的文件名的缓冲
		opfn.lpstrFile[0] = '\0'; 	//这个缓冲的第一个字符必须是NULL
		opfn.nMaxFile = sizeof(file_name);
		//指定lpstrFile缓冲的大小，以TCHARs为单位。
		//对于ANSI版本，是字节的个数；对于Unicode版本，是字符的个数。
		opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;  //OFN_FILEMUSTEXIST 指定用户仅可以在文件名登录字段中
		//输入已存在的文件的名字。
		//如果这个标记被指定的并且用户输入了一个无效的名字，对话框程序显示一个等待消息框。
		//如果这个标记被指定，OFN_PATHMUSTEXIST标记也被使用。
		if (GetOpenFileName(&opfn)){
			return w2a(opfn.lpstrFile);
		}
		return "";
	}
};

#endif
