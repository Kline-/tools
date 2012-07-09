#ifndef PLUGIN_H
#define PLUGIN_H

using namespace std;

class Plugin {
    public:
        const string gName() const { return m_name; };
        virtual const void Run() const = 0;

    Plugin( const string& name ) { m_name = name; return; };

    private:
        string m_name;
};

typedef Plugin* NewPlugin();
typedef void DeletePlugin( Plugin* );

#endif
