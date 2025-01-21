/****************************************************************************
 Configuration : handling configuration files
****************************************************************************/

#include "init.h"
#include "Configuration.h"

Configuration::Configuration()
{

}

void Configuration::Clear()
{
    data.clear();
}

bool Configuration::Load(const string& file)
{
    ifstream inFile(file.c_str());

    if (!inFile.good())
    {
        cout << "Cannot read configuration file " << file << endl;
        return false;
    }

    while (inFile.good() && ! inFile.eof())
    {
        string line;
        getline(inFile, line);

        // filter out comments
        if (!line.empty())
        {
            int pos = line.find('#');

            if (pos != string::npos)
            {
                line = line.substr(0, pos);
            }
        }

        // split line into key and value
        if (!line.empty())
        {
            int pos = line.find('=');

            if (pos != string::npos)
            {
                string key     = Trim(line.substr(0, pos));
                string value   = Trim(line.substr(pos + 1));

                if (!key.empty() && !value.empty())
                {
                    data[key] = value;
                }
            }
        }
    }

    return true;
}

bool Configuration::Contains(const string& key) const
{
    return data.find(key) != data.end();
}

bool Configuration::Get(const string& key, string& value) const
{
    map<string,string>::const_iterator iter = data.find(key);

    if (iter != data.end())
    {
        value = iter->second;
        return true;
    }
    else
    {
        return false;
    }
}

bool Configuration::Get(const string& key, int& value) const
{
    string str;

    if (Get(key, str))
    {
        value = atoi(str.c_str());
        return true;
    }
    else
    {
        return false;
    }
}

bool Configuration::Get(const string& key, long& value) const
{
    string str;

    if (Get(key, str))
    {
        value = atol(str.c_str());
        return true;
    }
    else
    {
        return false;
    }
}

bool Configuration::Get(const string& key, double& value) const
{
    string str;

    if (Get(key, str))
    {
        value = atof(str.c_str());
        return true;
    }
    else
    {
        return false;
    }
}

bool Configuration::Get(const string& key, float& value) const
{
    string str;

    if (Get(key, str))
    {
        value = atof(str.c_str());
        return true;
    }
    else
    {
        return false;
    }
}


bool Configuration::Get(const string& key, bool& value) const
{
    string str;

    if (Get(key, str))
    {
        value = (str == "true");
        return true;
    }
    else
    {
        return false;
    }
}

string Configuration::Trim(const string& str)
{
    int first = str.find_first_not_of(" \t");

    if (first != string::npos)
    {
        int last = str.find_last_not_of(" \t");

        return str.substr(first, last - first + 1);
    }
    else
    {
        return "";
    }
}


void Configuration :: ReadConfigIm()
{

	if(!Get("im_dir", imdir))
	{
		cout << "Problem to get imdir (method read_param in ImProcTools.cpp), check .cfg file "  << endl;
		imdir = "im\\";
	}
	if(!Get("im_name", imname))
	{
		cout << "Problem to get imname, check .cfg  file" << endl;
		imname = "";
	}
	if(!Get("im_ext", imext))
	{
		cout << "Problem to get imext, check  .cfg  file" << endl;
		imext = ".png";
	}
	if(!Get("Ninit", n0))
	{
		cout << "Problem to get Ninit, check .cfg file " << endl;
		n0 = 1;
	}
	if(!Get("Nend", nend))
	{
		cout << "Problem to get nend, check .cfg file " << endl;
		nend = 1;
	}
	if(!Get("format", format))
	{
		cout << "Problem to get format, check .cfg  file " << endl;
		format = 2;
	}
	if(!Get("scale", scale))
	{
		//cout << "Problem to get scale , check .cfg  " << endl;
		scale =1;
	}

	if(!Get("colorspace", colorspace))
	{
		//cout << "Problem to get colorspace , check .cfg  " << endl;
		colorspace = "RGB";
	}
    if(!Get("channel", channel))
	{
		//cout << "Problem to get channel , check .cfg  " << endl;
		channel = 0;
	}

}


void Configuration :: ReadConfigStats()
{
	if(!Get("channel0", channel0))
	{
		cout << "Problem to get channel0, check  .cfg. Default value 0 is taken.  "  << endl;
		channel0 = 0;
	}
    if(!Get("channel1", channel1))
	{
		cout << "Problem to get channel1, check  .cfg. Default value 1 is taken.  "  << endl;
		channel1 = 1;
	}

}

void Configuration :: ReadConfigCluster()
{
	if(!Get("K", K))
	{
		cout << "Problem to get K check .cfg  file"  << endl;
		K = 3;
	}


}
void Configuration :: ReadConfigTexture()
{
	if(!Get("W", W))
	{
		cout << "Problem to get W, check .cfg file "  << endl;
		W = 7;
	}


}

void Configuration :: ReadConfigFilter()
{
	if(!Get("Fb", Fb))
	{
		cout << "Problem to get Fb, check .cfg file "  << endl;
		Fb = 10;
	}
	if(!Get("Fh", Fh))
	{
		cout << "Problem to get Fh, check .cfg file "  << endl;
		Fh = 10;
	}

}

