// ネットワークプログラミングを便利にするヘッダーファイル
//
#ifndef __INTEGNET_H_INCLUDED
#define __INTEGNET_H_INCLUDED

#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// ホスト名を in_addr型のIPアドレスに変換する関数
in_addr InAddr( const char* hostname )
{
  hostent* hp;
  in_addr sin_addr = {0};

  if( (hp = gethostbyname( hostname )) != NULL ){
    sin_addr = *(in_addr*) hp->h_addr;
  }
  return sin_addr;
}

// in_addr_t型のIPアドレスをin_addr型に変換する関数
in_addr InAddr( in_addr_t s_addr )
{
  return *(in_addr*) &s_addr;
}


// AF_LOCALが定義されていないOSでもそれを使えるようにする。
#ifndef AF_LOCAL
  #define AF_LOCAL AF_UNIX
#endif

// sockaddr_un型構造体の初期化用マクロ
// 使用例
// sockaddr_un addr = SOCKADDR_UN_INIT(AF_LOCAL,"/tmp/udsock");
//
#ifdef __FreeBSD__
  #define SOCKADDR_UN_INIT(AF,PATH) {0,AF,PATH}
#else
  #define SOCKADDR_UN_INIT(AF,PATH) {AF,PATH}
#endif

// sockaddr_in型構造体の初期化用マクロ
// 使用例
// sockaddr_in addr = SOCKADDR_IN_INIT(AF_INET,htons(80),InAddr("127.0.0.1"));
//
#ifdef __FreeBSD__
  #define SOCKADDR_IN_INIT(AF,PORT,IP) {0,AF,PORT,IP}
#else
  #define SOCKADDR_IN_INIT(AF,PORT,IP) {AF,PORT,IP}
#endif

// HP-UXでは recvfromの最後の引数にNULLを与えてはならない
#ifdef _INCLUDE_HPUX_SOURCE
  int ZERO = 0;
  #define recvfrom(A0,A1,A2,A3,A4,A5) \
    (A5==NULL) ? recvfrom(A0,A1,A2,A3,A4,&ZERO) : recvfrom(A0,A1,A2,A3,A4,A5)
#endif

#endif // __INTEGNET_H_INCLUDED
