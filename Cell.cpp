#include "Cell.h"

Cell::Cell()
{
	currentState = CELL_DEAD;
}

Cell::~Cell()
{

}

int Cell::TimeStep()
{
	currentState = nextState;
	return 1;
}

int Cell::GetCurrentState()
{
	return currentState;
}

int Cell::SetNextState(cell_states newState)
{
	nextState = newState;
	return 1;
}