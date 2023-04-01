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

		// collision iteration　計算イテレーションの回数設定？精度に影響？
		for(int i = 0; i < 100; i++)
		{
			// plane collision
			// ドライバプレーンからドリブンオブジェクトへの距離ベクトル
			MVector distVec = source2Vec - ctrlVec;
			// プレーンのノーマルと、距離ベクトルとの内積　＊内積は垂直の時に0
			double distancePointPlane = normalCtrlProdVec * distVec;

			// 内積から半径を引いた値が0より少ない時 = ドリブンが持つ範囲に衝突をしているとき =　ドリブンとドライバプレーンが半径を考慮したうえで垂直に近い時
			if (distancePointPlane - radius < 0)
			{
				// (プレーンのノーマルと、距離ベクトルとの内積)から半径を引いた値
				double distancePointPlaneOffsetedRadius = distancePointPlane - radius;
				// ドリブンの初期位置　－　平面オブジェクトのノーマルベクトル×半径を考慮した内積　例：初期位置Ｙ５ - ベクトルY１ * -0.5 = 5.5
				source2Vec = source2Vec - (normalCtrlProdVec * distancePointPlaneOffsetedRadius);
			}

			// keep length unit():指定されたベクトルと同じ方向を指すマグニチュード1のベクトル = 特定方向のベクトルを、半径(斜辺)1の同心円上で切り取った場合の位置 例：unit(<<1,1,1>>)=<<0.577, 0.577, 0.577>>
			// 親オブジェクト位置 + (単位ベクトルに正規化されたベクトル(計算結果位置-親オブジェクト位置) * ドリブンオブジェクトの親からの元々のオフセット距離)
			// 親オブジェクト分オフセットしつつ、移動すべき特定方向へのベクトルを正規化(向きだけ)。正規化したそのベクトルに元々のオフセットを値をかける = 例えばジョイントなら、その元々の距離を超えることはない
			// 注意：親・子・コントローラがy軸で一直線上に存在する場合、normalizedVecは(0, 1, 0)となり計算結果は元のsourceVec2と同じになる＝移動しない
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
