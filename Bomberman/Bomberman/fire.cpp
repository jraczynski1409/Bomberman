#include "fire.h"

Fire::Fire(int row, int coll)
{
	this->row = row;
	this->coll = coll;
	this->putTime = al_get_time();
}