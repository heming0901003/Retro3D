#ifndef RETRO3D_SCENERENDERER_H
#define RETRO3D_SCENERENDERER_H

/*=============================================================================================
Scene Renderer

Renders walls, ceilings, floors and sprites.

Everything is software rendered (obviously for educational purposes, as it is super-inefficient).

==============================================================================================*/

#include <stdint.h>
#include <unordered_map>
#include "Object/weak_objectptr.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <algorithm>

struct SDL_Surface;
struct SDL_Texture;

namespace Retro3D
{
	class Level;
	class CameraComponent; // TODO: This is bad!
	class IRenderTarget;

	class SceneRenderer
	{
	public:
		SceneRenderer();
		~SceneRenderer();

		/** Renders the whole scene. */
		void RenderScene(IRenderTarget* arg_rendertarget);

        void SetResolution(int xRes, int yRes);

		/** Sets the Level to render. Called from the engine. */
		void SetLevel(Level* arg_level);

		/* Call this to fetch data (textures, etc.) from current level. */
		void LoadLevelData();

		void SetCameraComponent(CameraComponent* arg_comp); // TEMP

        /* Set distance based light fade rate (0.0 = none, 1.0 = normal, > 1.0 = fast). */
        void SetLightFade(float fade);

        /* Set ambient light intensity (between 0 and 1). */
        void SetAmbientLight(float light);

	private:
		Level* mLevel = nullptr;
		bool mLevelDataLoaded = false;
        SDL_Surface* mTextureSurfaceMap[256];
		SDL_Surface* mSkyboxTexture = nullptr;
		WeakObjectPtr<CameraComponent> mCameraComponent; // TODO: pass on some other structure
		float mFOV = 95;
        int mTexWidth = 320;
        int mTexHeight = 200;
		SDL_Texture* mRenderTexture;
		std::vector<unsigned char> mPixels;
		std::vector<unsigned char> mClearPixels;
		std::vector<float> mDepthBuffer;
		std::vector<float> mClearDepthBuffer;

        float mAmbientLight = 1.0f; // between 0 and 1
        float mLightFade = 0.0f; // 0 = none; < 1 = slow; > 1 = fast
        float mLightIntensities[2048]; // index = squared distance; value = light intensity (in range: 0,1)

        void RecalculateLightIntensities();

        inline float GetLightIntensity(const glm::vec2& dir)
        {
            const unsigned int iSqrt = std::min(2047u, (unsigned int)((dir.x * dir.x + dir.y * dir.y) * 20.0f));
            return mLightIntensities[iSqrt];
        }

        inline float GetLightIntensity(const glm::vec3& dir)
        {
            const unsigned int iSqrt = std::min(2047u, (unsigned int)((dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) * 20.0f));
            return mLightIntensities[iSqrt];
        }
	};

	class SpriteRenderObject
	{
	public:
		const glm::vec3 mPosition;
		const Texture* mTexture;
        const glm::vec2 mUVOffset;
        const glm::vec2 mUVScale;
		SpriteRenderObject(const glm::vec3& arg_pos, const Retro3D::Texture* arg_texture, const glm::vec2& arg_uvoffset, const glm::vec2& arg_uvscale)
			: mPosition(arg_pos), mTexture(arg_texture) , mUVOffset(arg_uvoffset), mUVScale(arg_uvscale)
		{ }
	};
}

#endif
