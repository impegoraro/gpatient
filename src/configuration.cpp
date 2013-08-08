#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif
#include <string>
#include <cstdio>
#include <fstream>
#include <glibmm/miscutils.h>
#include <glibmm/ustring.h>
#include <json/value.h>
#include <json/json.h>

#include "configuration.h"

Configuration::Configuration()
{
	Glib::ustring path;
	//Glib::RefPtr<Gio::File> fdb;
	
	path = Glib::get_home_dir();
	path += "/." + (Glib::ustring)PACKAGE_NAME + (Glib::ustring)"/" + ((Glib::ustring)PACKAGE_NAME) + ".conf";
	m_fpath = path.lowercase();
}

Configuration::Configuration(const std::string& fpath) : m_fpath(fpath)
{
}

void Configuration::store()
{
	Json::Value root;
	Json::StyledWriter writer;
	std::string tmp;
	std::ofstream fstream;
	int posx(0), posy(0), width(0), height(0);

	get_window_position(posx, posy);
	get_window_size(width, height);

	root["Position"][(Json::UInt)0] = posx;
	root["Position"][(Json::UInt)1] = posy;
	root["Size"][(Json::UInt)0] = width;
	root["Size"][(Json::UInt)1] = height;
	root["Maximized"] = get_window_maximized();
	
	tmp = writer.write(root);
	fstream.open(m_fpath);
	fstream<< tmp;
	fstream.close();
	//std::cout<< tmp<<std::endl;
}

bool Configuration::parse()
{
	bool parsing;
	std::string text;
	Json::Value root;
	Json::Value jval;
	Json::Features features;
	Json::Reader reader(features);
	Json::Value jdefault;
	int posx(0), posy(0), width(860), height(640), maximized(0);

	text = read_input_file();
	parsing = reader.parse(text, root);
	if(root.empty()) {
		std::cout<<"There're no configuration options in the file"<<std::endl;
		return false;
	}
	if(!parsing) {
		//std::cout<< "Could not read from configurations from file: '"<< m_fpath<< "'." << std::endl<< reader.getFormattedErrorMessages()<< std::endl;
		std::cout<< "Could not read from configurations from file: '"<< m_fpath<< "'." << std::endl;
		return false;
	}

	// get the configurations

	maximized = root.get("Maximized", false).asBool();

	jval = root["Position"];
	if(!jval && jval.size() == 2){
		if(jval[(Json::UInt)0].isInt() && jval[(Json::UInt)1].isInt()) {
			posx = jval[(Json::UInt)0].asInt();
			posy = jval[(Json::UInt)1].asInt();
		}
	}
	
	jval = root["Size"];
	if(!jval.isNull() && jval.size() == 2) {
		if(jval[(Json::UInt)0].isInt() && jval[(Json::UInt)1].isInt()) {
			width = jval[(Json::UInt)0].asInt();
			height = jval[(Json::UInt)1].asInt();
		}
	}

	set_window_resize(width, height);
	//set_window_move(posx, posy);
	set_window_maximized(maximized);
	return true;
}

std::string Configuration::read_input_file()
{
   FILE *file = fopen( m_fpath.c_str(), "rb" );
   if ( !file )
      return std::string("");
   fseek( file, 0, SEEK_END );
   long size = ftell( file );
   fseek( file, 0, SEEK_SET );
   std::string text;
   char *buffer = new char[size+1];
   buffer[size] = 0;
   if ( fread( buffer, 1, size, file ) == (unsigned long)size )
      text = buffer;
   fclose( file );
   delete[] buffer;
   return text;
}
