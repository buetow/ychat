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
  conf(string s_conf, map<string,string>* p_start_params);
  ~conf();

  string colored_error_msg( string s_key );  //<<
};

#endif
