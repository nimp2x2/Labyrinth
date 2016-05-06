#ifndef IRESOURCE_MANAGER_H
#define	IRESOURCE_MANAGER_H

#include "ILogger.h"
#include "IMesh.h"

template <class T>
class resourceQueue
{
public:
	resourceQueue *next;
	T* resource;
	wchar_t* resourcePath;
};

class IResourceManager
{
	LPDIRECT3DDEVICE9	device;
	resourceQueue<IDirect3DTexture9> *firstTexture;
	resourceQueue<IDirect3DTexture9> *lastTexture;
	resourceQueue<ID3DXEffect> *firstEffect;
	resourceQueue<ID3DXEffect> *lastEffect;
	resourceQueue<ISimpleMesh> *firstSimpleMesh;
	resourceQueue<ISimpleMesh> *lastSimpleMesh;

public:
	bool	LoadTexture(wchar_t* texName);
	bool	UnloadTexture(wchar_t* texName);
	LPDIRECT3DTEXTURE9	GetTexture(wchar_t* texName);
	void	PurgeTexQueue();

	bool	LoadEffect(wchar_t* effectName);
	bool	UnloadEffect(wchar_t* effectName);
	ID3DXEffect* GetEffect(wchar_t* effectName);
	void	PurgeEffectQueue();

	bool	LoadSimpleMesh(wchar_t* meshName, LPDIRECT3DTEXTURE9 meshTexture = NULL);
	bool	LoadSimpleMesh(wchar_t* meshName, ISimpleMesh *mesh);
	bool	UnloadSimpleMesh(wchar_t* meshName);
	ISimpleMesh* GetSimpleMesh(wchar_t* meshName);
	void	PurgeSimpleMeshQueue();

	void	PurgeAll();

	IResourceManager(LPDIRECT3DDEVICE9 device);
	~IResourceManager();
};

#endif