#include <maya/MPxContextCommand.h>


class SimpleContextCmd : public MPxContextCommand
{
public:
    SimpleContextCmd();
    virtual ~SimpleContextCmd() override;

    virtual MPxContext* makeObj() override;


    // Static methods
    static void* Creator();

    static MString GetCommandName();

};
