#include "ICamera.h"

D3DXVECTOR3 WorldX(1.0f,0.0f,0.0f);
D3DXVECTOR3 WorldY(0.0f,1.0f,0.0f);
D3DXVECTOR3 WorldZ(0.0f,0.0f,1.0f);

ICamera::ICamera()
{
	this->accumPitchDegrees = 0.0f;
	this->accumYawDegrees = 0.0f;
	this->accumRollDegrees = 0.0f;
	this->maxPitchDegrees = 90.0f;
	this->maxYawDegrees = 360.0f;
	this->maxRollDegrees = 90.0f;
	this->fovy = 0.0f;
	this->aspectRatio = 0.0f;
	this->zNear = 0.0f;
	this->zFar = 0.0f;
	D3DXQuaternionIdentity(&this->orientation);
	D3DXQuaternionIdentity(&this->rotQuaternion);
	D3DXMatrixIdentity(&this->projMatrix);
	D3DXMatrixIdentity(&this->viewMatrix);
	D3DXMatrixIdentity(&this->rotMatrix);
	this->eye = D3DXVECTOR3(0.0f,0.0f,0.0f);
	this->xAxis = D3DXVECTOR3(1.0f,0.0f,0.0f);
	this->yAxis = D3DXVECTOR3(0.0f,1.0f,0.0f);
	this->zAxis = D3DXVECTOR3(0.0f,0.0f,1.0f);
	this->cameraType = AIRCRAFT;
}

ICamera::~ICamera()
{
}

//строит матрицу проекции
void ICamera::SetProjectionMatrix(float fovY, float aspect, float zNear, float zFar)
{
	D3DXMatrixPerspectiveFovLH(&this->projMatrix,fovY,aspect,zNear,zFar);
	this->fovy = fovY;
	this->zNear = zNear;
	this->zFar = zFar;
	this->aspectRatio = aspect;
}

//возвращает матрицу проекции
D3DXMATRIXA16& ICamera::GetProjMatrix()
{
	return this->projMatrix;
}

//фокусирует камеру на определенную точку
void ICamera::LookAt(const D3DXVECTOR3 &target)
{
	this->zAxis = target - this->eye;
	D3DXVec3Normalize(&this->zAxis, &this->zAxis);

    this->viewDir = this->zAxis;

    D3DXVec3Cross(&this->xAxis, &WorldY, &this->zAxis);
    D3DXVec3Normalize(&this->xAxis, &this->xAxis);

    D3DXVec3Cross(&this->yAxis, &this->zAxis, &this->xAxis);
    D3DXVec3Normalize(&this->yAxis, &this->yAxis);
    D3DXVec3Normalize(&this->xAxis, &this->xAxis);

    D3DXMatrixIdentity(&this->viewMatrix);

	this->viewMatrix(0,0) = this->xAxis.x;
    this->viewMatrix(1,0) = this->xAxis.y;
    this->viewMatrix(2,0) = this->xAxis.z;
    this->viewMatrix(3,0) = -D3DXVec3Dot(&this->xAxis, &this->eye);

    this->viewMatrix(0,1) = this->yAxis.x;
    this->viewMatrix(1,1) = this->yAxis.y;
    this->viewMatrix(2,1) = this->yAxis.z;
    this->viewMatrix(3,1) = -D3DXVec3Dot(&this->yAxis, &this->eye);

    this->viewMatrix(0,2) = this->zAxis.x;
    this->viewMatrix(1,2) = this->zAxis.y;
    this->viewMatrix(2,2) = this->zAxis.z;
    this-> viewMatrix(3,2) = -D3DXVec3Dot(&this->zAxis, &this->eye);

	this->accumPitchDegrees = D3DXToDegree(asinf(this->viewMatrix(1,2)));
	this->accumYawDegrees = D3DXToDegree(atan2f(this->viewMatrix._13, this->viewMatrix._33));

	D3DXQuaternionRotationMatrix(&this->orientation, &this->viewMatrix);
}

//возвращает матрицу вида
D3DXMATRIXA16& ICamera::GetViewMatrix()
{
	return this->viewMatrix;
}

//двигает камеру в её локальных координатах
void ICamera::Move(float dx, float dy, float dz)
{
    D3DXVECTOR3 forwards;

    if (this->cameraType == LAND)
    {
        D3DXVec3Cross(&forwards, &this->xAxis, &this->yAxis);
        D3DXVec3Normalize(&forwards, &forwards);
    }
    else
    {
        forwards = this->viewDir;
    }
    eye += this->xAxis * dx;
    eye += this->yAxis * dy;
    eye += forwards * dz;

	this->UpdateViewMatrix();
}

//двигает камеру в произвольном направлении
void ICamera::Move(const D3DXVECTOR3 &direction, const D3DXVECTOR3 &amount)
{
	this->eye.x += direction.x * amount.x;
	this->eye.y += direction.y * amount.y;
	this->eye.z += direction.z * amount.z;

	this->UpdateViewMatrix();
}

//двигает камеру в произвольном направлении
void ICamera::Move(const D3DXVECTOR3 &direction, float dx, float dy, float dz)
{
	this->eye.x += direction.x * dx;
	this->eye.y += direction.y * dy;
	this->eye.z += direction.z * dz;

	this->UpdateViewMatrix();
}

//двигает камеру в глобальных координатах
void ICamera::MoveGlobal(float dx, float dy, float dz)
{
	eye += WorldX * dx;
    eye += WorldY * dy;
    eye += WorldZ * dz;

	this->UpdateViewMatrix();
}

