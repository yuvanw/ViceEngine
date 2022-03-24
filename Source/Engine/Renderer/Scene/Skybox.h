#pragma once

#include "CoreMinimal.h"
#include "Geometry/Mesh.h"
#include "RHI/Cubemap.h"

class FScene;

template <typename ObjectType>
class FSetScene;

/**
 * A skybox is a cube that is textured using a cubemap.
 * It is used to give the illusion that the game environment
 * is larger than it really is.
 */
class FSkybox
{
public:
	explicit FSkybox(const FStringId& InName, const FCubemap& InCubemap);
	~FSkybox() = default;

	FSkybox(const FSkybox&) = default;
	FSkybox& operator=(const FSkybox&) = default;
	FSkybox(FSkybox&&) = default;
	FSkybox& operator=(FSkybox&&) = default;

	// Getters.
	const FStringId& GetName() const 
	{ 
		return Name; 
	}
	FMesh& GetMesh() 
	{ 
		return CubeMesh;
	}
	const FMesh& GetMesh() const
	{ 
		return CubeMesh; 
	}
	FCubemap& GetCubemap() 
	{ 
		return Cubemap; 
	}
	const FCubemap& GetCubemap() const 
	{ 
		return Cubemap; 
	}

	// Setters.
	void SetVisible();
	void SetInvisible();

	bool IsVisible() const;

private:
	FStringId Name;
	FScene* Scene;
	FMesh CubeMesh;
	FCubemap Cubemap;
	bool bIsVisible;

	friend class FSetScene<FSkybox>;
};
