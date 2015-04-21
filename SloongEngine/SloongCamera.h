#pragma once
#include "SloongVector.h"
#include "SloongPlane.h"
#include "SloongMatrix.h"
namespace SoaringLoong
{
	using namespace Math;
	using namespace Math::Vector;
	using namespace Math::Matrix;
	namespace Graphics3D
	{
		enum CAMERA_TYPE
		{
			CAMERA_ELUER,
			CAMERA_UVN,
		};
		class SLOONGENGINE_API CCamera
		{
		public:
			CCamera();
			virtual ~CCamera();

		public:
			virtual void Initialize(CAMERA_TYPE emType, const POINT4D& Position, const POINT4D& Direction, LPPOINT4D Target,
								LPVECTOR4D v, bool NeedTarget, double NearZ, double farZ, double FOV, double ScreenWidth, double ScreenHeight );

			virtual void UpdateCameraMatrix();
			virtual void UpdateProjectMatrix();
			virtual void UpdateScreenMatrix();

		public:
			POINT4D WorldPos; // ��������������
			VECTOR4D Direction; // ����ĳ������Ĭ�ϵĳ���
			CAMERA_TYPE emType; // �������
			VECTOR4D U, V, N; // UVN�����u,v,n����
			POINT4D UVNTarget; // UVN�����Ŀ���
			bool bUVNTargetNeedCompute; // UVN�����Ŀ����Ƿ���Ҫ���ݳ�����㣬UVNTarget�Ѹ��� 
			double ViewDistance; // �Ӿ�
			double FOV; // ��Ұ�Ƕ�
			double NearZ; // ���ü�����
			double FarZ; // Զ�ü�����
			PLANE3D  ClipPlaneLeft, ClipPlaneRight, ClipPlaneUp, ClipPlaneDown; // �������Ҳü�ƽ��
			double ViewPlaneWidth, ViewPlaneHeight; // ͸��ƽ��Ŀ�͸�
			double ScreenWidth, ScreenHeight; // ��Ļ���
			double ScreenCenterX, ScreenCenterY; // ��Ļ��������
			double AspectRatio; // ��߱�

			MATRIX4X4 MatrixCamera; // ����任����
			MATRIX4X4 MatrixProjection; // ͸��ͶӰ�任����
			MATRIX4X4 MatrixScreen; // ��Ļ�任����

		};
	}
}