//вращение
void ICamera::Roll(float degrees)
{
	this->accumRollDegrees += degrees;

	//проверка на полный оборот, нет смысла хранить значения больше 360 градусов
	if(this->accumRollDegrees >= 360.0f)
    {
        this->accumRollDegrees -= 360.0f;
    }

    if (this->accumRollDegrees <= -360.0f)
    {
        this->accumRollDegrees += -360.0f;
    }

    if (this->accumRollDegrees > maxRollDegrees)
    {
        degrees = maxRollDegrees - (this->accumRollDegrees - degrees);
        this->accumRollDegrees = maxRollDegrees;
    }

    if (this->accumRollDegrees < -maxRollDegrees)
    {
        degrees = -maxRollDegrees - (this->accumRollDegrees - degrees);
        this->accumRollDegrees = -maxRollDegrees;
    }

	D3DXQuaternionRotationAxis(&this->rotQuaternion, &WorldZ, D3DXToRadian(degrees));
    D3DXQuaternionMultiply(&this->orientation, &this->orientation, &this->rotQuaternion);
}

//наклон
void ICamera::Pitch(float degrees)
{
	this->accumPitchDegrees += degrees;

	//проверка на полный оборот, нет смысла хранить значения больше 360 градусов
	if (this->accumPitchDegrees >= 360.0f)
    {
        this->accumPitchDegrees -= 360.0f;
    }

    if (this->accumPitchDegrees <= -360.0f)
    {
        this->accumPitchDegrees += 360.0f;
    }

    if (this->accumPitchDegrees > maxPitchDegrees)
    {
        degrees = maxPitchDegrees- (this->accumPitchDegrees - degrees);
        this->accumPitchDegrees = maxPitchDegrees;
    }

    if (this->accumPitchDegrees < -maxPitchDegrees)
    {
        degrees = -maxPitchDegrees - (this->accumPitchDegrees - degrees);
        this->accumPitchDegrees = -maxPitchDegrees;
    }

	D3DXQuaternionRotationAxis(&this->rotQuaternion, &WorldX, D3DXToRadian(degrees));
    D3DXQuaternionMultiply(&this->orientation, &this->orientation, &this->rotQuaternion);
}

//рысканье
void ICamera::Yaw(float degrees)
{
	this->accumYawDegrees += degrees;

	//проверка на полный оборот, нет смысла хранить значения больше 360 градусов
	if (this->accumYawDegrees >= 360.0f)
    {
        this->accumYawDegrees -= 360.0f;
    }

    if (this->accumYawDegrees <= -360.0f)
    {
        this->accumYawDegrees += 360.0f;
    }

    if (this->accumYawDegrees > maxYawDegrees)
    {
        degrees = maxYawDegrees - (this->accumYawDegrees - degrees);
        this->accumYawDegrees = maxYawDegrees;
    }

    if (this->accumYawDegrees < -maxYawDegrees)
    {
        degrees = -maxYawDegrees - (this->accumYawDegrees - degrees);
        this->accumYawDegrees = -maxYawDegrees;
    }

	D3DXQuaternionRotationAxis(&this->rotQuaternion, &WorldY, D3DXToRadian(degrees));
	D3DXQuaternionMultiply(&this->orientation, &this->rotQuaternion, &this->orientation);
}

//устанавливает позицию камеры
void ICamera::SetEyeVector(D3DXVECTOR3 eye)
{
	this->eye = eye;
}

//обновляет матрицу вида
D3DXMATRIXA16& ICamera::UpdateViewMatrix()
{
	D3DXQuaternionNormalize(&this->orientation, &this->orientation);
    D3DXMatrixRotationQuaternion(&this->viewMatrix, &this->orientation);

    this->xAxis = D3DXVECTOR3(this->viewMatrix(0,0), this->viewMatrix(1,0), this->viewMatrix(2,0));
    this->yAxis = D3DXVECTOR3(this->viewMatrix(0,1), this->viewMatrix(1,1), this->viewMatrix(2,1));
    this->zAxis = D3DXVECTOR3(this->viewMatrix(0,2), this->viewMatrix(1,2), this->viewMatrix(2,2));
    this->viewDir = this->zAxis;

    this->viewMatrix(3,0) = -D3DXVec3Dot(&this->xAxis, &this->eye);
    this->viewMatrix(3,1) = -D3DXVec3Dot(&this->yAxis, &this->eye);
    this->viewMatrix(3,2) = -D3DXVec3Dot(&this->zAxis, &this->eye);

	return this->viewMatrix;
}

//возвращает угол рыскания
float ICamera::GetYaw()
{
	return this->accumYawDegrees;
}

//возвращает угол вращения
float ICamera::GetRoll()
{
	return this->accumRollDegrees;
}

//возвращает угол наклона
float ICamera::GetPitch()
{
	return this->accumPitchDegrees;
}

void ICamera::Reset()
{
	this->accumPitchDegrees = 0.0f;
	this->accumYawDegrees = 0.0f;
	this->accumRollDegrees = 0.0f;
	this->maxPitchDegrees = 90.0f;
	this->maxYawDegrees = 360.0f;
	this->maxRollDegrees = 90.0f;
	this->fovy = 0.0f;
	this->aspectRatio = 0.0f;
	this->zNear = 0.0f;
	this->zFar = 0.0f;
	D3DXQuaternionIdentity(&this->orientation);
	D3DXQuaternionIdentity(&this->rotQuaternion);
	D3DXMatrixIdentity(&this->projMatrix);
	D3DXMatrixIdentity(&this->viewMatrix);
	D3DXMatrixIdentity(&this->rotMatrix);
	this->eye = D3DXVECTOR3(0.0f,0.0f,0.0f);
	this->xAxis = D3DXVECTOR3(1.0f,0.0f,0.0f);
	this->yAxis = D3DXVECTOR3(0.0f,1.0f,0.0f);
	this->zAxis = D3DXVECTOR3(0.0f,0.0f,1.0f);
	this->cameraType = AIRCRAFT;
}