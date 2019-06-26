#ifndef UTF_CONVERT_UTIL_HPP_
#define UTF_CONVERT_UTIL_HPP_
#include <string>
#include "boost/locale.hpp"

#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#include <Shellapi.h>
#pragma comment(lib, "Shell32.lib");
#define snprintf _snprintf
#else
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <langinfo.h>
#define stricmp(a,b) strcasecmp(a,b)
#endif

using namespace std;

namespace utfConvert
{
	class util
	{
	public:
		enum
		{
			FILEPATH_ENCODING_UNKNOWN = -1,
			FILEPATH_ENCODING_ASCII,
			FILEPATH_ENCODING_UTF8,
			FILEPATH_ENCODING_UTF16,
		};
		
		static string create_temp_dir()
		{
			boost::system::error_code err;
			char dest_dir[256];
			boost::posix_time::ptime now;
			boost::posix_time::time_duration td;
			boost::gregorian::date today=boost::gregorian::day_clock::local_day();
			now = boost::posix_time::microsec_clock::local_time();
			td= now.time_of_day();
			// get uuid
			boost::uuids::basic_random_generator<boost::mt19937> gen;
			boost::uuids::uuid _uuid = gen();
			char szbuff[33]={0};
			for(int i = 0 ; (i < _uuid.size())&&(2*i<33) ; i++)
			{
				snprintf(szbuff+2*i,sizeof(szbuff),"%02X",_uuid.data[i]);
			}
			szbuff[32] = 0;

			string tempdir=temp_path();
			snprintf(dest_dir, sizeof(dest_dir)-1,"%sunzip/%04d_%02d_%02d-%02d_%02d_%02d_%s",tempdir.c_str(),(int)today.year(), (int)today.month(),(int)today.day(), (int)td.hours(), (int)td.minutes(), (int)td.seconds(),szbuff);
			boost::filesystem::create_directories(boost::filesystem::path(dest_dir),err);
			return dest_dir;
		}

		static string create_temp_str()
		{
			boost::system::error_code err;
			char dest_dir[256];
			boost::posix_time::ptime now;
			boost::posix_time::time_duration td;
			boost::gregorian::date today=boost::gregorian::day_clock::local_day();
			now = boost::posix_time::microsec_clock::local_time();
			td= now.time_of_day();
			// get uuid
			boost::uuids::basic_random_generator<boost::mt19937> gen;
			boost::uuids::uuid _uuid = gen();
			char szbuff[33]={0};
			for(int i = 0 ; (i < _uuid.size())&&(2*i<33) ; i++)
			{
				snprintf(szbuff+2*i,sizeof(szbuff),"%02X",_uuid.data[i]);
			}
			szbuff[32] = 0;

			string tempdir=temp_path();
			snprintf(dest_dir, sizeof(dest_dir)-1,"%sunzip/%04d_%02d_%02d-%02d_%02d_%02d_%s",tempdir.c_str(),(int)today.year(), (int)today.month(),(int)today.day(), (int)td.hours(), (int)td.minutes(), (int)td.seconds(),szbuff);

			return dest_dir;
		}

		static string self_path()
		{
			string current_path = boost::filesystem::initial_path<boost::filesystem::path>().string();
			return current_path;
		}

		static string temp_path()
		{
			string long_path = boost::filesystem::temp_directory_path().string();
			long_path.append("/cc/");
			return long_path;
		}
		static string filename_without_path(const string& path)
		{
			int n = path.rfind('\\');
			if (n==string::npos) 
			{
				n = path.rfind('/');
				if (n==string::npos)  return "";
			}

			string name=path.substr(n+1, path.length()-n-1);
			return name;
		}

		static string between(char const * p,
			std::string const & 	to_encoding,
			std::string const & 	from_encoding)
		{
			return boost::locale::conv::between(p, to_encoding, from_encoding );
		}

		static string from_utf8(const char* p,std::string const& to_encoding = "default")
		{
			if ("default" == to_encoding)
				return boost::locale::conv::from_utf<char>(p,charset());
			return boost::locale::conv::from_utf<char>(p,to_encoding);
			
		}

#ifdef WIN32 //wchar_t windows 16bits linux 32bits
		static string from_utf16(const wchar_t* p)
		{
			return boost::locale::conv::from_utf<wchar_t>(p,charset());
		}

		static wstring to_utf16(const char* p)
		{
			return boost::locale::conv::to_utf<wchar_t>(p,charset());
		}

