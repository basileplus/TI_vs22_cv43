#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "init.h"
using namespace std;

class Configuration
{
public:
    Configuration();
    // clear all values
    void Clear();

    // load a configuration file
    bool Load(const std::string& File);

    // check if value associated with given key exists
    bool Contains(const std::string& key) const;

    // get value associated with given key
    bool Get(const std::string& key, std::string& value) const;
    bool Get(const std::string& key, int&    value) const;
    bool Get(const std::string& key, long&   value) const;
    bool Get(const std::string& key, double& value) const;
    bool Get(const std::string& key, bool&   value) const;
    bool Get(const std::string& key, float& value) const;
    void ReadConfigIm(void);
    void ReadConfigStats(void);
    void ReadConfigCluster(void);
    void ReadConfigTexture(void);
    void ReadConfigFilter(void);

	string imdir;
	string imname;
	string imext;
	int n0;
	int nend;
	int format;
	int scale;
	bool color;
	string colorspace;
	int channel; //color component
	int channel0;
	int channel1;
	int K;
	int W;
	float Fb, Fh;
	int th;

private:
    // the container
    map<string,string> data;

    // remove leading and trailing tabs and spaces
    static std::string Trim(const std::string& str);
};


#endif
