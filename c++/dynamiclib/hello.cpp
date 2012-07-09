#include <iostream>
#include "plugin.h"

class Hello : public Plugin {
    public:
        virtual const void Run() const;

		Hello( const string& name = "Hello" ) : Plugin( name ) {};
};

const void Hello::Run() const
{
    cout << "Hello!" << endl;
    return;
}

extern "C" {
    Plugin* New() { return new Hello(); }
    void Delete( Plugin* p ) { delete p; }
}