		static wstring utf8_to_utf16(const char* p)
		{
			return boost::locale::conv::utf_to_utf<wchar_t>(p);
		}

		static string utf16_to_utf8(const wchar_t* p)
		{
			return boost::locale::conv::utf_to_utf<char>(p);
		}
 #else
 		static string from_utf32(const wchar_t* p)
 		{
 			return boost::locale::conv::from_utf<wchar_t>(p,charset());
 		}
 
 		static wstring to_utf32(const char* p)
 		{
 			return boost::locale::conv::to_utf<wchar_t>(p,charset());
 		}
 
 		static wstring utf8_to_utf32(const char* p)
 		{
 			return boost::locale::conv::utf_to_utf<wchar_t>(p);
 		}
 
 		static string utf32_to_utf8(const wchar_t* p)
 		{
 			return boost::locale::conv::utf_to_utf<char>(p);
 		}
 #endif

		static std::basic_string<unsigned short> utf8_to_utf16_v2(const char* p)
		{
			return boost::locale::conv::utf_to_utf<unsigned short, char>(p);
		}

		static string to_utf8(const char* p)
		{
			return boost::locale::conv::to_utf<char>(p,charset());
		}

		static string to_utf8(const char* p, int len)
		{
			return boost::locale::conv::to_utf<char>(p, p+len, charset());
		}		

		static int enum_directory( const string& full_path, vector<string>& files )
		{
			boost::filesystem::directory_iterator end;
			boost::system::error_code err;
			boost::filesystem::directory_iterator pos(full_path, err);
			if (err)
			{
				cout<<err<<endl;
				return 0;
			}
			for(; pos !=end; ++pos)
			{
				boost::filesystem::path temp_path(*pos);
				if(boost::filesystem::is_regular(temp_path,err))
				{
					if (err)
					{
						cout<<err<<endl;
						return 0;
					}
					string str= temp_path.string();
					files.push_back(str);
				}
				else if(boost::filesystem::is_directory(temp_path,err))
				{
					if (err)
					{
						cout<<err<<endl;
						return 0;
					}
					enum_directory(temp_path.string(), files);
				}
			}
			return files.size();
		}
		static bool is_utf8(const char *data, size_t size)
		{
			const unsigned char *str = (unsigned char*)data;
			const unsigned char *end = str + size;
			unsigned char byte;
			unsigned int code_length, i;
			unsigned int ch;
			while (str != end) {
				byte = *str;
				if (byte <= 0x7F) {
					/* 1 byte sequence: U+0000..U+007F */
					str += 1;
					continue;
				}

				if (0xC2 <= byte && byte <= 0xDF)
					/* 0b110xxxxx: 2 bytes sequence */
					code_length = 2;
				else if (0xE0 <= byte && byte <= 0xEF)
					/* 0b1110xxxx: 3 bytes sequence */
					code_length = 3;
				else if (0xF0 <= byte && byte <= 0xF4)
					/* 0b11110xxx: 4 bytes sequence */
					code_length = 4;
				else {
					/* invalid first byte of a multibyte character */
					return false;
				}

				if (str + (code_length - 1) >= end) {
					/* truncated string or invalid byte sequence */
					return false;
				}

				/* Check continuation bytes: bit 7 should be set, bit 6 should be
				 * unset (b10xxxxxx). */
				for (i=1; i < code_length; i++) {
					if ((str[i] & 0xC0) != 0x80)
						return false;
				}

				if (code_length == 2) {
					/* 2 bytes sequence: U+0080..U+07FF */
					ch = ((str[0] & 0x1f) << 6) + (str[1] & 0x3f);
					/* str[0] >= 0xC2, so ch >= 0x0080.
					   str[0] <= 0xDF, (str[1] & 0x3f) <= 0x3f, so ch <= 0x07ff */
				} else if (code_length == 3) {
					/* 3 bytes sequence: U+0800..U+FFFF */
					ch = ((str[0] & 0x0f) << 12) + ((str[1] & 0x3f) << 6) +
						  (str[2] & 0x3f);
					/* (0xff & 0x0f) << 12 | (0xff & 0x3f) << 6 | (0xff & 0x3f) = 0xffff,
					   so ch <= 0xffff */
					if (ch < 0x0800)
						return false;

					/* surrogates (U+D800-U+DFFF) are invalid in UTF-8:
					   test if (0xD800 <= ch && ch <= 0xDFFF) */
					if ((ch >> 11) == 0x1b)
						return false;
				} else if (code_length == 4) {
					/* 4 bytes sequence: U+10000..U+10FFFF */
					ch = ((str[0] & 0x07) << 18) + ((str[1] & 0x3f) << 12) +
						 ((str[2] & 0x3f) << 6) + (str[3] & 0x3f);
					if ((ch < 0x10000) || (0x10FFFF < ch))
						return false;
				}
				str += code_length;
			}
			return true;
		}

