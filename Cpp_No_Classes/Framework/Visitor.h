#pragma once

class Visitor
{
public:
	Visitor() = default;
	virtual ~Visitor() = default;

	virtual void Visit( class Observer &Obs ) = 0;
};

