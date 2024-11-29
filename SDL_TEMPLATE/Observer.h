#pragma once

class Subject;

class Observer {
public:
	virtual void update(const Subject& subject) = 0;
};

