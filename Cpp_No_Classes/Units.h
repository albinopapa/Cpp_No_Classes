#pragma once

#include "Framework.h"	// Included for Graphics
#include "Entity.h"		// Included for position and velocity
#include "Grid.h"		// Included for Grid::Path
#include <cstdint>		// Included for int32_t typedef

namespace Game
{
	namespace Units
	{
		enum class Job
		{
			Builder, Farmer, Fisher, Gatherer, FootSoldier, Mounted, Naval, Scout		
		};
		enum class JobState
		{
			// Builder,	Farmer,	   Fisher,	  Gatherer,	   Scount
			isBuilding, isFarming, isFishing, isGathering, isScouting,

			// Common
			isAttacking, 
			// Hasn't been assigned, or job no longer valid
			isIdle, 
			// Task complete, looking for next task
			isSearching, 
			// Picking up supplies for task 
			// ex: fishing pole broke, going to market to get a new one
			isPurchasing, 

			// Used when carrying resources or returning to job,
			// but still working
			isTransitioning
		};
		enum class Buffs
		{
			// Econo-buffs
			Agriculture, Technology, Labor,

			// Military-buffs
			Assassin, Horsebreader, Sailor, Eagleeye
		};

		template<Job j>struct traits{};

		template<>struct traits<Job::Builder>
		{
			static constexpr float max_speed = 310.f;
			static constexpr float attack = 6.f;
			static constexpr float base_hp = 25.f;
			static constexpr float armor = 0.f;
			static constexpr Job job = Job::Builder;
		};
		template<>struct traits<Job::Farmer>
		{
			static constexpr float max_speed = 300.f;
			static constexpr float attack = 5.f;
			static constexpr float base_hp = 20.f;
			static constexpr float armor = 0.f;
			static constexpr Job job = Job::Farmer;
		};
		template<>struct traits<Job::Fisher>
		{
			static constexpr float max_speed = 270.f;
			static constexpr float attack = 4.f;
			static constexpr float base_hp = 15.f;
			static constexpr float armor = 0.f;
			static constexpr Job job = Job::Fisher;
		};
		template<>struct traits<Job::Gatherer>
		{
			static constexpr float max_speed = 350.f;
			static constexpr float attack = 4.f;
			static constexpr float base_hp = 10.f;
			static constexpr float armor = 5.f;
			static constexpr Job job = Job::Gatherer;
		};
		template<>struct traits<Job::FootSoldier>
		{
			static constexpr float max_speed = 275.f;
			static constexpr float attack = 10.f;
			static constexpr float base_hp = 50.f;
			static constexpr float armor = 15.f;
			static constexpr Job job = Job::FootSoldier;
		};
		template<>struct traits<Job::Mounted>
		{
			static constexpr float max_speed = 375.f;
			static constexpr float attack = 20.f;
			static constexpr float base_hp = 50.f;
			static constexpr float armor = 25.f;
			static constexpr Job job = Job::Mounted;
		};
		template<>struct traits<Job::Naval>
		{
			static constexpr float max_speed = 325.f;
			static constexpr float attack = 20.f;
			static constexpr float base_hp = 50.f;
			static constexpr float armor = 5.f;
			static constexpr Job job = Job::Naval;
		};
		template<>struct traits<Job::Scout>
		{
			static constexpr float max_speed = 325.f;
			static constexpr float attack = 10.f;
			static constexpr float base_hp = 25.f;
			static constexpr float armor = 0.f;
			static constexpr Job job = Job::Scout;
		};

		struct vTable_Unit
		{
			void (*Update)( struct _Unit& unit, float dt, Grid::_Grid& grid  );
		};

		struct _Unit
		{
			template<class Traits>
			_Unit( Traits _traits, Team _team, Grid::_Grid& grid )
				:
				_Unit( _traits.attack, _traits.base_hp, _traits.armor, _traits.job, _team, grid )
			{

			}
			_Unit( float Attack, float HP, float Armor, Job _Job, Team _Team, Grid::_Grid& grid );

			operator _Entity&( );
			operator const _Entity&( )const;

			static constexpr Rectf rect = { -16.f,-32.f,16.f,32.f };
			static constexpr size_t max_aniframes = 16ull;

			vTable_Unit vtable;
			_Entity entity;
			Grid::_Path path;
			Vec2f jobLocation;
			const Vec2f* targetPosition = nullptr;

			std::vector<Vec2f> pathToGoal;

			float speed		= 0.f;
			float attack	= traits<Job::Builder>::attack;
			float hp		= traits<Job::Builder>::base_hp;
			float armor		= traits<Job::Builder>::armor;
			size_t aniframe = 0;
			const _Unit* p_attacker = nullptr;
			Job job = Job::Builder;
			JobState jobstate = JobState::isIdle;
			Buffs buff = Buffs::Agriculture;
		};

		template<Job _job>
		_Unit CreateUnit( Team team )
		{
			return _Unit(
				traits<_job>::attack,
				traits<_job>::base_hp,
				traits<_job>::armor,
				traits<_job>::job,
				team
			);
		}


		void Update( _Unit& _this, float dt, Grid::_Grid& grid );
		void ClampToScreen( _Unit& _this );
		void Draw( const _Unit& _this, Graphics& gfx );

		// Getters
		Vec2f GetPosition( const _Unit& _this );
		Job GetJob( const _Unit& _this );
		bool IsAlive( const _Unit& _this );
		bool IsIdle( const _Unit& _this );

		// Setters
		void SetAttackingUnit( _Unit& _this, const _Unit& pAttacker );
		void SetPosition( _Unit& _this, const Vec2f position );
		void SetTargetPosition( _Unit& _this, const Vec2f* position );
		void SetJobPosition( _Unit& _this, const Vec2f position );
	}

}