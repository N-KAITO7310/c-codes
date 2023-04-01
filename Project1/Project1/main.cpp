#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnPlugin.h>

class AddNode : public MPxNode
{
public:
	AddNode();
	virtual ~AddNode();
	static void* creator();
	static MStatus initialize();
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);

public:
	static MTypeId id;
	static MObject input1;
	static MObject input2;
	static MObject output;
};

MTypeId AddNode::id(0x80002);
MObject AddNode::input1;
MObject AddNode::input2;
MObject AddNode::output;

AddNode::AddNode()
{

}

AddNode::~AddNode()
{

}

void* AddNode::creator()
{
	return new AddNode();
}

// Attribute“o˜^
MStatus AddNode::initialize()
{
	MFnNumericAttribute nAttr;
	input1 = nAttr.create("input1", "in1", MFnNumericData::kDouble, 0.0);
	nAttr.setWritable(true);

	input2 = nAttr.create("input2", "in2", MFnNumericData::kDouble, 0.0);
	nAttr.setWritable(true);

	output = nAttr.create("output", "out", MFnNumericData::kDouble, 0.0);
	nAttr.setWritable(false);
	nAttr.setStorable(false);

	addAttribute(input1);
	addAttribute(input2);
	addAttribute(output);

	// input1 + input2 = output
	attributeAffects(input1, output);
	attributeAffects(input2, output);

	return MS::kSuccess;
}

// ƒm[ƒh‚ÌU‚é•‘‚¢‚ğì‚é
MStatus AddNode::compute(const MPlug& plug, MDataBlock& data)
{
	if (plug != output) {
		return MS::kUnknownParameter;
	}
	double input1_data = data.inputValue(input1).asDouble();
	double input2_data = data.inputValue(input2).asDouble();
	double output_value = input1_data + input2_data;
	data.outputValue(output).set(output_value);
	data.setClean(plug);
	return MS::kSuccess;
}

// ƒm[ƒh‚ğ“o˜^‚·‚é
MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, "TECH ART ONLINE", "1.0", "Any");
	status = plugin.registerNode("HelloAddNode", AddNode::id, AddNode::creator, AddNode::initialize);
	return status;
}

// ƒm[ƒh‚Ì“o˜^‚ğ‰ğœ‚·‚é
MStatus uninitalizePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj);
	status = plugin.deregisterNode(AddNode::id);
	return status;
}