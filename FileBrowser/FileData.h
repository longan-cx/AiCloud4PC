#pragma once
#include "stdafx.h"
#include "MD5.h"

#define TR_STATUS_NOR     0
#define TR_STATUS_TRING   1
#define TR_STATUS_TROVER  2
#define TR_STATUS_TRCANLE 3

#define TR_YTPE_NOR		 0
#define TR_TYPE_DOWNLOAD 1
#define TR_YTPE_UPLOAD   2

class CFileData
{
public:
	static std::string getFileType(size_t itype)
	{
		std::string types[18] = { "dir", "doc", "app", "cda", "exe",
			"pdf", "tt", "mp3", "avi", "xls", "vsd", "txt", "apk", "ipa", "rar", "other", "other", "other" };
		if (itype > 17){
			return types[17];
		}
		return types[itype];
	}

public:

	CFileData(std::string name, size_t size=0, std::string type="dir", 
		std::string create_time = "2018/6/23 22:10:59", 
		std::string modify_time = "2018/6/23 22:10:59")
	{
		this->name = name;
		this->type = type;
		this->size = size;
		this->setImage();
		this->create_time = create_time;
		this->modify_time = modify_time;
		isselect = false;
		tr_status = TR_STATUS_NOR;
		tr_type = TR_YTPE_NOR;
		tr_time = "--";
		tr_progress_text = "--";
		tr_speed = "100KB/s";
		setMd5();
		size_txt = "100";
		setSizeText();
		tr_overtime = "------";
		tr_status_text = "œ¬‘ÿÕÍ≥…";
		getProgress();
	}
	~CFileData()
	{
	}

	std::string name;
	std::string type;
	size_t size;
	std::string image;
	std::string small_image;
	std::string big_image;
	std::string middle_image;
	std::string create_time;
	std::string modify_time;
	std::string tr_time;
	std::string tr_progress_text;
	std::string tr_speed;
	std::string size_txt;
	std::string tr_overtime;
	std::string tr_status_text;
	std::string md5;
	int tr_status;
	int tr_type;
	size_t tr_progress;
	size_t tr_size;
	bool isselect;

	inline void setSizeText(){
		size_t gb = 1024 * 1024 * 1024;
		size_t mb = 1024 * 1024;
		size_t kb = 1024;
		CDuiString sText;
		if (size < kb){
			sText.Format(_T("%dbit"), size);
		}else if (size > kb && size < mb){
			sText.Format(_T("%dKB"), size / kb);
		}
		else if (size > mb && size < gb){
			sText.Format(_T("%dMB"), size / mb);
		}
		else if(size > gb){
			sText.Format(_T("%.2fGB"), (float)size / gb);
		}
		size_txt = w2a((wchar_t*)sText.GetData());
	}

	inline void setSelect(bool select)
	{
		this->isselect = select;
	}

	inline int getProgress()
	{
		if (size > 0)tr_progress = (tr_size / size) * 100;
		else tr_progress = 0;
		return tr_progress;
	}

private:
	inline void setMd5(){
		std::string tmp;
		CMD5 iMD5;
		tmp.append(name);
		tmp.append(type);
		tmp.append(create_time);
		iMD5.GenerateMD5((unsigned char*)tmp.data(), tmp.size());
		this->md5 = iMD5.ToString();
		Logger subTest = Logger::getInstance(LOG4CPLUS_TEXT("CFileData.h"));
		LOG4CPLUS_DEBUG(subTest, md5.data());
	}

	inline void setImage(){
		std::string imagefile;
		if (type == "dir"){
			imagefile = "FolderType.png";
		}
		else if (type == "doc"){
			imagefile = "DocType.png";
		}
		else if (type == "app"){
			imagefile = "Apps.png";
		}
		else if (type == "cda"){
			imagefile = "CADType.png";
		}
		else if (type == "exe"){
			imagefile = "ExeType.png";
		}
		else if (type == "pdf"){
			imagefile = "PdfType.png";
		}
		else if (type == "mp3"){
			imagefile = "MusicType.png";
		}
		else if (type == "tt"){
			imagefile = "TorrentType.png";
		}
		else if (type == "img"){
			imagefile = "ImgType.png";
		}
		else if (type == "ppt"){
			imagefile = "PptType.png";
		}
		else if (type == "rar"){
			imagefile = "RarType.png";
		}
		else if (type == "ipa"){
			imagefile = "IpaType.png";
		}
		else if (type == "apk"){
			imagefile = "ApkType.png";
		}
		else if (type == "txt"){
			imagefile = "TxtType.png";
		}
		else if (type == "vsd"){
			imagefile = "VsdType.png";
		}
		else if (type == "xls"){
			imagefile = "XlsType.png";
		}
		else if (type == "avi"){
			imagefile = "VideoType.png";
		}

		if (!imagefile.empty()){
			CMyUtils::std_string_format(image, "res='filetype/big/%s'  dest='20,10,80,70'", imagefile.data());
			CMyUtils::std_string_format(small_image, "res='filetype/small/%s'", imagefile.data());
			CMyUtils::std_string_format(big_image, "res='filetype/big/%s'", imagefile.data());
			CMyUtils::std_string_format(middle_image, "res='filetype/middle/%s'", imagefile.data());
		}
		else{
			image = "res='filetype/big/OtherType.png'  dest='20,10,80,70'";
			small_image = "res='filetype/small/OtherType.png'";
			big_image = "res='filetype/big/OtherType'";
			middle_image = "res='filetype/middle/OtherType.png'";
		}
	
	}
};



