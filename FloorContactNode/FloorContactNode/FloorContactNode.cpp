#include "FloorContactNode.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MGlobal.h>
#include <math.h>


//-----------------------------------------------------------------------------
// CONSTANTS
//-----------------------------------------------------------------------------
static const MTypeId TYPE_ID = MTypeId(0x0007F7C1);
static const MString TYPE_NAME = "floorContactNode";


//-----------------------------------------------------------------------------
// STATIC VARIABLES
//-----------------------------------------------------------------------------
MObject FloorContactNode::radiusAttr;
MObject FloorContactNode::source1MatrixAttr;
MObject FloorContactNode::source2MatrixAttr;
MObject FloorContactNode::ctrlMatrixAttr;
MObject FloorContactNode::normalAttr;
MObject FloorContactNode::outputTranslateAttr;

//-----------------------------------------------------------------------------
// PUBLIC METHODS
//-----------------------------------------------------------------------------
FloorContactNode::FloorContactNode() :
	MPxNode()
{
}

FloorContactNode::~FloorContactNode()
{
}

MStatus FloorContactNode::compute(const MPlug& plug, MDataBlock& data)
{
	if (plug == outputTranslateAttr)
	{
		// get incoming data
		double radius = data.inputValue(radiusAttr).asDouble();
		MMatrix source1Matrix = data.inputValue(source1MatrixAttr).asMatrix();
		MMatrix source2Matrix = data.inputValue(source2MatrixAttr).asMatrix();
		MMatrix ctrlMatrix = data.inputValue(ctrlMatrixAttr).asMatrix();
		MVector normalVec = MVector(data.inputValue(normalAttr).asFloat3());

		// compute
		// convert vector
		MTransformationMatrix source1TMat(source1Matrix);
		MVector source1Vec = MVector(source1TMat.getTranslation(MSpace::kWorld));

		MTransformationMatrix source2TMat(source2Matrix);
		MVector source2Vec = MVector(source2TMat.getTranslation(MSpace::kWorld));

		MTransformationMatrix ctrlTMat(ctrlMatrix);
		MVector ctrlVec = MVector(ctrlTMat.getTranslation(MSpace::kWorld));

		// culculate
		// offset length
		MVector parentToChildVec = source1Vec - source2Vec;
		double parentToChildDist = parentToChildVec.length();

		// multiply normal vector
		MVector normalCtrlProdVec = (normalVec * ctrlMatrix).normal();

		// collision iteration�@�v�Z�C�e���[�V�����̉񐔐ݒ�H���x�ɉe���H
		for(int i = 0; i < 100; i++)
		{
			// plane collision
			// �h���C�o�v���[������h���u���I�u�W�F�N�g�ւ̋����x�N�g��
			MVector distVec = source2Vec - ctrlVec;
			// �v���[���̃m�[�}���ƁA�����x�N�g���Ƃ̓��ρ@�����ς͐����̎���0
			double distancePointPlane = normalCtrlProdVec * distVec;

			// ���ς��甼�a���������l��0��菭�Ȃ��� = �h���u�������͈͂ɏՓ˂����Ă���Ƃ� =�@�h���u���ƃh���C�o�v���[�������a���l�����������Ő����ɋ߂���
			if (distancePointPlane - radius < 0)
			{
				// (�v���[���̃m�[�}���ƁA�����x�N�g���Ƃ̓���)���甼�a���������l
				double distancePointPlaneOffsetedRadius = distancePointPlane - radius;
				// �h���u���̏����ʒu�@�|�@���ʃI�u�W�F�N�g�̃m�[�}���x�N�g���~���a���l���������ρ@��F�����ʒu�x�T - �x�N�g��Y�P * -0.5 = 5.5
				source2Vec = source2Vec - (normalCtrlProdVec * distancePointPlaneOffsetedRadius);
			}

			// keep length unit():�w�肳�ꂽ�x�N�g���Ɠ����������w���}�O�j�`���[�h1�̃x�N�g�� = ��������̃x�N�g�����A���a(�Ε�)1�̓��S�~��Ő؂������ꍇ�̈ʒu ��Funit(<<1,1,1>>)=<<0.577, 0.577, 0.577>>
			// �e�I�u�W�F�N�g�ʒu + (�P�ʃx�N�g���ɐ��K�����ꂽ�x�N�g��(�v�Z���ʈʒu-�e�I�u�W�F�N�g�ʒu) * �h���u���I�u�W�F�N�g�̐e����̌��X�̃I�t�Z�b�g����)
			// �e�I�u�W�F�N�g���I�t�Z�b�g���A�ړ����ׂ���������ւ̃x�N�g���𐳋K��(��������)�B���K���������̃x�N�g���Ɍ��X�̃I�t�Z�b�g��l�������� = �Ⴆ�΃W���C���g�Ȃ�A���̌��X�̋����𒴂��邱�Ƃ͂Ȃ�
			// ���ӁF�e�E�q�E�R���g���[����y���ň꒼����ɑ��݂���ꍇ�AnormalizedVec��(0, 1, 0)�ƂȂ�v�Z���ʂ͌���sourceVec2�Ɠ����ɂȂ遁�ړ����Ȃ�
			MVector normalizedVec = (source2Vec - source1Vec).normal();
			source2Vec = source1Vec + (normalizedVec * parentToChildDist);
		}
		
		data.outputValue(outputTranslateAttr).set3Float(source2Vec.x, source2Vec.y, source2Vec.z);

		data.setClean(plug);
		return(MS::kSuccess);
	}
}


