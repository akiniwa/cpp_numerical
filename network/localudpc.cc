// LOCAL で DGRAM なソケットによる通信プログラムの雛型 (クライアント側)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "integnet.h"

// 接続の目印となるファイル
#define SOCKNAME "/tmp/udsock"

int main( void )
{
  // LOCAL で DGRAM なソケットの作成
  int s = socket( AF_LOCAL, SOCK_DGRAM, 0 );

  // 送信するデータ
  char msg[64] = "This is client.";

  // 通信開始のための目印の指定
  sockaddr_un addr = SOCKADDR_UN_INIT( AF_LOCAL, msg);

  // データの送信
  sendto( s, msg, strlen(msg), 0, (sockaddr*)&addr, sizeof(addr) );

  // ソケットの廃止
  close(s);

  return 0;
}
