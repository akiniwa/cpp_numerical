#include <iostream>
#include <string>
// myheader.h
#ifndef __MYHEADER_H_INCLUDE
#define __MYHEADER_H_INCLUDE

//元祖「猫」
class Neko
{
		string name;
		public:
		Neko(string s);   //「Neko(string);」でも可
		void naku() const;
};
#endif  //__MYHEADER_H_INCLUDE
