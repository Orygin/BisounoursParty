// BISOUNOURS PARTY : SKIN PROXY

#include "cbase.h"
#include "materialsystem/IMaterialProxy.h"
#include "materialsystem/IMaterial.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CSkinProxy : public IMaterialProxy
{
public:

	CSkinProxy();
	virtual ~CSkinProxy();
	virtual bool Init( IMaterial *pMaterial, KeyValues *pKeyValues );
	virtual void OnBind( void *pC_BaseEntity );
	virtual void Release( void ) { delete this; }
	virtual IMaterial *GetMaterial() { return NULL; }
};

CSkinProxy::CSkinProxy()
{
	DevMsg( 1, "CDummyMaterialProxy::CDummyMaterialProxy()\n" );
}

CSkinProxy::~CSkinProxy()
{
	DevMsg( 1, "CDummyMaterialProxy::~CDummyMaterialProxy()\n" );
}


bool CSkinProxy::Init( IMaterial *pMaterial, KeyValues *pKeyValues )
{
	DevMsg( 1, "CDummyMaterialProxy::Init( material = \"%s\" )\n", pMaterial->GetName() );
	return true;
}

void CSkinProxy::OnBind( void *pC_BaseEntity )
{
	DevMsg( 1, "CDummyMaterialProxy::OnBind( %p )\n", pC_BaseEntity );
}

EXPOSE_INTERFACE( CSkinProxy, IMaterialProxy, "skin" IMATERIAL_PROXY_INTERFACE_VERSION );
