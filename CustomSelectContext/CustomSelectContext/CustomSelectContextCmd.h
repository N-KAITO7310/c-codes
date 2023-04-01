#include <maya/MPxContextCommand.h>


class CustomSelectContextCmd : public MPxContextCommand
{
public:
    CustomSelectContextCmd();
    virtual ~CustomSelectContextCmd() override;

    virtual MPxContext* makeObj() override;


    // Static Methods
    static void* Creator();

    static MString GetCommandName();
};
