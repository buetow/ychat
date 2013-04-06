#include "../incl.h"

#ifdef OPENSSL
#ifndef SSLSOCK_H
#define SSLSOCK_H

#include "sock.h"

#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

using namespace std;

class sslsock : public sock
{
private:
  SSL_CTX* p_ctx;
  string s_certificate_path;
  string s_privatekey_path;
  map<int,void*> map_certs;

public:

  sslsock( );
  ~sslsock( );

  int _send(socketcontainer *p_sock, const char *sz, int len);
  int _read(socketcontainer *p_sock, char *sz, int len);
  int _close(socketcontainer *p_sock);
  void _main_loop_init();
  bool _main_loop_do_ssl_stuff(int &i_new_sock);
  socketcontainer* _create_container(int& i_sock);
  int _make_server_socket(int i_port);
};

#endif
#endif
