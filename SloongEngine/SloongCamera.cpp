#include "stdafx.h"
#include "SloongCamera.h"
#include "SloongMathBase.h"

using namespace SoaringLoong::Math;
using namespace SoaringLoong::Math::Vector;

SoaringLoong::Graphics3D::CCamera::CCamera()
{

}

SoaringLoong::Graphics3D::CCamera::~CCamera()
{

}


void SoaringLoong::Graphics3D::CCamera::Initialize(CAMERA_TYPE emType, const POINT4D& Position, const POINT4D& Direction, LPPOINT4D Target, LPVECTOR4D v, bool NeedTarget, double NearZ, double farZ, double FOV, double ScreenWidth, double ScreenHeight)
{
	this->emType = emType;

	// ����λ�úͳ���
	this->WorldPos = Position;
	this->Direction = Direction;

	// �������Ŀ���
	if ( Target )
	{
		this->UVNTarget = Target;
	}
	else
	{
		this->UVNTarget.Initialize(0, 0, 0);
	}

	if ( v )
	{
		this->V = v;
	}

	this->bUVNTargetNeedCompute = NeedTarget;

	// ����ü������Ļ����
	this->NearZ = NearZ;
	this->FarZ = FarZ;
	this->ScreenWidth = ScreenWidth;
	this->ScreenHeight = ScreenHeight;
	this->ScreenCenterX = (ScreenWidth - 1) / 2;
	this->ScreenCenterY = (ScreenHeight - 1) / 2;
	this->AspectRatio = ScreenWidth / ScreenHeight;
	this->FOV = FOV;
	this->ViewPlaneWidth = 2.0;
	this->ViewPlaneHeight = 2.0 / this->AspectRatio;

	// ����FOV����ƽ�����d
	this->ViewDistance = (0.5) * (this->ViewPlaneWidth) / tan(AngleToRadian(FOV / 2));

	// ���вü��涼��ԭ��
	POINT3D Pos;
	Pos.Initialize(0, 0, 0);

	// �淨��
	VECTOR3D vn;

	if ( FOV == 90.0 )
	{
		// �Ҳü���
		vn.Initialize(1, 0, -1);
		this->ClipPlaneRight.Initialize(Pos, vn, true);

		// ��ü���
		vn.Initialize(-1, 0, -1);
		this->ClipPlaneLeft.Initialize(Pos, vn, true);

		// �ϲü���
		vn.Initialize(0, 1, -1);
		this->ClipPlaneUp.Initialize(Pos, vn, true);

		// �²ü���
		vn.Initialize(0, -1, -1);
		this->ClipPlaneDown.Initialize(Pos, vn, true);
	}
	else
	{
		// �����Ұ����90�ȣ�������ĳ���ü���ķ�����ʱ����ȥ��ƽ�����ĸ������ڸ�ƽ���ϵ���������Ϊ�òü����ϵ�����������Ȼ�����ˣ�����
		// ����ķ�����vnֱ��ʹ���˽��

		double dwTempWidth = ((this->ViewPlaneWidth) / 2.0);
		// �Ҳü���
		vn.Initialize( this->ViewDistance, 0, -dwTempWidth);
		this->ClipPlaneRight.Initialize(Pos, vn, true);

		// ��ü���
		vn.Initialize( -(this->ViewDistance), 0, -dwTempWidth);
		this->ClipPlaneLeft.Initialize(Pos, vn, true);

		// �ϲü���
		vn.Initialize( 0, this->ViewDistance, -dwTempWidth);
		this->ClipPlaneUp.Initialize(Pos, vn, true);

		// �²ü���
		vn.Initialize( 0, -this->ViewDistance, -dwTempWidth);
		this->ClipPlaneDown.Initialize(Pos, vn, true);
	}
}

void SoaringLoong::Graphics3D::CCamera::UpdateCameraMatrix()
{
	CVector4D vMove(-WorldPos.x, -WorldPos.y, -WorldPos.z);
	CMatrix4x4 mMove;
	mMove.BuildMoveMatrix(vMove);

	switch (emType)
	{
	case SoaringLoong::Graphics3D::CAMERA_ELUER:
	{
		CMatrix4x4 mRotation;
		mRotation.BuildRotateMartix(-this->Direction.x, -this->Direction.y, -this->Direction.z);
		this->MatrixCamera.Multiply(&mMove, &mRotation);
	}break;
	case SoaringLoong::Graphics3D::CAMERA_UVN:
	{
		if (this->bUVNTargetNeedCompute)
		{
			// ����ǣ�����
			double phi = this->Direction.x;
			double theta = this->Direction.y;

			double sin_phi = CMathBase::Fast_Sin(phi);
			double cos_phi = CMathBase::Fast_Cos(phi);
			double sin_theta = CMathBase::Fast_Sin(theta);
			double cos_theta = CMathBase::Fast_Cos(theta);

			this->UVNTarget.x = -1 * sin_phi * sin_theta;
			this->UVNTarget.y = 1 * cos_phi;
			this->UVNTarget.z = 1 * sin_phi * cos_theta;
		}

		// ��ʱ��UVN
		CVector4D u, v, n;

		n = CVector4D::Subtract(this->UVNTarget, this->WorldPos);
		v = this->V;

		// ӦΪN��V�������һ��ƽ�棬���Կ�������U  
		u = CVector4D::Cross(v, n);

		// ��ΪV��N���ܲ���ֱ�����Է���V��ʹ��V��U��N����ֱ  
		v = CVector4D::Cross(n, u);

		this->U.VECTOR4D_Normalize(&u);
		this->V.VECTOR4D_Normalize(&v);
		this->N.VECTOR4D_Normalize(&n);

		// UVN�任����  
		CMatrix4x4 mUVN;
		mUVN.Initialize(
			U.x, V.x, N.x, 0,
			U.y, V.y, N.y, 0,
			U.z, V.z, N.z, 0,
			0, 0, 0, 1);

		MatrixCamera.Multiply(&mMove, &mUVN);

	}break;
	default:
		break;
	}
}

void SoaringLoong::Graphics3D::CCamera::UpdateProjectMatrix()
{
	MatrixProjection.Initialize(
		ViewDistance, 0, 0, 0,
		0, -ViewDistance, 0, 0,
		0, 0, 1, 1,
		0, 0, 0, 0); 
}

void SoaringLoong::Graphics3D::CCamera::UpdateScreenMatrix()
{
	MatrixScreen.Initialize(
		ScreenWidth / 2, 0, 0, 0,
		0, ScreenWidth / 2, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}
