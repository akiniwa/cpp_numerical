#include "neko.h"

Neko::Neko(string s) : name(s){}

void Neko::naku() const{
		cout<<"にゃあ。俺様は"<<name<<"だ。"<<endl;
}