//-----------------------------------------------------------------------------
// STATIC METHODS
//-----------------------------------------------------------------------------
void* FloorContactNode::Creator()
{
	return(new FloorContactNode());
}

MStatus FloorContactNode::Initialize()
{
	// input
	MFnNumericAttribute numericAttr;
	radiusAttr = numericAttr.create("radius", "rad", MFnNumericData::kDouble, 0.0);
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);

	MFnMatrixAttribute matrixAttr;
	source1MatrixAttr = matrixAttr.create("source1Matrix", "s1Mat", MFnMatrixAttribute::kDouble);
	matrixAttr.setKeyable(true);
	matrixAttr.setReadable(false);

	source2MatrixAttr = matrixAttr.create("source2Matrix", "s2Mat", MFnMatrixAttribute::kDouble);
	matrixAttr.setKeyable(true);
	matrixAttr.setReadable(false);

	ctrlMatrixAttr = matrixAttr.create("ctrlMatrix", "cMat", MFnMatrixAttribute::kDouble);
	matrixAttr.setKeyable(true);
	matrixAttr.setReadable(false);

	normalAttr = numericAttr.createPoint("normal", "nr");
	matrixAttr.setKeyable(true);
	matrixAttr.setReadable(false);

	// output
	outputTranslateAttr = numericAttr.createPoint("outputTranslate", "ot");
	numericAttr.setWritable(false);
	numericAttr.setStorable(false);

	addAttribute(radiusAttr);
	addAttribute(source1MatrixAttr);
	addAttribute(source2MatrixAttr);
	addAttribute(ctrlMatrixAttr);
	addAttribute(normalAttr);
	addAttribute(outputTranslateAttr);

	attributeAffects(radiusAttr, outputTranslateAttr);
	attributeAffects(source1MatrixAttr, outputTranslateAttr);
	attributeAffects(source2MatrixAttr, outputTranslateAttr);
	attributeAffects(normalAttr, outputTranslateAttr);
	attributeAffects(ctrlMatrixAttr, outputTranslateAttr);

	return(MS::kSuccess);
}

MTypeId FloorContactNode::GetTypeId()
{
	return(TYPE_ID);
}

MString FloorContactNode::GetTypeName()
{
	return(TYPE_NAME);
}
