#pragma once
#include "MochinekoEngine/BaseObject.h"
#include "MochinekoEngine/Box.h"

class Player : public BaseObject{
private:
	Box* cmdBox_;
	bool isCmdOpen_;
public:

	Player();
	~Player();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};

