#ifndef ICAMERA_H
#define ICAMERA_H

#include "ILogger.h"

class ICamera
{
	enum CameraType { LAND, AIRCRAFT }; //��� ������
	CameraType cameraType;
	float accumPitchDegrees; //��� �������� �����
	float accumYawDegrees;
	float accumRollDegrees;
	float maxPitchDegrees; //������������ ���� ��������
	float maxYawDegrees;
	float maxRollDegrees;
    float fovy; //������� ��������� �� �
    float aspectRatio; //������
    float zNear; // ������� ���������
    float zFar; //�������
    D3DXVECTOR3 viewDir; //����������� ������
    D3DXQUATERNION orientation; //����������� ��� ��������
	D3DXQUATERNION rotQuaternion;
    D3DXMATRIXA16 viewMatrix; //������� 
    D3DXMATRIXA16 projMatrix;
	D3DXMATRIXA16 rotMatrix;
	D3DXVECTOR3 xAxis; //��������� ���������� ������
	D3DXVECTOR3 zAxis;
	D3DXVECTOR3 yAxis;
public:
	D3DXVECTOR3 eye; //������� �����������(������)
	ICamera();
	~ICamera();

	void SetEyeVector(D3DXVECTOR3 eye); //������������� ������� ������
	void SetProjectionMatrix(float fovY, float aspect, float zNear, float zFar); //������ ������� ��������
	void Move(float dx, float dy, float dz); //������� ������ � � ��������� �����������
	void Move(const D3DXVECTOR3 &direction, const D3DXVECTOR3 &amount); //������� ������ � ������������ �����������
	void Move(const D3DXVECTOR3 &direction, float dx, float dy, float dz); //������� ������ � ������������ �����������
	void MoveGlobal(float dx, float dy, float dz); //������� ������ � ���������� �����������
	void Pitch(float degrees); //������
	void Yaw(float degrees); //��������
	void Roll(float degrees); //��������
	D3DXMATRIXA16& UpdateViewMatrix(); //���������� ������� ����
	void LookAt(const D3DXVECTOR3 &target); //���������� ������ �� ������������ �����
	float GetYaw(); //���������� ���� �������, �������� � ��������
	float GetPitch();
	float GetRoll();
	void Reset();
	D3DXMATRIXA16& GetViewMatrix(); //���������� ������� ����
	D3DXMATRIXA16& GetProjMatrix(); //���������� ������� ��������
};

#endif

