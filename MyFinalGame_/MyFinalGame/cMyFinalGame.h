/*
	This class is your specific game
*/

#ifndef EAE6320_CEXAMPLEGAME_H
#define EAE6320_CEXAMPLEGAME_H

// Includes
//=========

#include <Engine/Application/iApplication.h>
#include <Engine/Results/Results.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Physics/sRigidbodyState.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include "Resource Files/Resource.h"
#endif


namespace eae6320
{
	class RigidbodyObject
	{
	public:
		eae6320::Physics::sRigidBodyState rbState;
	};

	class RenderableObject : public RigidbodyObject
	{
	public:
		RenderableObject(std::vector<eae6320::Graphics::MyMesh*>& i_meshes, std::vector<eae6320::Graphics::MyEffect*>& i_effects)
		{
			meshes = i_meshes;
			effects = i_effects;
			alive = true;
			hurtingRemainTime = 0.0f;
			health = 100;
		}
		
		std::vector<eae6320::Graphics::MyMesh*> meshes;
		std::vector<eae6320::Graphics::MyEffect*> effects;
		bool alive;
		float hurtingRemainTime;
		int health;
	};

	class MyCamera : public RigidbodyObject
	{
	};

	enum MyGameState
	{
		default,
		removeOneObject,
		changeOneEffect,
		changeObjectsMesh
	};

	class MyFinalGame final : public Application::iApplication
	{
	public:
		MyFinalGame()
		{
			myGameState = MyGameState::default;
			mainObjIndex = 0;
			camera.rbState.position.z = 5.0f;
			firingTypeCD[0] = 1.0f;
			firingTypeCD[1] = 0.2f;
			firingTypeCD[2] = 0.2f;
			planeStopFiringRemainTime[0] = planeStopFiringRemainTime[1] = 0.0f;
			planeFiringType[0] = planeFiringType[1] = 0;
			generateEnemyCD = 1.5f;
			StopGeneratingEnemyRemainTime = 0.0f;
			planePower[0] = planePower[1] = 0.0f;
		};
		~MyFinalGame() { CleanUpGraphicsContent(); };

	private:
		RenderableObject* GetMainObject()
		{
			if (0 <= mainObjIndex && mainObjIndex < objects.size())
				return objects[mainObjIndex];
			else
				return nullptr;
		}

		// Configuration
		//--------------

#if defined( EAE6320_PLATFORM_WINDOWS )
		// The main window's name will be displayed as its caption (the text that is displayed in the title bar).
		// You can make it anything that you want, but please keep the platform name and debug configuration at the end
		// so that it's easy to tell at a glance what kind of build is running.
		const char* GetMainWindowName() const final
		{
			return "Haonan Dong - Final"
				" -- "
#if defined( EAE6320_PLATFORM_D3D )
				"Direct3D"
#elif defined( EAE6320_PLATFORM_GL )
				"OpenGL"
#endif
#ifdef _DEBUG
				" -- Debug"
#endif
			;
		}		
		// Window classes are almost always identified by name;
		// there is a unique "ATOM" associated with them,
		// but in practice Windows expects to use the class name as an identifier.
		// If you don't change the name below
		// your program could conceivably have problems if it were run at the same time on the same computer
		// as one of your classmate's.
		// You don't need to worry about this for our class,
		// but if you ever ship a real project using this code as a base you should set this to something unique
		// (a generated GUID would be fine since this string is never seen)
		const char* GetMainWindowClassName() const final { return "Haonan(Amos) Dong's Main Window Class"; }
		// The following three icons are provided:
		//	* IDI_EAEGAMEPAD
		//	* IDI_EAEALIEN
		//	* IDI_VSDEFAULT_LARGE / IDI_VSDEFAULT_SMALL
		// If you want to try creating your own a convenient website that will help is: http://icoconvert.com/
		/*const WORD* GetLargeIconId() const final { static constexpr WORD iconId_large = IDI_VSDEFAULT_LARGE; return &iconId_large; }
		const WORD* GetSmallIconId() const final { static constexpr WORD iconId_small = IDI_VSDEFAULT_SMALL; return &iconId_small; }*/
		const WORD* GetLargeIconId() const final { static constexpr WORD iconId_large = IDI_EAEGAMEPAD; return &iconId_large; }
		const WORD* GetSmallIconId() const final { static constexpr WORD iconId_small = IDI_EAEALIEN; return &iconId_small; }
#endif

		// Run
		//----
		void UpdateBasedOnInput() final;
		void UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) final;

		// Initialize / Clean Up
		//----------------------
		void InitGraphicsContent();
		cResult Initialize() final;
		void CleanUpGraphicsContent();
		cResult CleanUp() final;

		virtual void UpdateSimulationBasedOnInput() override;
		void UpdateSimulationBasedOnTimePerCharacter(const float i_elapsedSecondCount_sinceLastUpdate, RenderableObject* character, const char* characterTag);
		virtual void UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;
		virtual void SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate) override;
	
	public:
		MyGameState myGameState;
	private:
		std::vector<RenderableObject*> objects;
		RenderableObject* plane[2];
		std::vector<RenderableObject*> plane_bullets[2];
		RenderableObject* planePowerBar[2];
		RenderableObject* planeHealthBar[2];
		std::vector<RenderableObject*> enemies;		
		size_t mainObjIndex;
		MyCamera camera;
		unsigned int counter = 0;
		float firingTypeCD[3];
		float planeStopFiringRemainTime[2];
		int planeFiringType[2];
		float generateEnemyCD;
		float StopGeneratingEnemyRemainTime;
		float planePower[2];
	};
}

// Result Definitions
//===================

namespace eae6320
{
	namespace Results
	{
		namespace Application
		{
			// You can add specific results for your game here:
			//	* The System should always be Application
			//	* The __LINE__ macro is used to make sure that every result has a unique ID.
			//		That means, however, that all results _must_ be defined in this single file
			//		or else you could have two different ones with equal IDs.
			//	* Note that you can define multiple Success codes.
			//		This can be used if the caller may want to know more about how a function succeeded.
			constexpr cResult ExampleResult( IsFailure, eSystem::Application, __LINE__, Severity::Default );
		}
	}
}

#endif	// EAE6320_CEXAMPLEGAME_H