		static FILE* file_open(const char* path,const char* mode, int encoding = FILEPATH_ENCODING_UTF8)
		{
			FILE* fp = NULL;
#ifdef WIN32
			if (encoding == FILEPATH_ENCODING_UTF8)
			{
				fp = _wfopen(utf8_to_utf16(path).c_str(), utf8_to_utf16(mode).c_str());
			}
			else if (encoding == FILEPATH_ENCODING_ASCII)
			{
				fp = fopen(path, mode);
			}
#else
			fp = fopen(path, mode);
#endif
			return fp;
		}
		static int remove_corrupt_tail(string &strutf8)
		{
			int count = 0;
			int flags = 0;
			int i = 0;
			if (strutf8.length() < 3)
			{
				return 0;
			}
			try{
				for(i=1;i<4;++i)
				{
					char c = strutf8.at(strutf8.length()-i);
					if (0x00 == (c&0x80))
					{
						break;
					}
					if (0x80 == (c&0xc0))
					{
						++count;
						continue;
					}
					if (0xc0 == (c&0xe0))
					{
						if (1 != count)
						{
							flags = 1;
						}
						break;
					}
					if (0xe0 == (c&0xf0))
					{
						if (2 != count)
						{	
							flags = 1;
						}
						break;
					}
					flags = 1;
					break;
				}

				if (1 == flags)
				{
					strutf8 = strutf8.substr(0,strutf8.length()-i);
					return 1;
				}
			}
			catch(...){}
			
			return 0;
		}
		static void get_utf8_length(const char *str, int *cnt)
		{
			while (*str != '\0'){
				if (*str & 1 << 7){/* 第8位(从右到左)为1*/
					if (*str & (1 << 6)){/*第7位为1*/
						if (*str & (1 << 5)){/*第6位为1*/
							if (*str & (1 << 4)){/*第5位为1  11110xxx*/
								cnt[4]++, cnt[0]++, str += 4;/*4字节的字符总数加1,各种长度总数加1,指针向后移动4位*/
								continue;/*开始下次循环*/
							}

							cnt[3]++, cnt[0]++, str += 3;/* 1110xxxx*/
							continue;

						}

						cnt[2]++, cnt[0]++, str += 2;/*110xxxxx*/
						continue;

					}
				}

				cnt[1]++, cnt[0]++, str += 1;/*0xxxxxxx*/
				continue;

			}
		}
	};
}

#define FILEOPEN(path,mode) keyword_search::util::file_open(path,mode)

#define TO_UTF8(szsrc) keyword_search::util::to_utf8(szsrc)
#define FROM_UTF8(szsrc) keyword_search::util::from_utf8(szsrc)

#ifdef WIN32
#define TO_UTF16(szsrc) keyword_search::util::to_utf16(szsrc)
#define FROM_UTF16(szsrc) keyword_search::util::from_utf16(szsrc)
#define UTF8_TO_UTF16(szsrc) keyword_search::util::utf8_to_utf16(szsrc)
#define UTF16_TO_UTF8(szsrc) keyword_search::util::utf16_to_utf8(szsrc)
#else
#define FROM_UTF32(szsrc) keyword_search::util::from_utf32(szsrc)
#define TO_UTF32(szsrc) keyword_search::util::to_utf32(szsrc)
#define UTF8_TO_UTF32(szsrc) keyword_search::util::utf8_to_utf32(szsrc)
#define UTF32_TO_UTF8(szsrc) keyword_search::util::utf32_to_utf8(szsrc)
#endif

#ifdef WIN32
#define UTF8_TO_WIDECHAR UTF8_TO_UTF16
#define WIDECHAR_TO_UTF8 UTF16_TO_UTF8
#define FROM_WIDECHAR FROM_UTF16
#else
#define UTF8_TO_WIDECHAR UTF8_TO_UTF32
#define WIDECHAR_TO_UTF8 UTF32_TO_UTF8
#define FROM_WIDECHAR FROM_UTF32
#endif

#endif
