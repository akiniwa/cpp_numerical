// LOCAL �� DGRAM �ʥ����åȤˤ���̿��ץ����ο��� (���饤�����¦)

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

  // ��������ǡ���
  char msg[64] = "This is client.";

  // �̿����ϤΤ�����ܰ��λ���
  sockaddr_un addr = SOCKADDR_UN_INIT( AF_LOCAL, msg);

  // �ǡ���������
  sendto( s, msg, strlen(msg), 0, (sockaddr*)&addr, sizeof(addr) );

  // �����åȤ��ѻ�
  close(s);

  return 0;
}
