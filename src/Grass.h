#pragma once
#include "Entity.h"
#include "Environment.h"
class SimainBase;


class Grass : public Entity
{
public:
	Grass(SimianBase * theApp, char * theName);
	~Grass();

	EnvironmentState mState;

	int		TempAgeCounter;				//Temporary variables for the age counter.
	int		FoodLevel;					//Variable for the Food level for the amount of time the food can be eaten by an animal.
	int		FoodLimit;					//Variable for the limit of the food each level.
	int		CurrentFoodLevel;			//Current Food level of the Grass.
	int		AnimationSwitch;			//Condition to switch the animation.
	bool	EatenStateSwtich;
	bool	OccupiedStateSwitch;
	int		AgeUp;
	int		FoodDeath;
	int		MaxFoodLimit;


	// Variables that affects the plant grounding state
	bool	PastMaturity;				//Check if the Grass has past sexual maturity
	bool	GetMature();

	virtual void	Update();
	void			Render();
	virtual void	FSM();				// Current state function which displays the current state of the Grass.
	void			AnimationState();	// Animations State is used to provide the Animation for respective stages for the grass.


	void	GrowState();				// Growing state function
	void	ReproductionState();		// Reproduction state function

	// Factors for the eaten state.
	int		EatingFoodLimitStage1;		//Condition for the food limit in the eaten state. ( stage 1)
	int		EatingFoodLimitStage2;		//Condition for the food limit in the eaten state. ( stage 2)
	int		EatingFoodLimitStage3;		//Condition for the food limit in the eaten state. ( stage 3)

	bool	IsEaten;					//conditoin to check if the Grass is being eaten by an animal.
	bool	IsOccupied;				//Condition to check if the grass is being occupied by a prey ( by request of brian)
	bool	GetEaten();
	bool	GetOccupied();
	void	SetOccupied(bool thestate);	//For Brian Used
	void	SetEating(bool thestate);	//For Brian's used
	void	EatenState();				//Eaten state Function
	void	Recover();
	int		GetFoodMax();

	// Animation thingy
	int 	ANIMATION_SEEDGROWING;
	int		ANIMATION_SPROUTGROWING;
	int		ANIMATION_YOUNGGROWING;
	int		ANIMATION_EATING;
	int		ANIMATION_GROWN;

	// Dummy Functions
	virtual void Caught();
	virtual bool SearchConditionCheck(Entity * curTarget);
	virtual void Reset();

};