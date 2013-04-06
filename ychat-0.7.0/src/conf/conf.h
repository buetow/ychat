// class conf declaration. this class parses the server config file.

#ifndef CONF_H
#define CONF_H

class conf;

#include "../incl.h"
#include "../maps/nmap.h"
#include "../name.h"
#include "../contrib/xml/tinyxml.h"

using namespace std;

class conf : public nmap<string,string>, name
{
private:
    TiXmlDocument* p_xml;
    void exit_if_xml_error() const;
    void parse_xml( TiXmlNode* p_node, vector<string>* p_vec);
public:
    //conf ( string s_conf );
    conf ( string s_conf, map<string,string>* p_start_params );
    ~conf();
};

#endif
