#include <maya/MPxContextCommand.h>


class JointCreateContextCmd : public MPxContextCommand
{
public:
    JointCreateContextCmd();
    virtual ~JointCreateContextCmd() override;

    virtual MPxContext* makeObj() override;


    // Static Methods
    static void* Creator();

    static MString GetCommandName();
};
