// LOCAL �� DGRAM �ʥ����åȤˤ���̿��ץ����ο��� (�����С�¦)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "integnet.h"

// ��³���ܰ��Ȥʤ�ե�����
#define SOCKNAME "/tmp/udsock"

int main( void )
{
  // LOCAL �� DGRAM �ʥ����åȤκ���
  int s = socket( AF_LOCAL, SOCK_DGRAM, 0 );

  // ��³���ܰ��λ���
  sockaddr_un addr = SOCKADDR_UN_INIT( AF_LOCAL, SOCKNAME );

  // �Ť��ܰ��κ��(�⤷�ĤäƤ�������)
  unlink( SOCKNAME );

  // �ܰ��θ���
  bind( s, (sockaddr*)&addr, sizeof(addr) );

  // �ǡ����μ���
  char msg[64];
  int len;
  len = recvfrom( s, msg, sizeof(msg), 0, NULL, NULL );
  msg[len] = '\0';

  // �ǡ�����ɽ��
  printf("Server received : %s\n", msg );

  // �����åȤ��ѻ�
  close(s);

  // �ܰ��κ��(��̳�ǤϤʤ����鵷)
  unlink( SOCKNAME );

  return 0;
}
