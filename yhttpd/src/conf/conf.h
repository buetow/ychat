#ifndef CONF_H
#define CONF_H

class conf; // Predefine for nmap.tmpl

#include <map>
#include "../incl.h"
#include "../maps/shashmap.h"
#include "../name.h"
#include "../contrib/xml/tinyxml.h"

using namespace std;

class conf : public shashmap<string>, name
{
private:
  TiXmlDocument* p_xml;
  void exit_if_xml_error() const;
  void parse_xml( TiXmlNode* p_node, vector<string>* p_vec);

public:
  conf(string s_conf, map<string,string>* p_main_loop_params);
  ~conf();

  int get_int(string s_key);
  vector<string> get_vector(string s_key);
};

#endif
