// �ͥåȥ���ץ���ߥ󥰤������ˤ���إå����ե�����
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

// �ۥ���̾�� in_addr����IP���ɥ쥹���Ѵ�����ؿ�
in_addr InAddr( const char* hostname )
{
  hostent* hp;
  in_addr sin_addr = {0};

  if( (hp = gethostbyname( hostname )) != NULL ){
    sin_addr = *(in_addr*) hp->h_addr;
  }
  return sin_addr;
}

// in_addr_t����IP���ɥ쥹��in_addr�����Ѵ�����ؿ�
in_addr InAddr( in_addr_t s_addr )
{
  return *(in_addr*) &s_addr;
}


// AF_LOCAL���������Ƥ��ʤ�OS�Ǥ⤽���Ȥ���褦�ˤ��롣
#ifndef AF_LOCAL
  #define AF_LOCAL AF_UNIX
#endif

// sockaddr_un����¤�Τν�����ѥޥ���
// ������
// sockaddr_un addr = SOCKADDR_UN_INIT(AF_LOCAL,"/tmp/udsock");
//
#ifdef __FreeBSD__
  #define SOCKADDR_UN_INIT(AF,PATH) {0,AF,PATH}
#else
  #define SOCKADDR_UN_INIT(AF,PATH) {AF,PATH}
#endif

// sockaddr_in����¤�Τν�����ѥޥ���
// ������
// sockaddr_in addr = SOCKADDR_IN_INIT(AF_INET,htons(80),InAddr("127.0.0.1"));
//
#ifdef __FreeBSD__
  #define SOCKADDR_IN_INIT(AF,PORT,IP) {0,AF,PORT,IP}
#else
  #define SOCKADDR_IN_INIT(AF,PORT,IP) {AF,PORT,IP}
#endif

// HP-UX�Ǥ� recvfrom�κǸ�ΰ�����NULL��Ϳ���ƤϤʤ�ʤ�
#ifdef _INCLUDE_HPUX_SOURCE
  int ZERO = 0;
  #define recvfrom(A0,A1,A2,A3,A4,A5) \
    (A5==NULL) ? recvfrom(A0,A1,A2,A3,A4,&ZERO) : recvfrom(A0,A1,A2,A3,A4,A5)
#endif

#endif // __INTEGNET_H_INCLUDED
