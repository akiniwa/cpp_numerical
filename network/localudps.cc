// LOCAL で DGRAM なソケットによる通信プログラムの雛型 (サーバー側)

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

  // 接続の目印の指定
  sockaddr_un addr = SOCKADDR_UN_INIT( AF_LOCAL, SOCKNAME );

  // 古い目印の削除(もし残っていると厄介)
  unlink( SOCKNAME );

  // 目印の公開
  bind( s, (sockaddr*)&addr, sizeof(addr) );

  // データの受信
  char msg[64];
  int len;
  len = recvfrom( s, msg, sizeof(msg), 0, NULL, NULL );
  msg[len] = '\0';

  // データの表示
  printf("Server received : %s\n", msg );

  // ソケットの廃止
  close(s);

  // 目印の削除(義務ではないが礼儀)
  unlink( SOCKNAME );

  return 0;
}
