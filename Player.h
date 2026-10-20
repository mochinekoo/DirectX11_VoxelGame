#pragma once
#include "MochinekoEngine/BaseObject.h"
#include "MochinekoEngine/Box.h"
#include "MochinekoEngine/FBX.h"

class Player : public BaseObject{
private:
	Box* cmdBox_;
	bool isCmdOpen_;
	FBX* modelFBX_;
	bool isThree_;

	bool isJumping_;
public:

	Player();
	~Player();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};

