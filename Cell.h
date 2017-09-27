#pragma once

typedef enum
{
	CELL_DEAD,
	CELL_LIVING,
	NUM_CELL_TYPES
} cell_states;

class Cell
{
	public:
		Cell();
		~Cell();

		int TimeStep(void);
		int GetCurrentState(void);
		int SetNextState(cell_states);
	
	private:
		cell_states currentState;
		cell_states nextState;
};