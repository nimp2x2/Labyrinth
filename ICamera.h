#ifndef ICAMERA_H
#define ICAMERA_H

#include "ILogger.h"

class ICamera
{
	enum CameraType { LAND, AIRCRAFT }; //тип камеры
	CameraType cameraType;
	float accumPitchDegrees; //для хранения углов
	float accumYawDegrees;
	float accumRollDegrees;
	float maxPitchDegrees; //максимальные углы поворота
	float maxYawDegrees;
	float maxRollDegrees;
    float fovy; //область видимости по у
    float aspectRatio; //аспект
    float zNear; // ближняя плоскость
    float zFar; //дальняя
    D3DXVECTOR3 viewDir; //направление обзора
    D3DXQUATERNION orientation; //кватернионы для вращения
	D3DXQUATERNION rotQuaternion;
    D3DXMATRIXA16 viewMatrix; //матрицы 
    D3DXMATRIXA16 projMatrix;
	D3DXMATRIXA16 rotMatrix;
	D3DXVECTOR3 xAxis; //локальные координаты камеры
	D3DXVECTOR3 zAxis;
	D3DXVECTOR3 yAxis;
public:
	D3DXVECTOR3 eye; //позиция наблюдателя(камеры)
	ICamera();
	~ICamera();

	void SetEyeVector(D3DXVECTOR3 eye); //устанавливает позицию камеры
	void SetProjectionMatrix(float fovY, float aspect, float zNear, float zFar); //строит матрицу проекции
	void Move(float dx, float dy, float dz); //двигает камеру в её локальных координатах
	void Move(const D3DXVECTOR3 &direction, const D3DXVECTOR3 &amount); //двигает камеру в произвольном направлении
	void Move(const D3DXVECTOR3 &direction, float dx, float dy, float dz); //двигает камеру в произвольном направлении
	void MoveGlobal(float dx, float dy, float dz); //двигает камеру в глобальных координатах
	void Pitch(float degrees); //наклон
	void Yaw(float degrees); //рысканье
	void Roll(float degrees); //вращение
	D3DXMATRIXA16& UpdateViewMatrix(); //обновлении матрицы вида
	void LookAt(const D3DXVECTOR3 &target); //фокусирует камеру на определенную точку
	float GetYaw(); //возвращает углы наклона, рысканья и вращения
	float GetPitch();
	float GetRoll();
	void Reset();
	D3DXMATRIXA16& GetViewMatrix(); //возвращает матрицу вида
	D3DXMATRIXA16& GetProjMatrix(); //возвращает матрицу проекции
};

#endif

