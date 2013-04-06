#include "../incl.h"

#ifdef OPENSSL
#ifndef SSLSOCK_CPP
#define SSLSOCK_CPP

#include "sslsock.h"

using namespace std;

sslsock::sslsock() : sock()
{
  s_certificate_path = wrap::CONF->get_elem( "httpd.ssl.certificatepath" );
  s_privatekey_path = wrap::CONF->get_elem( "httpd.ssl.privatekeypath" );
  p_ctx = NULL;
}

int
sslsock::_send(socketcontainer *p_sock, const char *sz, int len)
{
  return SSL_write((SSL*)p_sock->p_ssl_context,sz, len);
}

int
sslsock::_read(socketcontainer *p_sock,  char *sz, int len)
{
  return SSL_read((SSL*)p_sock->p_ssl_context,sz,len);
}

int
sslsock::_close(socketcontainer *p_sock)
{
  SSL_free((SSL*)p_sock->p_ssl_context);
  sock::_close(p_sock);
}

int
sslsock::_make_server_socket(int i_port)
{
  SSL_METHOD *p_ssl_method;
  unsigned long e;
  char sz[1024];
  string s_error;

  int i_sock = sock::_make_server_socket(i_port);

  if(i_sock <= 0)
  {
    wrap::system_message(SSLERR1);
    return -1;
  }

  SSL_load_error_strings();
  SSLeay_add_ssl_algorithms();

  p_ssl_method = SSLv23_server_method();
  p_ctx = SSL_CTX_new (p_ssl_method);
  if (!p_ctx)
  {
    e = ERR_get_error();
    ERR_error_string_n(e, sz, sizeof(sz) - 1);
    s_error = sz;
    wrap::system_message(SSLERR1);
    return -1;
  }

  if (SSL_CTX_use_certificate_file(p_ctx, s_certificate_path.c_str(), SSL_FILETYPE_PEM) <= 0)
  {
    e = ERR_get_error();
    ERR_error_string_n(e, sz, sizeof(sz) - 1);
    s_error = sz;
    wrap::system_message(SSLERR1);
    return -1;
  }

  if (SSL_CTX_use_PrivateKey_file(p_ctx, s_privatekey_path.c_str(), SSL_FILETYPE_PEM) <= 0)
  {
    e = ERR_get_error();
    ERR_error_string_n(e, sz, sizeof(sz) - 1);
    s_error = sz;
    wrap::system_message(SSLERR1);
    return -1;
  }

  if (!SSL_CTX_check_private_key(p_ctx))
  {
    wrap::system_message(SSLERR2);
    return -1;
  }

  return i_sock;
}

void
sslsock::_main_loop_init()
{
  wrap::system_message(SOCKSEC);
}

bool
sslsock::_main_loop_do_ssl_stuff(int& i_new_sock)
{
  SSL* p_ssl = SSL_new(p_ctx);

  if( p_ssl == NULL || i_new_sock < 0)
  {
    wrap::system_message(SSLERR3);

    close(i_new_sock);
    if(p_ssl != NULL)
      SSL_free(p_ssl);

    return 1;
  }

  else
  {
    SSL_set_fd(p_ssl, i_new_sock);
    if(SSL_accept(p_ssl) == -1)
    {
      wrap::system_message(SSLERR4);
      close(i_new_sock);
      return 1;
    }

    map_certs[i_new_sock] = p_ssl;
  }

  return 0;
}

socketcontainer*
sslsock::_create_container(int &i_sock)
{
  socketcontainer* p_sock = sock::_create_container(i_sock);
  p_sock->p_ssl_context = map_certs[i_sock];
  return p_sock;
}

#endif
#endif
